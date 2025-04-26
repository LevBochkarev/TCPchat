#ifndef CONNECTIONWINDOW_H
#define CONNECTIONWINDOW_H

#include <QMessageBox>
#include <QAbstractButton>
#include <QErrorMessage>
//#include <QHostAddress>

namespace Ui {
class ConnectionWindow;
}

class ConnectionWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectionWindow(QWidget *parent = nullptr,
                              QString nickname = "",
                              QString ip = "", QString LocalPort = "",
                              QString ForeignPort = "");
    ~ConnectionWindow();

    void checkConnection(bool status);

private slots:
    void on_ConnectButton_clicked();

    void on_CancelButton_clicked();

private:
    Ui::ConnectionWindow *ui;

signals:
    void sendConnectionParameters(QString IP, QString LocalPort,
                                  QString ForeignPort, QString nickname);
    void startConnection();
};

#endif // CONNECTIONWINDOW_H
