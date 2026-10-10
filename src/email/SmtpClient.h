// SMTP 客户端 — 封装 VMime 的 SMTP 发送操作，支持纯文本和 HTML 多部分邮件构建

#ifndef SMTPCLIENT_H
#define SMTPCLIENT_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <memory>

#include "EmailAccount.h"

// VMime 前置声明
namespace vmime {
    class message;
    namespace net {
        class session;
        class transport;
    }
}

class SmtpClient : public QObject
{
    Q_OBJECT
    Q_PROPERTY(EmailAccount* account READ account WRITE setAccount NOTIFY accountChanged)
    Q_PROPERTY(bool connected READ isConnected NOTIFY connectedChanged)

public:
    explicit SmtpClient(QObject *parent = nullptr);
    ~SmtpClient();

    EmailAccount* account() const;
    void setAccount(EmailAccount *account);

    bool isConnected() const;

    // 以下方法均为 Q_INVOKABLE，可在 QML 中直接调用
    Q_INVOKABLE void connectToServer();
    Q_INVOKABLE void disconnectFromServer();
    Q_INVOKABLE void sendMessage(const QString &to, const QString &subject,
                                 const QString &bodyPlain, const QString &bodyHtml);

signals:
    void accountChanged();
    void connectedChanged();
    void connectionError(const QString &error);
    void messageSent();              // 邮件发送成功
    void errorOccurred(const QString &error);

private:
    // 内部实现：实际执行 VMime 网络操作
    void doConnect();
    void doDisconnect();

    // 构建 VMime 邮件对象：有 HTML 内容时生成 multipart/alternative，否则仅纯文本
    std::shared_ptr<vmime::message> buildMessage(const QString &to, const QString &subject,
                                                  const QString &bodyPlain, const QString &bodyHtml);

    EmailAccount *m_account = nullptr;
    bool m_connected = false;

    std::shared_ptr<vmime::net::session> m_session;      // VMime 会话
    std::shared_ptr<vmime::net::transport> m_transport;  // SMTP transport 连接实例
};

#endif // SMTPCLIENT_H
