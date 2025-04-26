#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextDocument>
#include <QTcpSocket>
#include <QNetworkDatagram>
#include "emojiwindow.h"
#include "connectionwindow.h"
#include "messageballoon.h"
#include <QtMultimedia>
#include <QFileDialog>
#include <QDataStream>
#include "volumedialog.h"
#include "imageballoon.h"
#include "clientswindow.h"



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct Protocol{
    int type = 1;
    QString name = "Безымянный пользователь";
    QDateTime date;
    QByteArray data = 0;
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void renderMessage(QString message, QString nickname, QDateTime date, MessageStyle type = RECEIVED);

    void renderImage(QString nickname, QByteArray im, QDateTime date, MessageStyle type = RECEIVED);

    void moveSlider();

    void setNotificVolume(int newVolume);

    void sendImage(QByteArray ba);

private slots:
    void on_emojiButton_clicked();

    void on_SetConnectionAction_triggered();

    void bindSocket();

    void on_sendButton_clicked();

    void socketReadyRead();

    void ReceiveParams(QString rec_IP, QString rec_LocalPort,
                       QString rec_ForeignPort, QString rec_Nickname);

    void on_VolumeAction_triggered();


    void on_photoButton_clicked();

    void on_ClientsAction_triggered();

private:
    Ui::MainWindow *ui;

    ConnectionWindow* cw = nullptr;

    bool connected = false;

    EmojiWindow* EmojiTableWindow = nullptr;

    QHostAddress loc = QHostAddress::AnyIPv4;

    QString LocalIP = loc.toString();

    QString nickname, LocalPort, IP, ForeignPort = "";
    QTcpSocket *socket = nullptr;
    int volume = 50;

    QMediaPlayer *player;

    QByteArray BinarySerialize(const Protocol &segment);
    Protocol BinaryDeserialize(const QByteArray &buffer);
    Protocol PackSegment(int type, QString name, QDateTime date, QByteArray data);

    QStringList clients_ip;


signals:
    void sendConnectionStatus(bool connectionStatus);

};
#endif // MAINWINDOW_H
