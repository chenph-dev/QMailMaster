// 邮件管理器 — QML 侧的统一入口，管理账户列表、协调 IMAP/SMTP 客户端
// 在 QML 中注册为单例或上下文属性后，可直接调用其 Q_INVOKABLE 方法

#ifndef EMAILMANAGER_H
#define EMAILMANAGER_H

#include <QObject>
#include <QQmlListProperty>
#include <QList>

#include "EmailAccount.h"
#include "EmailMessage.h"
#include "ImapClient.h"
#include "SmtpClient.h"

class EmailManager : public QObject
{
    Q_OBJECT

    // 账户列表（只读，供 QML Repeater/ListView 使用）
    Q_PROPERTY(QQmlListProperty<EmailAccount> accounts READ accountsProperty NOTIFY accountsChanged)

    // 当前账户的 IMAP/SMTP 客户端实例
    Q_PROPERTY(ImapClient* currentImapClient READ currentImapClient NOTIFY currentAccountChanged)
    Q_PROPERTY(SmtpClient* currentSmtpClient READ currentSmtpClient NOTIFY currentAccountChanged)

    // 当前选中的账户
    Q_PROPERTY(EmailAccount* currentAccount READ currentAccount WRITE setCurrentAccount NOTIFY currentAccountChanged)

    // 当前文件夹的邮件列表
    Q_PROPERTY(QList<EmailMessage*> currentMessages READ currentMessages NOTIFY currentMessagesChanged)

public:
    explicit EmailManager(QObject *parent = nullptr);
    ~EmailManager();

    QQmlListProperty<EmailAccount> accountsProperty();
    QList<EmailMessage*> currentMessages() const;

    ImapClient* currentImapClient() const;
    SmtpClient* currentSmtpClient() const;
    EmailAccount* currentAccount() const;
    void setCurrentAccount(EmailAccount *account);

    // 以下方法均为 Q_INVOKABLE，可在 QML 中直接调用
    Q_INVOKABLE EmailAccount* createAccount();
    Q_INVOKABLE void removeAccount(EmailAccount *account);
    Q_INVOKABLE void connectCurrentAccount();
    Q_INVOKABLE void disconnectCurrentAccount();
    Q_INVOKABLE void fetchFolders();
    Q_INVOKABLE void fetchMessages(const QString &folderPath);
    Q_INVOKABLE void sendMessage(const QString &to, const QString &subject,
                                 const QString &bodyPlain, const QString &bodyHtml);

signals:
    void accountsChanged();
    void currentAccountChanged();
    void currentMessagesChanged();
    void folderListReady(const QStringList &folders);
    void messageListReady(const QList<EmailMessage*> &messages);
    void messageSent();
    void errorOccurred(const QString &error);

private slots:
    // 转发 IMAP/SMTP 客户端的信号
    void onFolderListReady(const QStringList &folders);
    void onMessageListReady(const QList<EmailMessage*> &messages);
    void onMessageSent();
    void onError(const QString &error);

private:
    QList<EmailAccount*> m_accounts;
    QList<EmailMessage*> m_currentMessages;
    EmailAccount *m_currentAccount = nullptr;

    ImapClient *m_imapClient = nullptr;
    SmtpClient *m_smtpClient = nullptr;

    // QQmlListProperty 回调函数（Qt6 要求使用 qsizetype）
    static qsizetype accountCount(QQmlListProperty<EmailAccount> *prop);
    static EmailAccount* accountAt(QQmlListProperty<EmailAccount> *prop, qsizetype index);
};

#endif // EMAILMANAGER_H
