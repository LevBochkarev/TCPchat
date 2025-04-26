#include <QCoreApplication>
#include <QTimer>
#include "serverhandler.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    ServerHandler servHandler;
    servHandler.run();
    return a.exec();
}

