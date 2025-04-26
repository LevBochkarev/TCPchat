#ifndef SERVERHANDLER_H
#define SERVERHANDLER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QDateTime>

struct Protocol{
    int type = 1;
    QString name = "Безымянный пользователь";
    QDateTime date;
    QByteArray data = 0;
};
//struct Protocol_header{
//    int type = 1;
//    QString name = "Безымянный пользователь";
//    QDateTime date;
//};
//struct Protocol1{
//    int type = 1;
//    QString name = "Безымянный пользователь";
//    QDateTime date;
//    QString message = "";
//};
//struct Protocol2{
//    int type = 1;
//    QString name = "Безымянный пользователь";
//    QDateTime date;
//    QImage image;
//};
//struct Protocol3{
//    int type = 1;
//    QString name = "Безымянный пользователь";
//    QDateTime date;
//    QStringList clients;
//};

class ServerHandler : public QObject
{
    Q_OBJECT
public:
    explicit ServerHandler(QObject *parent = 0)  : QObject(parent){};
    ~ServerHandler(){};

private:
    QTcpServer *server;
    QList<QTcpSocket*> clients;
    QStringList clients_ip;

    QByteArray BinarySerialize(const Protocol &segment);
    Protocol BinaryDeserialize(const QByteArray &buffer);
    Protocol PackSegment(int type, QString name, QDateTime date, QByteArray data);


public slots:
    void run();
    void newClientConnection();
    void clientReadyRead();
    void clientDisconnected();

signals:
    void finished();
};

#endif // SERVERHANDLER_H
