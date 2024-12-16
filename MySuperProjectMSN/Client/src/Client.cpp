#include "../header/Client.h"
#include <QDebug>

// Constructor for the Client class.
// Initializes the QTcpSocket and connects signals for handling socket events.
Client::Client(QObject *parent) : QObject(parent), socket(new QTcpSocket(this))
{
    connect(socket, &QTcpSocket::readyRead, this, &Client::onReadyRead);
    connectSocket();
}

// Disconnects from the host if already connected, then tries to connect the socket server
void Client::connectSocket()
{
    qDebug() << "Trying to connect to server...";
    if (socket->isOpen()) {
        socket->disconnectFromHost();
        qDebug() << "Disconnected from the server.";
    }
    socket->connectToHost("127.0.0.1", 2512);
}

// Sends a message to the server if the socket is open and the message is not empty
// Writes the message to the socket and send to the server
void Client::sendMessage(const QString &message)
{
    if (socket->isOpen() && !message.isEmpty()) {
        QString finalMessage = username + " : " + message;
        socket->write(finalMessage.toUtf8());
    }
}

// Slot called when data is available
// Reads all data from the socket and emits it as a signal to the UI
void Client::onReadyRead()
{
    QByteArray message = socket->readAll();
    emit messageReceived(QString::fromUtf8(message));
}
