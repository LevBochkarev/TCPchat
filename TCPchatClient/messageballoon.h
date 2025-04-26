#ifndef MESSAGEBALLOON_H
#define MESSAGEBALLOON_H

#include <QTextEdit>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QDateTime>

enum MessageStyle{
    RECEIVED,
    SENDED
};

class MessageBalloon : public QTextEdit
{
    Q_OBJECT
public:
    explicit MessageBalloon(QWidget *parent = nullptr, QString text = "(тут пусто)",
                            QString name = "Безымянный пользователь", QDateTime date = QDateTime(QDate(2024,1,1)), MessageStyle ms = RECEIVED);

private:
    void setReceivedStyle(QString name, QString text, QDateTime date);
    void setSendedStyle(QString name, QString text, QDateTime date);


signals:

};

#endif // MESSAGEBALLOON_H
