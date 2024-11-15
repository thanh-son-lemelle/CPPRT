#include "Server.h"
#include <QDebug>

Server::Server(QObject *parent) : QTcpServer(parent) {
    connect(this, &QTcpServer::newConnection, this, &Server::onNewConnection);
    qDebug() << "Serveur initialisé.";
}

void Server::incomingConnection(qintptr socketDescriptor) {
    qDebug() << "Connexion entrante avec descripteur:" << socketDescriptor;
    QTcpSocket *clientSocket = new QTcpSocket(this);
    if (clientSocket->setSocketDescriptor(socketDescriptor)) {
        clients.append(clientSocket);
        connect(clientSocket, &QTcpSocket::readyRead, this, &Server::onReadyRead);
        qDebug() << "Client connecté avec succès.";
    } else {
        qDebug() << "Échec de la configuration du descripteur du socket.";
        delete clientSocket;
    }
}

void Server::onNewConnection() {
    QTcpSocket *clientSocket = nextPendingConnection();
    if (clientSocket) {
        clients.append(clientSocket);
        connect(clientSocket, &QTcpSocket::readyRead, this, &Server::onReadyRead);
        connect(clientSocket, &QTcpSocket::disconnected, this, [this, clientSocket]() {
            clients.removeAll(clientSocket);
            clientSocket->deleteLater();
            qDebug() << "Client déconnecté.";
        });
        qDebug() << "Nouvelle connexion établie.";
    } else {
        qDebug() << "Failed to get new connection socket.";
    }
}


void Server::onReadyRead() {
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket) {
        qDebug() << "Client socket invalide dans onReadyRead.";
        return;
    }
    QByteArray data = clientSocket->readAll();
    qDebug() << "Données reçues du client:" << data;
    for (QTcpSocket *socket : clients) {
        if (socket != clientSocket) {
            socket->write(data);
        }
    }
}
