#include "ClientSocket.h"
#include <QDebug>

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
    hostAddress = host;
    hostPort = port;
    attemptReconnect = true;

    qDebug() << "Attempting to connect to server at" << host << "on port" << port;
    socket->connectToHost(host, port);

    if (socket->waitForConnected(3000)) {
        qDebug() << "Connection established.";
        emit connectionEstablished();
    } else {
        qDebug() << "Failed to connect to server:" << socket->errorString();
        emit errorOccurred(socket->errorString());
    }
}

void ClientSocket::sendMessage(const QString &message) {
    if (message != ""){
    if (socket->state() == QAbstractSocket::ConnectedState) {
        socket->write(message.toUtf8());
        qDebug() << "Message sent to server:" << message;
    } else {
        qDebug() << "Error: Cannot send message. Socket is not connected.";
        emit errorOccurred("Socket is not connected.");
    }
        }}

void ClientSocket::onReadyRead() {
    QByteArray data = socket->readAll();
    QString message = QString::fromUtf8(data);
    qDebug() << "Message received from server:" << message;
    emit messageReceived(message);
}

void ClientSocket::onDisconnected() {
    qDebug() << "Socket disconnected from server.";
    emit connectionClosed();
    if (attemptReconnect) {
        qDebug() << "Attempting to reconnect...";
        reconnectToServer();
    }
}


void ClientSocket::onErrorOccurred(QAbstractSocket::SocketError socketError) {
    Q_UNUSED(socketError)
    QString errorMsg = "Socket error occurred: " + socket->errorString();
    qDebug() << errorMsg;
    emit errorOccurred(errorMsg);
}

void ClientSocket::reconnectToServer() {
    QThread::sleep(2);
    qDebug() << "Reconnecting to" << hostAddress << "on port" << hostPort;
    socket->abort();
    socket->connectToHost(hostAddress, hostPort);
}
