// SMTP 客户端实现 — 通过 VMime 库连接 SMTP 服务器并发送邮件
//
// VMime API 要点：
// - session->getTransport("smtps") 获取 SMTP transport
// - transport->setProperty() 设置服务器地址/端口/认证信息
// - 构建邮件：创建 vmime::message，设置 header（From/To/Subject/Date），设置 body
// - 多部分邮件：body 设置为 multipart/alternative，分别添加 text/plain 和 text/html 子部分

#include "SmtpClient.h"

#include <vmime/vmime.hpp>

SmtpClient::SmtpClient(QObject *parent)
    : QObject(parent)
{
}

SmtpClient::~SmtpClient()
{
    doDisconnect();
}

EmailAccount* SmtpClient::account() const
{
    return m_account;
}

void SmtpClient::setAccount(EmailAccount *account)
{
    if (m_account == account)
        return;
    m_account = account;
    emit accountChanged();
}

bool SmtpClient::isConnected() const
{
    return m_connected;
}

void SmtpClient::connectToServer()
{
    if (!m_account) {
        emit errorOccurred(tr("No account configured"));
        return;
    }

    try {
        doConnect();
        m_connected = true;
        emit connectedChanged();
    } catch (const std::exception &e) {
        m_connected = false;
        emit connectionError(QString::fromUtf8(e.what()));
        emit connectedChanged();
    }
}

void SmtpClient::disconnectFromServer()
{
    doDisconnect();
    m_connected = false;
    emit connectedChanged();
}

void SmtpClient::sendMessage(const QString &to, const QString &subject,
                              const QString &bodyPlain, const QString &bodyHtml)
{
    if (!m_connected) {
        emit errorOccurred(tr("Not connected to SMTP server"));
        return;
    }

    try {
        auto msg = buildMessage(to, subject, bodyPlain, bodyHtml);

        // 构造发件人邮箱
        vmime::mailbox expeditor(
            vmime::text(m_account->displayName().toStdString()),
            vmime::emailAddress(m_account->emailAddress().toStdString()));

        // 构造收件人列表
        vmime::mailboxList recipients;
        recipients.appendMailbox(vmime::make_shared<vmime::mailbox>(
            vmime::emailAddress(to.toStdString())));

        m_transport->send(msg, expeditor, recipients);
        emit messageSent();
    } catch (const std::exception &e) {
        emit errorOccurred(QString::fromUtf8(e.what()));
    }
}

// 建立 SMTP 连接：根据 TLS 设置选择 smtps/smtp 协议
void SmtpClient::doConnect()
{
    const std::string proto = m_account->smtpTls() ? "smtps" : "smtp";

    vmime::propertySet props;
    props["transport.protocol"] = proto;

    m_session = vmime::net::session::create(props);
    m_transport = m_session->getTransport(proto);

    // setProperty 会自动添加协议前缀
    m_transport->setProperty("server.address", m_account->smtpServer().toStdString());
    m_transport->setProperty("server.port", m_account->smtpPort());
    m_transport->setProperty("auth.username", m_account->username().toStdString());
    m_transport->setProperty("auth.password", m_account->password().toStdString());

    m_transport->connect();
}

void SmtpClient::doDisconnect()
{
    try {
        if (m_transport && m_transport->isConnected())
            m_transport->disconnect();
    } catch (...) {
    }
    m_transport.reset();
    m_session.reset();
}

// 构建 VMime 邮件对象
// 如果有 HTML 内容，生成 multipart/alternative（纯文本 + HTML 两个子部分）
// 否则仅设置纯文本正文
std::shared_ptr<vmime::message> SmtpClient::buildMessage(
    const QString &to, const QString &subject,
    const QString &bodyPlain, const QString &bodyHtml)
{
    auto msg = vmime::make_shared<vmime::message>();

    auto header = msg->getHeader();

    // 设置发件人（显示名称 + 邮箱地址）
    header->From()->setValue(vmime::mailbox(
        vmime::text(m_account->displayName().toStdString()),
        vmime::emailAddress(m_account->emailAddress().toStdString())));

    // 设置收件人
    auto toList = vmime::make_shared<vmime::addressList>();
    toList->appendAddress(vmime::make_shared<vmime::mailbox>(
        vmime::emailAddress(to.toStdString())));
    header->To()->setValue(toList);

    // 设置主题
    header->Subject()->setValue(vmime::text(subject.toStdString()));

    // 设置发送时间（当前时间）
    const auto now = vmime::datetime::now();
    header->Date()->setValue(now);

    if (!bodyHtml.isEmpty()) {
        // 多部分邮件：multipart/alternative，客户端优先显示 HTML
        auto plainPart = vmime::make_shared<vmime::bodyPart>();
        plainPart->getBody()->setContents(
            vmime::make_shared<vmime::stringContentHandler>(bodyPlain.toStdString()),
            vmime::mediaType("text", "plain"),
            vmime::charset("utf-8"));

        auto htmlPart = vmime::make_shared<vmime::bodyPart>();
        htmlPart->getBody()->setContents(
            vmime::make_shared<vmime::stringContentHandler>(bodyHtml.toStdString()),
            vmime::mediaType("text", "html"),
            vmime::charset("utf-8"));

        msg->getBody()->setContentType(
            vmime::mediaType("multipart", "alternative"));
        msg->getBody()->appendPart(plainPart);
        msg->getBody()->appendPart(htmlPart);
    } else {
        // 纯文本邮件
        msg->getBody()->setContents(
            vmime::make_shared<vmime::stringContentHandler>(bodyPlain.toStdString()),
            vmime::mediaType("text", "plain"),
            vmime::charset("utf-8"));
    }

    return msg;
}
