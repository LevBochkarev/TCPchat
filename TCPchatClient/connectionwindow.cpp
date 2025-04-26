#include "connectionwindow.h"
#include "ui_connectionwindow.h"
#include "mainwindow.h"
#include <QUdpSocket>

ConnectionWindow::ConnectionWindow(QWidget *parent,
                                   QString nickname, QString ip,
                                   QString LocalPort, QString ForeignPort):
    QDialog(parent),
    ui(new Ui::ConnectionWindow)
{
    ui->setupUi(this);
    //setWindowFlags(Qt::SplashScreen | Qt::FramelessWindowHint);
    if(nickname != ""){
        ui->NicknameEdit->setText(nickname);
    }
//    if(LocalPort != ""){
//        ui->LocalPortEdit->setText(LocalPort);
//    }
    if(ip != ""){
        ui->RemoteIPEdit->setText(ip);
    }
    if(ForeignPort != ""){
        ui->RemotePortEdit->setText(ForeignPort);
    }
    //connect(parent, &MainWindow::sendConnectionStatus, this, &ConnectionWindow::on_CancelButton_clicked);
}

ConnectionWindow::~ConnectionWindow()
{
    delete ui;
}

void ConnectionWindow::checkConnection(bool status)
{
    //qDebug() << "111";
    if(status){
        close();
    }
    else{
        //Ругаемся
        QMessageBox::warning(this, "Ошибка подключения", "Не удалось установить подключение", QMessageBox::Ok);
        //delete error_message;
        //error_message = nullptr;
    }
}


void ConnectionWindow::on_ConnectButton_clicked()
{
    QString IP, LocalPort, Nickname, ForeignPort;
    IP = ui->RemoteIPEdit->text();
    //LocalPort = ui->LocalPortEdit->text();
    Nickname = ui->NicknameEdit->text();
    ForeignPort = ui->RemotePortEdit->text();

    emit sendConnectionParameters(IP, LocalPort, ForeignPort, Nickname);
}


void ConnectionWindow::on_CancelButton_clicked()
{
    close();
}

