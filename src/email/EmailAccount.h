// 邮件账户模型 — 存储 IMAP/SMTP 服务器配置及连接状态，通过 Q_PROPERTY 暴露给 QML

#ifndef EMAILACCOUNT_H
#define EMAILACCOUNT_H

#include <QObject>
#include <QString>

class EmailAccount : public QObject
{
    Q_OBJECT

    // 基本信息
    Q_PROPERTY(QString accountId READ accountId WRITE setAccountId NOTIFY accountIdChanged)
    Q_PROPERTY(QString displayName READ displayName WRITE setDisplayName NOTIFY displayNameChanged)
    Q_PROPERTY(QString emailAddress READ emailAddress WRITE setEmailAddress NOTIFY emailAddressChanged)

    // IMAP 收件配置
    Q_PROPERTY(QString imapServer READ imapServer WRITE setImapServer NOTIFY imapServerChanged)
    Q_PROPERTY(int imapPort READ imapPort WRITE setImapPort NOTIFY imapPortChanged)
    Q_PROPERTY(bool imapSsl READ imapSsl WRITE setImapSsl NOTIFY imapSslChanged)

    // SMTP 发件配置
    Q_PROPERTY(QString smtpServer READ smtpServer WRITE setSmtpServer NOTIFY smtpServerChanged)
    Q_PROPERTY(int smtpPort READ smtpPort WRITE setSmtpPort NOTIFY smtpPortChanged)
    Q_PROPERTY(bool smtpTls READ smtpTls WRITE setSmtpTls NOTIFY smtpTlsChanged)

    // 认证凭据
    Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY usernameChanged)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)

    // 连接状态
    Q_PROPERTY(ConnectionState connectionState READ connectionState NOTIFY connectionStateChanged)

public:
    // 账户连接状态枚举，可在 QML 中直接使用
    enum ConnectionState {
        Disconnected,   // 未连接
        Connecting,     // 正在连接
        Connected,      // 已连接
        Error           // 连接出错
    };
    Q_ENUM(ConnectionState)

    explicit EmailAccount(QObject *parent = nullptr);

    QString accountId() const;
    void setAccountId(const QString &id);

    QString displayName() const;
    void setDisplayName(const QString &name);

    QString emailAddress() const;
    void setEmailAddress(const QString &email);

    QString imapServer() const;
    void setImapServer(const QString &server);

    int imapPort() const;
    void setImapPort(int port);

    bool imapSsl() const;
    void setImapSsl(bool ssl);

    QString smtpServer() const;
    void setSmtpServer(const QString &server);

    int smtpPort() const;
    void setSmtpPort(int port);

    bool smtpTls() const;
    void setSmtpTls(bool tls);

    QString username() const;
    void setUsername(const QString &username);

    QString password() const;
    void setPassword(const QString &password);

    ConnectionState connectionState() const;
    void setConnectionState(ConnectionState state);

signals:
    void accountIdChanged();
    void displayNameChanged();
    void emailAddressChanged();
    void imapServerChanged();
    void imapPortChanged();
    void imapSslChanged();
    void smtpServerChanged();
    void smtpPortChanged();
    void smtpTlsChanged();
    void usernameChanged();
    void passwordChanged();
    void connectionStateChanged();

private:
    QString m_accountId;
    QString m_displayName;
    QString m_emailAddress;

    QString m_imapServer;
    int m_imapPort = 993;    // IMAP SSL 默认端口
    bool m_imapSsl = true;

    QString m_smtpServer;
    int m_smtpPort = 587;    // SMTP STARTTLS 默认端口
    bool m_smtpTls = true;

    QString m_username;
    QString m_password;

    ConnectionState m_connectionState = Disconnected;
};

#endif // EMAILACCOUNT_H
