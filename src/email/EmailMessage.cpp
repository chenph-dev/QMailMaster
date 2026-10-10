// 邮件消息模型实现 — 标准的 Qt 属性模式，每个 setter 在值变化时发射对应信号

#include "EmailMessage.h"

EmailMessage::EmailMessage(QObject *parent)
    : QObject(parent)
{
}

QString EmailMessage::messageId() const { return m_messageId; }
void EmailMessage::setMessageId(const QString &id)
{
    if (m_messageId != id) { m_messageId = id; emit messageIdChanged(); }
}

QString EmailMessage::from() const { return m_from; }
void EmailMessage::setFrom(const QString &from)
{
    if (m_from != from) { m_from = from; emit fromChanged(); }
}

QString EmailMessage::fromName() const { return m_fromName; }
void EmailMessage::setFromName(const QString &name)
{
    if (m_fromName != name) { m_fromName = name; emit fromNameChanged(); }
}

QString EmailMessage::to() const { return m_to; }
void EmailMessage::setTo(const QString &to)
{
    if (m_to != to) { m_to = to; emit toChanged(); }
}

QString EmailMessage::subject() const { return m_subject; }
void EmailMessage::setSubject(const QString &subject)
{
    if (m_subject != subject) { m_subject = subject; emit subjectChanged(); }
}

QString EmailMessage::bodyPlain() const { return m_bodyPlain; }
void EmailMessage::setBodyPlain(const QString &body)
{
    if (m_bodyPlain != body) { m_bodyPlain = body; emit bodyPlainChanged(); }
}

QString EmailMessage::bodyHtml() const { return m_bodyHtml; }
void EmailMessage::setBodyHtml(const QString &body)
{
    if (m_bodyHtml != body) { m_bodyHtml = body; emit bodyHtmlChanged(); }
}

QDateTime EmailMessage::date() const { return m_date; }
void EmailMessage::setDate(const QDateTime &date)
{
    if (m_date != date) { m_date = date; emit dateChanged(); }
}

bool EmailMessage::isRead() const { return m_isRead; }
void EmailMessage::setIsRead(bool read)
{
    if (m_isRead != read) { m_isRead = read; emit isReadChanged(); }
}

bool EmailMessage::isFlagged() const { return m_isFlagged; }
void EmailMessage::setIsFlagged(bool flagged)
{
    if (m_isFlagged != flagged) { m_isFlagged = flagged; emit isFlaggedChanged(); }
}

bool EmailMessage::hasAttachment() const { return m_hasAttachment; }
void EmailMessage::setHasAttachment(bool has)
{
    if (m_hasAttachment != has) { m_hasAttachment = has; emit hasAttachmentChanged(); }
}

int EmailMessage::size() const { return m_size; }
void EmailMessage::setSize(int size)
{
    if (m_size != size) { m_size = size; emit sizeChanged(); }
}

int EmailMessage::uid() const { return m_uid; }
void EmailMessage::setUid(int uid)
{
    if (m_uid != uid) { m_uid = uid; emit uidChanged(); }
}
