#include "server.h"
#include <QDebug>

Server::Server(QObject *parent) : QTcpServer(parent) {
    qDebug() << "Server initialized.";
}

void Server::incomingConnection(qintptr socketDescriptor) {
    qDebug() << "Incoming connection with descriptor:" << socketDescriptor;
    QTcpSocket *clientSocket = new QTcpSocket(this);
    if (clientSocket->setSocketDescriptor(socketDescriptor)) {
        clients.append(clientSocket);
        connect(clientSocket, &QTcpSocket::readyRead, this, &Server::onReadyRead);
        connect(clientSocket, &QTcpSocket::disconnected, this, &Server::onClientDisconnected);
        qDebug() << "Client connected successfully.";
    } else {
        qDebug() << "Failed to set socket descriptor.";
        delete clientSocket;
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

void Server::onReadyRead() {
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket *>(sender());
    if (clientSocket) {
        QByteArray data = clientSocket->readAll();
        QString message = QString::fromUtf8(data).trimmed();

        qDebug() << "Message reçu de" << clientSocket->peerAddress().toString() << ":" << message;

        messageQueue.enqueue(message);

        relayMessage(message, clientSocket);
    }
}

void Server::relayMessage(const QString &message, QTcpSocket *sender) {
    for (QTcpSocket *client : clients) {
        if (client != sender) {
            QString formattedMessage = QString("%1 : %2")
                                           .arg(sender->socketDescriptor())
                                           .arg(message);
            client->write(formattedMessage.toUtf8());
            qDebug() << "Message relayé à:" << client->peerAddress().toString();
        }
    }
}
