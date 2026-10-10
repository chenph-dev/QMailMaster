// 邮件管理器实现 — 管理账户列表，协调 IMAP/SMTP 客户端，转发信号给 QML

#include "EmailManager.h"

EmailManager::EmailManager(QObject *parent)
    : QObject(parent)
    , m_imapClient(new ImapClient(this))
    , m_smtpClient(new SmtpClient(this))
{
    // 转发 IMAP 客户端信号
    connect(m_imapClient, &ImapClient::folderListReady,
            this, &EmailManager::onFolderListReady);
    connect(m_imapClient, &ImapClient::messageListReady,
            this, &EmailManager::onMessageListReady);
    connect(m_imapClient, &ImapClient::errorOccurred,
            this, &EmailManager::onError);

    // 转发 SMTP 客户端信号
    connect(m_smtpClient, &SmtpClient::messageSent,
            this, &EmailManager::onMessageSent);
    connect(m_smtpClient, &SmtpClient::errorOccurred,
            this, &EmailManager::onError);
}

EmailManager::~EmailManager()
{
    qDeleteAll(m_currentMessages);
}

QQmlListProperty<EmailAccount> EmailManager::accountsProperty()
{
    return QQmlListProperty<EmailAccount>(this, nullptr,
        &EmailManager::accountCount, &EmailManager::accountAt);
}

qsizetype EmailManager::accountCount(QQmlListProperty<EmailAccount> *prop)
{
    auto *mgr = static_cast<EmailManager*>(prop->object);
    return mgr->m_accounts.size();
}

EmailAccount* EmailManager::accountAt(QQmlListProperty<EmailAccount> *prop, qsizetype index)
{
    auto *mgr = static_cast<EmailManager*>(prop->object);
    return mgr->m_accounts.at(index);
}

ImapClient* EmailManager::currentImapClient() const
{
    return m_imapClient;
}

SmtpClient* EmailManager::currentSmtpClient() const
{
    return m_smtpClient;
}

EmailAccount* EmailManager::currentAccount() const
{
    return m_currentAccount;
}

// 切换当前账户：先断开旧账户，再将新账户同步设置到 IMAP/SMTP 客户端
void EmailManager::setCurrentAccount(EmailAccount *account)
{
    if (m_currentAccount == account)
        return;

    if (m_currentAccount)
        disconnectCurrentAccount();

    m_currentAccount = account;

    if (m_currentAccount) {
        m_imapClient->setAccount(m_currentAccount);
        m_smtpClient->setAccount(m_currentAccount);
    }

    emit currentAccountChanged();
}

QList<EmailMessage*> EmailManager::currentMessages() const
{
    return m_currentMessages;
}

// 创建新账户并自动设为当前账户（如果是第一个账户）
EmailAccount* EmailManager::createAccount()
{
    auto *account = new EmailAccount(this);
    account->setAccountId(QString("acct-%1").arg(m_accounts.size() + 1));
    m_accounts.append(account);
    emit accountsChanged();

    if (!m_currentAccount)
        setCurrentAccount(account);

    return account;
}

// 删除账户：如果删除的是当前账户，自动切换到第一个剩余账户
void EmailManager::removeAccount(EmailAccount *account)
{
    if (!m_accounts.removeOne(account))
        return;

    if (m_currentAccount == account) {
        disconnectCurrentAccount();
        m_currentAccount = m_accounts.isEmpty() ? nullptr : m_accounts.first();
        if (m_currentAccount) {
            m_imapClient->setAccount(m_currentAccount);
            m_smtpClient->setAccount(m_currentAccount);
        }
        emit currentAccountChanged();
    }

    account->deleteLater();
    emit accountsChanged();
}

void EmailManager::connectCurrentAccount()
{
    if (!m_currentAccount)
        return;
    m_imapClient->connectToServer();
}

void EmailManager::disconnectCurrentAccount()
{
    m_imapClient->disconnectFromServer();
    m_smtpClient->disconnectFromServer();
}

void EmailManager::fetchFolders()
{
    m_imapClient->fetchFolderList();
}

void EmailManager::fetchMessages(const QString &folderPath)
{
    m_imapClient->fetchMessages(folderPath);
}

// 发送邮件：如果 SMTP 未连接则先自动连接
void EmailManager::sendMessage(const QString &to, const QString &subject,
                                const QString &bodyPlain, const QString &bodyHtml)
{
    if (!m_smtpClient->isConnected())
        m_smtpClient->connectToServer();
    m_smtpClient->sendMessage(to, subject, bodyPlain, bodyHtml);
}

void EmailManager::onFolderListReady(const QStringList &folders)
{
    emit folderListReady(folders);
}

// 收到新邮件列表：替换旧的邮件列表并通知 QML 刷新
void EmailManager::onMessageListReady(const QList<EmailMessage*> &messages)
{
    qDeleteAll(m_currentMessages);
    m_currentMessages = messages;
    emit currentMessagesChanged();
    emit messageListReady(messages);
}

void EmailManager::onMessageSent()
{
    emit messageSent();
}

void EmailManager::onError(const QString &error)
{
    emit errorOccurred(error);
}
