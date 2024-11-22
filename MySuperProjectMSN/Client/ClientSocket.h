#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <QObject>
#include <QTcpSocket>

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
    void connectionEstablished();
    void connectionClosed();
    void errorOccurred(const QString &error);

private slots:
    void onReadyRead();
    void onDisconnected();
    void onErrorOccurred(QAbstractSocket::SocketError socketError);

private:
    QTcpSocket *socket;
};

#endif // CLIENTSOCKET_H
