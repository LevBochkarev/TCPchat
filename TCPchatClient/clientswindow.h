#ifndef CLIENTSWINDOW_H
#define CLIENTSWINDOW_H

#include <QDialog>

namespace Ui {
class ClientsWindow;
}

class ClientsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ClientsWindow(QWidget *parent = nullptr, QStringList clients = QStringList(0));
    ~ClientsWindow();
    void update_clients(QStringList clients);

private:
    Ui::ClientsWindow *ui;
    QStringList clients;

    void update_Table();
};

#endif // CLIENTSWINDOW_H
