#include <QCoreApplication>
#include "Server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Server server;
    if (!server.listen(QHostAddress::Any, 1234)) {
        qCritical() << "Unable to start the server: " << server.errorString();
        return 1;
    }
    qDebug() << "Server started...";

    return a.exec();
}
