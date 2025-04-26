#include "serverhandler.h"
#include <QTextCodec>
#include <QDataStream>
#include <QNetworkInterface>

QDataStream& operator<< (QDataStream &stream, const Protocol &segment)
{
    return stream << segment.type
                  << segment.name
                  << segment.date
                  << segment.data;
}

QDataStream& operator>> (QDataStream &stream, Protocol &segment)
{
    return stream >> segment.type
                  >> segment.name
                  >> segment.date
                  >> segment.data;
}

QByteArray ServerHandler::BinarySerialize(const Protocol &segment)
{
    QByteArray buffer;
    QDataStream stream(&buffer, QIODevice::WriteOnly);
    stream << segment;
    return buffer;
}

Protocol ServerHandler::BinaryDeserialize(const QByteArray &buffer)
{
    Protocol segment;
    QDataStream stream(buffer);
    stream >> segment;
    return segment;
}

Protocol ServerHandler::PackSegment(int type, QString name, QDateTime date, QByteArray data)
{
    Protocol segment;
    segment.type = type;
    segment.name = name;
    segment.date = date;
    segment.data = data;
    return segment;
}

void ServerHandler::run(){ //Запуск сервера
    QTextStream outStream(stdout); //Поток вывода в терминале
    outStream.setCodec(QTextCodec::codecForName("cp866")); //Устанавливаем кодек, чтобы отображалась кириллица
    QTextStream inStream(stdin); //Поток ввода из терминала


    server = new QTcpServer(this); //Создаем сервер
    if(server){ //Если создался
        outStream << QString("Сервер создан\n") << flush; //Пишем в терминал
    }

    connect(server, &QTcpServer::newConnection, this, &ServerHandler::newClientConnection); // Подключаем сигнал
    //О новом подключении к слоту обработки нового клиента

    bool ok = false; //Переменная для проверки правильности ввода порта
    ushort port; //Переменная для порта
    QString stringPort; //Стринг для порта
    while(!ok){ //Пока не введен валидный порт
        outStream << QString("Введите порт или впишите \"auto\" : \n") << flush; //Выводим в терминал
        inStream >> stringPort; //Считываем в стринг из терминала
        if(stringPort == "auto"){ //Если юзер вписал авто
            port = 0; //Для автоматической генерации порта (в документации QTCPServer прописано)
            ok = true; //Ставим флаг в истинное значение
            continue; //Переходим к новой итерации цикла
        }
        port = stringPort.toUShort(&ok); //Переводим порт из стринга в юшорт
        if(!ok or stringPort.toInt() >= 65535){ //Если неверный порт или превышает максимальный порт
            outStream << QString("Неверный порт\n") << flush; //Выводим в терминал
        }
    }
    server->listen(QHostAddress::Any, port); //Слушаем все адреса

    outStream << "\n------------------------------------------\n" << flush; //Разделительная строка в терминале
    outStream << QString("IP-адрес сервера : \n") << flush; //Вывод в терминал
    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost); //Записываем IP 127.0.0.1 для избегания в будущем
    for (const QHostAddress &address: QNetworkInterface::allAddresses()) { //Перебираем все IP
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost) //Если IPv4 и не 127.0.0.1
             qDebug() << address.toString(); //Выводим адрес в терминал
    }
    outStream << QString("\nПорт сервера : ") << server->serverPort() << flush; //Выводим порт в терминал
    outStream << "\n------------------------------------------\n" << flush; //Разделительная строка в терминале
}

void ServerHandler::newClientConnection() //Обработка подключения нового клиента
{
    QTextStream outStream(stdout); //Поток вывода в терминале
    outStream.setCodec(QTextCodec::codecForName("cp866")); //Устанавливаем кодек, чтобы отображалась кириллица
    // Пока есть ожидающие клиентские поключения
    while(server->hasPendingConnections()) {
        // Получаем и настраиваем клиентское подключение
        QTcpSocket *client = server->nextPendingConnection();
        connect(client, &QTcpSocket::readyRead, this, &ServerHandler::clientReadyRead);
        connect(client, &QTcpSocket::disconnected, this, &ServerHandler::clientDisconnected);

        // Сохраняем сокет подключенного клиента
        clients << client;

        // Отображаем IP подключенного клиента
        outStream << QString("Подключен клиент. IP : "+ client->peerAddress().toString() + "\n") << flush;
        clients_ip.append(client->peerAddress().toString());
        qDebug() << clients_ip;
        QByteArray buffer;
        QByteArray buffer1;
        QDataStream stream(&buffer, QIODevice::WriteOnly);

        stream << clients_ip;
        Protocol clients_info = PackSegment(3, "Server", QDateTime::currentDateTime(), buffer);
        buffer1 = BinarySerialize(clients_info);
        //client->write(buffer1);
        for(QTcpSocket *cli : clients){
            cli->write(buffer1);
        }
    }
}

void ServerHandler::clientReadyRead() //Обработка получения данных сокетом
{
    QTextStream outStream(stdout); //Поток вывода в терминале
    outStream.setCodec(QTextCodec::codecForName("cp866")); //Устанавливаем кодек, чтобы отображалась кириллица
    // Клиентский сокет, который прислал данные
    QTcpSocket *client = (QTcpSocket*)sender();

    /* Поток и данные*/
    QDataStream receiveStream(client);

    /*Транзакция*/
    receiveStream.startTransaction();
    QByteArray buffer;
    Protocol segment;
    receiveStream >> segment;

    if(!receiveStream.commitTransaction()){ //Если транзакция завершилась неуспешно
        outStream << QString("Ожидаем данные \n"); //Выводим в терминал
        receiveStream.rollbackTransaction();
        return; //Ждем новые данные
    }

    //Protocol segment = BinaryDeserialize(buffer);
    qDebug() << segment.type
             << segment.name
             << QString(segment.data)
             << segment.date;

    /****************************************************************/
    /* Вывод в консоль */
    QString ip = client->peerAddress().toString(); // Читаем IP клиента
    if(segment.type == 1 or segment.type == 3){ //Если текст
        // Отображаем сообщение
        QString text = ip + "\n" + QString(segment.data) + "\n"; //Добавляем ip и переносы строк к сообщению
        // Выводим сообщение с подробностями в терминал
        outStream << QString(segment.date.toString(Qt::TextDate) + " | Новое сообщение. Ник: " + segment.name + text) << flush;
    }
    else if(segment.type == 2){
        // Выводим сообщение с подробностями в терминал
        outStream << QString(segment.date.toString(Qt::TextDate) + " |Новое сообщение. Ник: " + segment.name + " Изображение\n") << flush;
    }
    /****************************************************************/
    /* Отправляем сообщение всем клиентам*/
    for(QTcpSocket *cli : clients){ //Перебираем подключенные клиенты
        if(cli != client){
            cli->write(BinarySerialize(segment)); //Отправляем сериализованный массив
        }
            //cli->write(text.toUtf8());
    }
    /* Отправляем сообщение всем клиентам*/
}

void ServerHandler::clientDisconnected() //Если клиент отключился
{
    QTextStream outStream(stdout);
    outStream.setCodec(QTextCodec::codecForName("cp866"));
    // Клиентский сокет, который отключился
    QTcpSocket *client = (QTcpSocket*)sender();

    QString ip = client->peerAddress().toString();
    clients_ip.removeOne(client->peerAddress().toString());
    outStream << QString("Отключен клиент. IP : " + ip + "\n") << flush; //Выводим в консоль

    QByteArray buffer;
    QByteArray buffer1;
    QDataStream stream(&buffer, QIODevice::WriteOnly);

    stream << clients_ip;
    Protocol clients_info = PackSegment(3, "Server", QDateTime::currentDateTime(), buffer);
    buffer1 = BinarySerialize(clients_info);
    //client->write(buffer1);
    for(QTcpSocket *cli : clients){
        cli->write(buffer1);
    }
}
