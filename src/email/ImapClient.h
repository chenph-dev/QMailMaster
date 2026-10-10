// IMAP 客户端 — 封装 VMime 的 IMAP 操作，提供连接、获取文件夹列表、拉取邮件列表等功能
// 所有网络操作均为同步阻塞调用（后续可移至线程），通过信号将结果异步传递给 QML

#ifndef IMAPCLIENT_H
#define IMAPCLIENT_H

#include <QObject>
#include <QString>
#include <QList>
#include <memory>

#include "EmailAccount.h"
#include "EmailMessage.h"

// VMime 前置声明，避免在头文件中引入完整 VMime 头文件
namespace vmime {
    class propertySet;
    namespace net {
        class session;
        class store;
        class folder;
        class message;
    }
    namespace security {
        class authenticator;
    }
}

class ImapClient : public QObject
{
    Q_OBJECT
    Q_PROPERTY(EmailAccount* account READ account WRITE setAccount NOTIFY accountChanged)
    Q_PROPERTY(bool connected READ isConnected NOTIFY connectedChanged)

public:
    explicit ImapClient(QObject *parent = nullptr);
    ~ImapClient();

    EmailAccount* account() const;
    void setAccount(EmailAccount *account);

    bool isConnected() const;

    // 以下方法均为 Q_INVOKABLE，可在 QML 中直接调用
    Q_INVOKABLE void connectToServer();
    Q_INVOKABLE void disconnectFromServer();
    Q_INVOKABLE void fetchFolderList();
    Q_INVOKABLE void fetchMessages(const QString &folderPath);
    Q_INVOKABLE void fetchMessageContent(int uid);

signals:
    void accountChanged();
    void connectedChanged();
    void connectionError(const QString &error);
    void folderListReady(const QStringList &folders);       // 文件夹列表获取完成
    void messageListReady(const QList<EmailMessage*> &messages); // 邮件列表拉取完成
    void messageContentReady(int uid, const QString &bodyPlain, const QString &bodyHtml); // 邮件正文获取完成
    void errorOccurred(const QString &error);

private:
    // 内部实现：实际执行 VMime 网络操作
    void doConnect();
    void doDisconnect();
    void doFetchFolders();
    void doFetchMessages(const QString &folderPath);

    EmailAccount *m_account = nullptr;
    bool m_connected = false;

    std::shared_ptr<vmime::net::session> m_session;   // VMime 会话
    std::shared_ptr<vmime::net::store> m_store;       // IMAP store 连接实例
};

#endif // IMAPCLIENT_H
