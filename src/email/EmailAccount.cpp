// 邮件账户模型实现 — 标准的 Qt 属性模式，每个 setter 在值变化时发射对应信号

#include "EmailAccount.h"

EmailAccount::EmailAccount(QObject *parent)
    : QObject(parent)
{
}

QString EmailAccount::accountId() const { return m_accountId; }
void EmailAccount::setAccountId(const QString &id)
{
    if (m_accountId != id) {
        m_accountId = id;
        emit accountIdChanged();
    }
}

QString EmailAccount::displayName() const { return m_displayName; }
void EmailAccount::setDisplayName(const QString &name)
{
    if (m_displayName != name) {
        m_displayName = name;
        emit displayNameChanged();
    }
}

QString EmailAccount::emailAddress() const { return m_emailAddress; }
void EmailAccount::setEmailAddress(const QString &email)
{
    if (m_emailAddress != email) {
        m_emailAddress = email;
        emit emailAddressChanged();
    }
}

QString EmailAccount::imapServer() const { return m_imapServer; }
void EmailAccount::setImapServer(const QString &server)
{
    if (m_imapServer != server) {
        m_imapServer = server;
        emit imapServerChanged();
    }
}

int EmailAccount::imapPort() const { return m_imapPort; }
void EmailAccount::setImapPort(int port)
{
    if (m_imapPort != port) {
        m_imapPort = port;
        emit imapPortChanged();
    }
}

bool EmailAccount::imapSsl() const { return m_imapSsl; }
void EmailAccount::setImapSsl(bool ssl)
{
    if (m_imapSsl != ssl) {
        m_imapSsl = ssl;
        emit imapSslChanged();
    }
}

QString EmailAccount::smtpServer() const { return m_smtpServer; }
void EmailAccount::setSmtpServer(const QString &server)
{
    if (m_smtpServer != server) {
        m_smtpServer = server;
        emit smtpServerChanged();
    }
}

int EmailAccount::smtpPort() const { return m_smtpPort; }
void EmailAccount::setSmtpPort(int port)
{
    if (m_smtpPort != port) {
        m_smtpPort = port;
        emit smtpPortChanged();
    }
}

bool EmailAccount::smtpTls() const { return m_smtpTls; }
void EmailAccount::setSmtpTls(bool tls)
{
    if (m_smtpTls != tls) {
        m_smtpTls = tls;
        emit smtpTlsChanged();
    }
}

QString EmailAccount::username() const { return m_username; }
void EmailAccount::setUsername(const QString &username)
{
    if (m_username != username) {
        m_username = username;
        emit usernameChanged();
    }
}

QString EmailAccount::password() const { return m_password; }
void EmailAccount::setPassword(const QString &password)
{
    if (m_password != password) {
        m_password = password;
        emit passwordChanged();
    }
}

EmailAccount::ConnectionState EmailAccount::connectionState() const { return m_connectionState; }
void EmailAccount::setConnectionState(ConnectionState state)
{
    if (m_connectionState != state) {
        m_connectionState = state;
        emit connectionStateChanged();
    }
}
