#include "messageballoon.h"

MessageBalloon::MessageBalloon(QWidget *parent, QString text,
                               QString name, QDateTime date, MessageStyle ms) : QTextEdit(parent)
{
    switch (ms)
    {
    case RECEIVED:
        setReceivedStyle(name, text, date);
        break;
    case SENDED:
        setSendedStyle(name, text, date);
        break;
    }

}

void MessageBalloon::setReceivedStyle(QString name, QString text, QDateTime date)
{
    setStyleSheet("border: 1px solid; "
                  "border-radius:10px;"
                  " background-color:rgb(170, 255, 255);"
                  "border-color:rgb(0, 0, 127);"
                  "margin: 10px;"
                  "padding: 10px;"
                  "font-size: 18px;");
    setMaximumSize(650,100 * ((text.size() / 86)+1));
    setMinimumSize(650,100);
    setTextInteractionFlags(Qt::TextSelectableByMouse|Qt::LinksAccessibleByMouse);
    if(name == ""){
        name = "Безымянный пользователь";
    }
    setText("От <b>" + name + "</b> " + date.toString("dd-MM-yyyy HH:mm:ss") + "<br>" + text);
}

void MessageBalloon::setSendedStyle(QString name, QString text, QDateTime date)
{
    setStyleSheet("border: 1px solid; "
                  "border-radius:10px;"
                  " background-color:rgb(222, 222, 222);"
                  "border-color:rgb(100, 100, 100);"
                  "margin: 10px;"
                  "padding: 10px;"
                  "font-size: 18px;");
    setMaximumSize(650,100 * ((text.size() / 64)+1));
    setMinimumSize(650,100);
    setTextInteractionFlags(Qt::TextSelectableByMouse|Qt::LinksAccessibleByMouse);
    if(name == ""){
        name = "Безымянный пользователь";
    }
    setText("От <b>" + name + "</b> " + date.toString("dd-MM-yyyy HH:mm:ss") + "<br>" + text);
}

