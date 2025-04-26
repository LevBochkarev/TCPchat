#ifndef IMAGEBALLOON_H
#define IMAGEBALLOON_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QDateTime>
#include "messageballoon.h"

class ImageBalloon : public QWidget
{
    Q_OBJECT
public:
    explicit ImageBalloon(QWidget *parent = nullptr,
                          QByteArray ba = nullptr,
                          QString nickname = "Безымянный пользователь",
                          QDateTime date = QDateTime(QDate(2024,1,1)),
                          MessageStyle ms = RECEIVED);

private:
    void setReceivedStyle(QString nickname, QDateTime date, QImage* im);
    void setSendedStyle(QString nickname, QDateTime date, QImage* im);

    QImage* im;

signals:

};

#endif // IMAGEBALLOON_H
