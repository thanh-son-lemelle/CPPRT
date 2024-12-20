#include "server.h"
#include <QDebug>

Server::Server(QObject *parent) : QTcpServer(parent) {}

void Server::startServer(const QString &address, quint16 port) {
    const QHostAddress hostAddress(address);
    if (this->listen(hostAddress, port)) {
        qDebug() << "Server started at" << address << ":" << port;
    } else {
        qDebug() << "Server failed to start:" << this->errorString();
    }
}

void Server::incomingConnection(qintptr socketDescriptor) {
    qDebug() << "Incoming connection with descriptor:" << socketDescriptor;
    QTcpSocket *clientSocket = new QTcpSocket(this);
    if (clientSocket->setSocketDescriptor(socketDescriptor)) {
        clients.append(clientSocket);
        connect(clientSocket, &QTcpSocket::readyRead, this, &Server::onReadyRead);
        connect(clientSocket, &QTcpSocket::disconnected, this, &Server::onClientDisconnected);
        qDebug() << "Client connected successfully." << serverAddress()<< serverPort() ;
        qDebug() <<"clientSocket size : " << clients.size();
    } else {
        qDebug() << "Failed to set socket descriptor.";
        delete clientSocket;
    }
}

void Server::onReadyRead() {
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket) {
        qDebug() << "Invalid client socket in onReadyRead.";
        return;
    }
    QByteArray data = clientSocket->readAll();
    qDebug() << "Data received from client:" << data;
    for (QTcpSocket *socket : clients) {
        if (socket != clientSocket) {
            socket->write(data);
        }
    }
}

void Server::onClientDisconnected() {
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (clientSocket) {
        clients.removeAll(clientSocket);
        clientSocket->deleteLater();
        qDebug() << "Client disconnected.";
    } else {
        qDebug() << "Error: Disconnected signal from an invalid socket.";
    }
}
