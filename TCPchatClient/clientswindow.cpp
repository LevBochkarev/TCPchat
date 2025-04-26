#include "clientswindow.h"
#include "ui_clientswindow.h"
#include <QDebug>

ClientsWindow::ClientsWindow(QWidget *parent, QStringList clients_new) :
    QDialog(parent),
    ui(new Ui::ClientsWindow)
{
    ui->setupUi(this);
    qDebug() << clients_new;
    update_clients(clients_new);
}

ClientsWindow::~ClientsWindow()
{
    delete ui;
}

void ClientsWindow::update_clients(QStringList clients_new)
{
    clients = clients_new;
    update_Table();
}

void ClientsWindow::update_Table()
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(clients.count());
    for(int i = 0; i < clients.count(); i++){
        QTableWidgetItem* item = new QTableWidgetItem;
        ui->tableWidget->setItem(i,0,item);
        ui->tableWidget->item(i,0)->setText(clients[0]);
    }
}
