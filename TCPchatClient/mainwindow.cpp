#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QScrollBar>
#include <QDateTime>
//#define MESSAGE_SEP "$Nickname$"
//#define IMAGE_SEP "$Image$"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->MessagesScrollArea->setWidget(ui->scrollAreaWidgetContents);
    ui->scrollAreaWidgetContents->setLayout(ui->MessagesLayout);
    ui->MessagesScrollArea->setAlignment(Qt::AlignBottom);
    ui->MessagesLayout->setDirection(QBoxLayout::TopToBottom);
    connect(ui->MessagesScrollArea->verticalScrollBar(), &QScrollBar::rangeChanged, this, &MainWindow::moveSlider);

    //delete player;
    player  = new QMediaPlayer;
    player->setMedia(QUrl("E:/work/3sem/3/UDPchat/notification1.wav"));
    player->setVolume(volume);
    setWindowTitle("Не подключен");
    qDebug()<<sizeof(QString);

    // Создаем и настраиваем клиенсткий сокет.
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::socketReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, [=]{setWindowTitle("Не подключен");});
    connect(socket, &QTcpSocket::connected, this, [=]{setWindowTitle(nickname + " Подключен. IP: " + IP + " Порт: " + ForeignPort);});

    on_SetConnectionAction_triggered();
}

MainWindow::~MainWindow()
{
    delete ui;

}

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

QByteArray MainWindow::BinarySerialize(const Protocol &segment)
{
    QByteArray buffer;
    QDataStream stream(&buffer, QIODevice::WriteOnly);
    stream << segment;
    return buffer;
}

Protocol MainWindow::BinaryDeserialize(const QByteArray &buffer)
{
    Protocol segment;
    QDataStream stream(buffer);
    stream >> segment;
    return segment;
}

Protocol MainWindow::PackSegment(int type, QString name, QDateTime date, QByteArray data)
{
    Protocol segment;
    segment.type = type;
    segment.name = name;
    segment.date = date;
    segment.data = data;
    return segment;
}

void MainWindow::renderMessage(QString message, QString nickname, QDateTime date, MessageStyle type)
{
    MessageBalloon* mb = new MessageBalloon(this, message, nickname, date, type);
    switch(type){
    case RECEIVED:
        ui->MessagesLayout->addWidget(mb);
        player->stop();
        player->play();
        break;
    case SENDED:
        QHBoxLayout* HLayout = new QHBoxLayout;
        int layoutWidth = ui->MessagesScrollArea->width() - mb->width();
        HLayout->addSpacerItem(new QSpacerItem(layoutWidth-15,1,QSizePolicy::Preferred, QSizePolicy::Minimum));
        HLayout->addWidget(mb);
        ui->MessagesLayout->addLayout(HLayout);
        break;
    }
}

void MainWindow::renderImage(QString nickname, QByteArray ba, QDateTime date, MessageStyle type)
{
    ImageBalloon* ib = new ImageBalloon(this, ba, nickname, date, type);//поменять для картинок!
    switch(type){
    case RECEIVED:
        ui->MessagesLayout->addWidget(ib);
        player->stop();
        player->play();
        break;
    case SENDED:
        QHBoxLayout* HLayout = new QHBoxLayout;
        int layoutWidth = ui->MessagesScrollArea->width() - ib->width();
        HLayout->addSpacerItem(new QSpacerItem(layoutWidth-15,1,QSizePolicy::Preferred, QSizePolicy::Minimum));
        HLayout->addWidget(ib);
        ui->MessagesLayout->addLayout(HLayout);
        break;
    }
}

void MainWindow::moveSlider()
{
    QScrollBar *bar = ui->MessagesScrollArea->verticalScrollBar();
    bar->setSliderPosition(bar->maximumHeight());
}

void MainWindow::setNotificVolume(int newVolume)
{
    volume = newVolume;
    player->setVolume(volume);
}

void MainWindow::sendImage(QByteArray ba)
{
    if(socket == nullptr){
        //Поругаться
        return;
    }
    if(EmojiTableWindow != nullptr){
        EmojiTableWindow->close();
        delete EmojiTableWindow;
        EmojiTableWindow = nullptr;
    }
    renderImage("Вы", ba, QDateTime::currentDateTime(), SENDED);
    //renderMessage(message, nickname, SENDED);
    ui->textEditSend->clear();


    Protocol segment = PackSegment(2, nickname, QDateTime::currentDateTime(), ba);

    //int type = 2;
    //QDataStream stream(&buf, QIODevice::WriteOnly);
    //stream << type << nickname << ba << QDateTime::currentDateTime();
    QByteArray buf = BinarySerialize(segment);
    socket->write(buf);
    buf.clear();
    //socket->writeDatagram(buf, remoteAddress, us_remotePort);
}

void MainWindow::on_emojiButton_clicked()
{
    if(EmojiTableWindow == nullptr){
        EmojiTableWindow = new EmojiWindow(this, ui->emojiButton);
        EmojiTableWindow->exec();
    }
    else{
        EmojiTableWindow->close();
        delete EmojiTableWindow;
        EmojiTableWindow = nullptr;
    }
}


void MainWindow::on_SetConnectionAction_triggered()
{
    cw = new ConnectionWindow(this, nickname, IP, LocalPort, ForeignPort);
    connect(cw, &ConnectionWindow::sendConnectionParameters, this, &MainWindow::ReceiveParams);
    connect(cw, &ConnectionWindow::startConnection, this, &MainWindow::bindSocket);
    connect(this, &MainWindow::sendConnectionStatus, cw, &ConnectionWindow::checkConnection, Qt::UniqueConnection);
    cw->exec();
}

void MainWindow::bindSocket()
{
    // "Координаты" сервера (IP-адрес и порт).
    //QString serverIp = ui->serverIpEdit->text();

    //QString n_IP = "192.168.0.167";
    //QString n_port = "64514";

    QHostAddress serverAddress(IP);
    ushort serverPort = ForeignPort.toUShort();

    // Подключаемся к серверу.
    // Может быть долго...
    socket->connectToHost(serverAddress, serverPort);

    // Дождаться подключения к серверу можно и тут.
    // Ожидание (10 сек) подключения ...
    connected = socket->waitForConnected(10000);
    //ui->sendMessageButton->setEnabled(connected);
    emit sendConnectionStatus(connected);
    if(!connected) {
        // Подключиться к серверу не удалось (закончился таймаут ожидания).
        socket->disconnectFromHost();
    }
    else{
        delete cw;
        cw = nullptr;
//        setWindowTitle(nickname + " Подключен. IP: " + IP + " Порт: " + ForeignPort);

    }
}


void MainWindow::on_sendButton_clicked()
{
    if(socket == nullptr){
        //Поругаться
        return;
    }
    if(ui->textEditSend->toPlainText() == ""){
        //Поругаться
        return;
    }
    QString message = ui->textEditSend->toPlainText();

    //QByteArray data;
    //QDataStream sendStream(&data, QIODevice::WriteOnly);
    //int type = 1;
    QDateTime date = QDateTime::currentDateTime();

    //sendStream << type << nickname << message << date;
    Protocol segment = PackSegment(1, nickname, date, message.toUtf8());
    QByteArray data = BinarySerialize(segment);
    socket->write(data);

    if(EmojiTableWindow != nullptr){
        EmojiTableWindow->close();
        delete EmojiTableWindow;
        EmojiTableWindow = nullptr;
    }

    renderMessage(message, "Вы", date, SENDED);
    ui->textEditSend->clear();
}

void MainWindow::socketReadyRead()
{
    // Клиентский сокет, который прислал данные

    QDataStream receiveStream(socket);
    receiveStream.startTransaction();
    Protocol segment;
    receiveStream >> segment;
    if(!receiveStream.commitTransaction()){
        //Ожидаем данные
        return;
    }
    //Protocol segment = BinaryDeserialize(buffer);
    if(segment.type == 1){
        renderMessage(QString(segment.data), segment.name, segment.date, RECEIVED);
    }
    else if(segment.type == 2){
        //qDebug()<<"receive"<<type<<data.length();
        renderImage(segment.name, segment.data, segment.date, RECEIVED);
    }
    else if(segment.type == 3){
        //qDebug() << segment.data << segment.type;
        qDebug() << segment.data;
        QDataStream stream(segment.data);
        clients_ip.clear();
        stream >> clients_ip;
        qDebug() << clients_ip;
    }
}

void MainWindow::ReceiveParams(QString rec_IP,
                               QString rec_LocalPort, QString rec_ForeignPort,
                               QString rec_Nickname)
{
    IP = rec_IP;
    nickname = rec_Nickname;
    //LocalPort = rec_LocalPort;
    ForeignPort = rec_ForeignPort;
    bindSocket();
    //qDebug() << rec_IP << rec_ForeignPort << rec_LocalPort;
}

void MainWindow::on_VolumeAction_triggered()
{
    VolumeDialog* vd = new VolumeDialog(this, volume);
    connect(vd, &VolumeDialog::sendVolume, this, &MainWindow::setNotificVolume);
    vd->exec();
}


void MainWindow::on_photoButton_clicked()
{
    qDebug()<<"enter";
    QString filePath = QFileDialog::getOpenFileName(this, "Открыть файл", ".", "Изображение (*.png *.jpg *.jpeg)");
    if(filePath.isEmpty()){
        //QMessageBox::information(this, "Внимание!", "Файл не выбран", QMessageBox::Ok);
        return;
    }
    QFile* file = new QFile(filePath);
    if (file->size() > 5242880){ //Проверяем размер, сравнивая с 5 МБ
        qDebug()<<file->size();
        QMessageBox::information(this, "Внимание!", "Размер изображения должен быть менее 5 Мб", QMessageBox::Ok);
        delete file;
        file = nullptr;
        return;
    }
    delete file;
    file = nullptr;
    QImage imag1(filePath);

    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    imag1.save(&buffer, "PNG");
    //qDebug()<<"buffer";
    buffer.close();
    sendImage(ba);
    ba.clear();
}


void MainWindow::on_ClientsAction_triggered()
{
    ClientsWindow *cw = new ClientsWindow(this, clients_ip);
    qDebug() << clients_ip;
    cw->exec();
}

