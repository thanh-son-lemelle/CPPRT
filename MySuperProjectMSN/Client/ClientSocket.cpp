#include "ClientSocket.h"

ClientSocket::ClientSocket(QObject *parent) :
    QObject(parent),
    socket(new QTcpSocket(this))
{
    connect(socket, &QTcpSocket::readyRead, this, &ClientSocket::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &ClientSocket::onDisconnected);
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred),
            this, &ClientSocket::onErrorOccurred);
}

ClientSocket::~ClientSocket() {}

void ClientSocket::connectToServer(const QString &host, quint16 port) {
    socket->connectToHost(host, port);
    if (socket->waitForConnected(3000)) {
        emit connectionEstablished();
    } else {
        emit errorOccurred("Failed to connect to server.");
    }
}

void ClientSocket::sendMessage(const QString &message) {
    if (socket->state() == QAbstractSocket::ConnectedState) {
        socket->write(message.toUtf8());
    } else {
        emit errorOccurred("Socket is not connected.");
    }
}

void ClientSocket::onReadyRead() {
    QByteArray data = socket->readAll();
    emit messageReceived(QString::fromUtf8(data));
}

void ClientSocket::onDisconnected() {
    emit connectionClosed();
}

void ClientSocket::onErrorOccurred(QAbstractSocket::SocketError socketError) {
    Q_UNUSED(socketError)
    emit errorOccurred(socket->errorString());
}
