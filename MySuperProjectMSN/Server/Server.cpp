#include "Server.h"
#include <QDebug>

// Constructor to initialize server and start listening for connections
Server::Server(QObject *parent) : QObject(parent)
{
    server = new QTcpServer(this);

    connect(server, &QTcpServer::newConnection, this, &Server::onNewConnection);

    if (!server->listen(QHostAddress::Any, 2512)) {
        qCritical() << "Server could not start!";
    } else {
        qDebug() << "Server started, waiting for connections...";
    }
}

// Slot to handle new client connections
void Server::onNewConnection()
{
    QTcpSocket *clientSocket = server->nextPendingConnection();
    clients << clientSocket;

    connect(clientSocket, &QTcpSocket::readyRead, this, &Server::onReadyRead);
    connect(clientSocket, &QTcpSocket::disconnected, this, &Server::onClientDisconnected);

    qDebug() << "Client connected!";
}

// Slot to handle incoming messages from clients
void Server::onReadyRead()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket *>(sender());
    if (clientSocket) {
        QByteArray message = clientSocket->readAll();
        qDebug() << message;

        for (QTcpSocket *client : clients) {
            client->write(message);
        }
    }
}

// Slot to handle client disconnection
void Server::onClientDisconnected()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket *>(sender());
    if (clientSocket) {
        clients.removeAll(clientSocket);
        clientSocket->deleteLater();
        qDebug() << "Client disconnected!";
    }
}
