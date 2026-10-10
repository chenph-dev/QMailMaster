// 邮件消息模型 — 表示一封邮件的元数据和内容，通过 Q_PROPERTY 暴露给 QML 用于列表展示

#ifndef EMAILMESSAGE_H
#define EMAILMESSAGE_H

#include <QObject>
#include <QString>
#include <QDateTime>

class EmailMessage : public QObject
{
    Q_OBJECT

    // 邮件标识
    Q_PROPERTY(QString messageId READ messageId WRITE setMessageId NOTIFY messageIdChanged)
    Q_PROPERTY(int uid READ uid WRITE setUid NOTIFY uidChanged)

    // 发件人信息
    Q_PROPERTY(QString from READ from WRITE setFrom NOTIFY fromChanged)
    Q_PROPERTY(QString fromName READ fromName WRITE setFromName NOTIFY fromNameChanged)

    // 收件人信息
    Q_PROPERTY(QString to READ to WRITE setTo NOTIFY toChanged)

    // 邮件内容
    Q_PROPERTY(QString subject READ subject WRITE setSubject NOTIFY subjectChanged)
    Q_PROPERTY(QString bodyPlain READ bodyPlain WRITE setBodyPlain NOTIFY bodyPlainChanged)
    Q_PROPERTY(QString bodyHtml READ bodyHtml WRITE setBodyHtml NOTIFY bodyHtmlChanged)

    // 时间戳
    Q_PROPERTY(QDateTime date READ date WRITE setDate NOTIFY dateChanged)

    // 状态标记
    Q_PROPERTY(bool isRead READ isRead WRITE setIsRead NOTIFY isReadChanged)
    Q_PROPERTY(bool isFlagged READ isFlagged WRITE setIsFlagged NOTIFY isFlaggedChanged)
    Q_PROPERTY(bool hasAttachment READ hasAttachment WRITE setHasAttachment NOTIFY hasAttachmentChanged)

    // 邮件大小（字节）
    Q_PROPERTY(int size READ size WRITE setSize NOTIFY sizeChanged)

public:
    explicit EmailMessage(QObject *parent = nullptr);

    QString messageId() const;
    void setMessageId(const QString &id);

    QString from() const;
    void setFrom(const QString &from);

    QString fromName() const;
    void setFromName(const QString &name);

    QString to() const;
    void setTo(const QString &to);

    QString subject() const;
    void setSubject(const QString &subject);

    QString bodyPlain() const;
    void setBodyPlain(const QString &body);

    QString bodyHtml() const;
    void setBodyHtml(const QString &body);

    QDateTime date() const;
    void setDate(const QDateTime &date);

    bool isRead() const;
    void setIsRead(bool read);

    bool isFlagged() const;
    void setIsFlagged(bool flagged);

    bool hasAttachment() const;
    void setHasAttachment(bool has);

    int size() const;
    void setSize(int size);

    int uid() const;
    void setUid(int uid);

signals:
    void messageIdChanged();
    void fromChanged();
    void fromNameChanged();
    void toChanged();
    void subjectChanged();
    void bodyPlainChanged();
    void bodyHtmlChanged();
    void dateChanged();
    void isReadChanged();
    void isFlaggedChanged();
    void hasAttachmentChanged();
    void sizeChanged();
    void uidChanged();

private:
    QString m_messageId;
    QString m_from;
    QString m_fromName;
    QString m_to;
    QString m_subject;
    QString m_bodyPlain;
    QString m_bodyHtml;
    QDateTime m_date;
    bool m_isRead = false;
    bool m_isFlagged = false;
    bool m_hasAttachment = false;
    int m_size = 0;
    int m_uid = 0;
};

#endif // EMAILMESSAGE_H
