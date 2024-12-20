#include <QCoreApplication>
#include "Server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Server server;
    server.startServer("127.0.0.1", 1234);

    return a.exec();
}
