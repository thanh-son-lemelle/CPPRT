#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QThread>

class ClientSocket : public QObject
{
    Q_OBJECT

public:
    explicit ClientSocket(QObject *parent = nullptr);
    ~ClientSocket();

    void connectToServer(const QString &host, quint16 port);
    void sendMessage(const QString &message);

signals:
    void messageReceived(const QString &message);
    void serverResponse(const QString &response);
    void errorOccurred(const QString &error);
    void connectionEstablished();
    void connectionClosed();

private slots:
    void onReadyRead();
    void onDisconnected();
    void onErrorOccurred(QAbstractSocket::SocketError socketError);

private:
    QString hostAddress;
    quint16 hostPort;
    bool attemptReconnect;
    QTcpSocket *socket;

    void reconnectToServer();
};

#endif // CLIENTSOCKET_H
