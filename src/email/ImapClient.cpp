// IMAP 客户端实现 — 通过 VMime 库连接 IMAP 服务器并拉取邮件数据
//
// VMime API 要点：
// - session::create(propertySet) 创建会话，getStore("imaps") 获取 IMAP store
// - store->setProperty() 设置服务器地址/端口/认证信息（自动添加协议前缀）
// - folder->getAndFetchMessages() 一次性拉取邮件元数据（信封、标志、大小、UID）
// - headerFieldValue 是 const shared_ptr，需用 vmime::dynamicCast<const T>() 转换

#include "ImapClient.h"

#include <vmime/vmime.hpp>

#include <QUrl>

ImapClient::ImapClient(QObject *parent)
    : QObject(parent)
{
}

ImapClient::~ImapClient()
{
    doDisconnect();
}

EmailAccount* ImapClient::account() const
{
    return m_account;
}

void ImapClient::setAccount(EmailAccount *account)
{
    if (m_account == account)
        return;
    m_account = account;
    emit accountChanged();
}

bool ImapClient::isConnected() const
{
    return m_connected;
}

void ImapClient::connectToServer()
{
    if (!m_account) {
        emit errorOccurred(tr("No account configured"));
        return;
    }

    m_account->setConnectionState(EmailAccount::Connecting);

    try {
        doConnect();
        m_connected = true;
        m_account->setConnectionState(EmailAccount::Connected);
        emit connectedChanged();
    } catch (const std::exception &e) {
        m_connected = false;
        m_account->setConnectionState(EmailAccount::Error);
        emit connectionError(QString::fromUtf8(e.what()));
        emit connectedChanged();
    }
}

void ImapClient::disconnectFromServer()
{
    doDisconnect();
    m_connected = false;
    if (m_account)
        m_account->setConnectionState(EmailAccount::Disconnected);
    emit connectedChanged();
}

void ImapClient::fetchFolderList()
{
    if (!m_connected) {
        emit errorOccurred(tr("Not connected to server"));
        return;
    }
    try {
        doFetchFolders();
    } catch (const std::exception &e) {
        emit errorOccurred(QString::fromUtf8(e.what()));
    }
}

void ImapClient::fetchMessages(const QString &folderPath)
{
    if (!m_connected) {
        emit errorOccurred(tr("Not connected to server"));
        return;
    }
    try {
        doFetchMessages(folderPath);
    } catch (const std::exception &e) {
        emit errorOccurred(QString::fromUtf8(e.what()));
    }
}

void ImapClient::fetchMessageContent(int uid)
{
    Q_UNUSED(uid)
    // TODO: 实现邮件正文内容拉取（通过 UID 获取完整邮件）
}

// 建立 IMAP 连接：根据 SSL 设置选择 imaps/imap 协议
void ImapClient::doConnect()
{
    const std::string proto = m_account->imapSsl() ? "imaps" : "imap";

    vmime::propertySet props;
    props["store.protocol"] = proto;

    m_session = vmime::net::session::create(props);
    m_store = m_session->getStore(proto);

    // setProperty 会自动添加协议前缀（如 "store.transport.server.address"）
    m_store->setProperty("server.address", m_account->imapServer().toStdString());
    m_store->setProperty("server.port", m_account->imapPort());
    m_store->setProperty("auth.username", m_account->username().toStdString());
    m_store->setProperty("auth.password", m_account->password().toStdString());

    m_store->connect();
}

void ImapClient::doDisconnect()
{
    try {
        if (m_store && m_store->isConnected())
            m_store->disconnect();
    } catch (...) {
    }
    m_store.reset();
    m_session.reset();
}

// 遍历 IMAP 根文件夹下的所有子文件夹
void ImapClient::doFetchFolders()
{
    auto rootFolder = m_store->getRootFolder();
    auto subFolders = rootFolder->getFolders(true);

    QStringList folders;
    for (const auto &f : subFolders) {
        const auto path = f->getFullPath();
        folders.append(QString::fromStdString(
            path.toString("/", vmime::charset::getLocalCharset())));
    }
    emit folderListReady(folders);
}

// 拉取指定文件夹的邮件列表（仅元数据，不含正文）
void ImapClient::doFetchMessages(const QString &folderPath)
{
    // 将 QString 路径转换为 VMime 的 utility::path
    vmime::utility::path vmimePath(
        vmime::utility::path::fromString(
            folderPath.toStdString(), "/", vmime::charset::getLocalCharset()));

    auto folder = m_store->getFolder(vmimePath);
    folder->open(vmime::net::folder::MODE_READ_ONLY);

    const size_t count = folder->getMessageCount();
    if (count == 0) {
        folder->close(false);
        emit messageListReady({});
        return;
    }

    // 批量拉取属性：信封（发件人/收件人/主题/日期）+ 标志 + 大小 + UID
    const vmime::net::fetchAttributes fetchAttrs(
        vmime::net::fetchAttributes::ENVELOPE
        | vmime::net::fetchAttributes::FLAGS
        | vmime::net::fetchAttributes::SIZE
        | vmime::net::fetchAttributes::UID
        | vmime::net::fetchAttributes::PEEK);

    auto msgs = folder->getAndFetchMessages(
        vmime::net::messageSet::byNumber(1, count), fetchAttrs);

    const auto localCharset = vmime::charset::getLocalCharset();

    QList<EmailMessage *> messages;
    messages.reserve(static_cast<int>(msgs.size()));

    for (const auto &msg : msgs) {
        auto *emailMsg = new EmailMessage(this);

        emailMsg->setUid(std::stoul(std::string(msg->getUID())));
        emailMsg->setSize(static_cast<int>(msg->getSize()));

        // 解析邮件标志：已读/已标记
        const int flags = msg->getFlags();
        emailMsg->setIsRead(flags & vmime::net::message::FLAG_SEEN);
        emailMsg->setIsFlagged(flags & vmime::net::message::FLAG_MARKED);

        auto header = msg->getHeader();
        if (!header) {
            messages.append(emailMsg);
            continue;
        }

        // 主题：headerFieldValue → dynamicCast<const vmime::text>
        if (header->hasField(vmime::fields::SUBJECT)) {
            auto field = header->Subject();
            auto fieldValue = field->getValue();
            auto textVal = vmime::dynamicCast<const vmime::text>(fieldValue);
            if (textVal) {
                emailMsg->setSubject(QString::fromStdString(
                    textVal->getConvertedText(localCharset)));
            }
        }

        // 发件人：headerFieldValue → dynamicCast<const vmime::mailboxList>
        if (header->hasField(vmime::fields::FROM)) {
            auto field = header->From();
            auto fieldValue = field->getValue();
            auto mboxList = vmime::dynamicCast<const vmime::mailboxList>(fieldValue);
            if (mboxList && mboxList->getMailboxCount() > 0) {
                auto mbox = mboxList->getMailboxAt(0);
                emailMsg->setFrom(QString::fromStdString(
                    mbox->getEmail().toString()));
                emailMsg->setFromName(QString::fromStdString(
                    mbox->getName().getConvertedText(localCharset)));
            }
        }

        // 收件人：headerFieldValue → dynamicCast<const vmime::addressList>
        // 每个地址需要再 dynamicCast<const vmime::mailbox> 获取邮箱
        if (header->hasField(vmime::fields::TO)) {
            auto field = header->To();
            auto fieldValue = field->getValue();
            auto addrList = vmime::dynamicCast<const vmime::addressList>(fieldValue);
            if (addrList && addrList->getAddressCount() > 0) {
                QStringList toList;
                for (size_t i = 0; i < addrList->getAddressCount(); ++i) {
                    auto addr = addrList->getAddressAt(i);
                    auto mbox = vmime::dynamicCast<const vmime::mailbox>(addr);
                    if (mbox) {
                        toList.append(QString::fromStdString(
                            mbox->getEmail().toString()));
                    }
                }
                emailMsg->setTo(toList.join(", "));
            }
        }

        // 日期：headerFieldValue → dynamicCast<const vmime::datetime>
        if (header->hasField(vmime::fields::DATE)) {
            auto field = header->Date();
            auto fieldValue = field->getValue();
            auto dt = vmime::dynamicCast<const vmime::datetime>(fieldValue);
            if (dt) {
                QDateTime qdt;
                qdt.setDate(QDate(dt->getYear(), dt->getMonth(), dt->getDay()));
                qdt.setTime(QTime(dt->getHour(), dt->getMinute(), dt->getSecond()));
                emailMsg->setDate(qdt);
            }
        }

        messages.append(emailMsg);
    }

    folder->close(false);
    emit messageListReady(messages);
}
