#include "imageballoon.h"
#define MESSAGE_SEP "$Nickname$"
#define IMAGE_SEP "$Image$"
#include <QDebug>

ImageBalloon::ImageBalloon(QWidget *parent,
                           QByteArray ba,
                           QString nickname, QDateTime date, MessageStyle ms) : QWidget(parent)
{
    qDebug()<<ba;
    int inx = ba.indexOf(IMAGE_SEP);
    ba.remove(inx, QString(IMAGE_SEP).size());
    inx = ba.indexOf(IMAGE_SEP);
    ba.remove(inx, QString(IMAGE_SEP).size());

    qDebug()<<ba;
    im = new QImage;
    im->loadFromData(ba);
    switch (ms) {
    case RECEIVED:
        setReceivedStyle(nickname, date, im);
        break;
    case SENDED:
        setSendedStyle(nickname, date, im);
        break;
        //setSendedStyle(nickname, im);

    }
}

void ImageBalloon::setReceivedStyle(QString nickname, QDateTime date, QImage *im)
{
    setMaximumSize(640,640);
    double imH = im->height();
    double imW = im->width();
    double ratio = 1.0;
    //qDebug()<<maximumHeight();
    if(imH > maximumHeight()){
        ratio = maximumHeight() / imH;
        if(imW * ratio > maximumWidth()){
            ratio = maximumWidth() / imW;
        }
    }
    imW = ratio * imW;
    imH = ratio * imH;
    //qDebug()<<imW << imH;
    QImage imResize = im->scaled(imW, imH,
                                 Qt::KeepAspectRatio,
                                 Qt::SmoothTransformation);
    if(!imResize.isNull()){
        delete im;
        im = nullptr;
    }
    QVBoxLayout* vboxlayout = new QVBoxLayout(this);
    setLayout(vboxlayout);

    QLabel* nicknameLabel = new QLabel(this);
    if(nickname == ""){
        nickname = "Безымянный пользователь";
    }
    nicknameLabel->setText("От <b>" + nickname + "</b> " + date.toString("dd-MM-yyyy HH:mm:ss"));
    vboxlayout->addWidget(nicknameLabel);
    //QImage test = im->scaled()
    QLabel* pixmapLabel = new QLabel(this);
    pixmapLabel->setPixmap(QPixmap::fromImage(imResize));
    vboxlayout->addWidget(pixmapLabel);

    setMaximumSize(imW + 30, imH + 30 + nicknameLabel->height());
    setMinimumSize(imW + 30, imH + 30 + nicknameLabel->height());

    setStyleSheet("border: 1px solid; "
                  "border-radius:10px;"
                  " background-color:rgb(170, 255, 255);"
                  "border-color:rgb(0, 0, 127);"
                  "margin: 10px;"
                  "padding: 15px;"
                  "font-size: 18px;");

    //setPixmap(QPixmap::fromImage(imResize));
}

void ImageBalloon::setSendedStyle(QString nickname, QDateTime date, QImage *im)
{
    setMaximumSize(640,640);
    double imH = im->height();
    double imW = im->width();
    double ratio = 1.0;
    //qDebug()<<maximumHeight();
    if(imH > maximumHeight()){
        ratio = maximumHeight() / imH;
        if(imW * ratio > maximumWidth()){
            ratio = maximumWidth() / imW;
        }
    }
    imW = ratio * imW;
    imH = ratio * imH;
    //qDebug()<<imW << imH;
    QImage imResize = im->scaled(imW, imH,
                                 Qt::KeepAspectRatio,
                                 Qt::SmoothTransformation);
    if(!imResize.isNull()){
        delete im;
        im = nullptr;
    }
    QVBoxLayout* vboxlayout = new QVBoxLayout(this);
    setLayout(vboxlayout);

    QLabel* nicknameLabel = new QLabel(this);
    if(nickname == ""){
        nickname = "Безымянный пользователь";
    }
    nicknameLabel->setText("От <b>" + nickname + "</b> " + date.toString("dd-MM-yyyy HH:mm:ss"));
    vboxlayout->addWidget(nicknameLabel);
    //QImage test = im->scaled()
    QLabel* pixmapLabel = new QLabel(this);
    pixmapLabel->setPixmap(QPixmap::fromImage(imResize));
    vboxlayout->addWidget(pixmapLabel);

    setMaximumSize(imW + 30, imH + 30 + nicknameLabel->height());
    setMinimumSize(imW + 30, imH + 30 + nicknameLabel->height());

    setStyleSheet("border: 1px solid; "
                  "border-radius:10px;"
                  " background-color:rgb(222, 222, 222);"
                  "border-color:rgb(100, 100, 100);"
                  "margin: 10px;"
                  "padding: 10px;"
                  "font-size: 18px;");
}
