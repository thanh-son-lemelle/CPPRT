#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QThread>
#include <QJsonObject>
#include <QJsonDocument>

class ClientSocket : public QObject
{
    Q_OBJECT

public:
    explicit ClientSocket(QObject *parent = nullptr);
    ~ClientSocket();

    void connectToServer(const QString &host, quint16 port);
    void sendMessage(const QString &message);
    void sendLoginRequest(QString email, QString password);
    void sendRegistrationRequest(QString firstName, QString lastName, QString email, QString password, QString username);

signals:
    void messageReceived(const QString &message);
    void serverResponse(const QString &response);
    void errorOccurred(const QString &error);
    void connectionEstablished();
    void connectionClosed();
    void loginSuccess();
    void loginError();
    void registrationSuccess();
    void registrationError();

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
    void handleServerResponse(const QByteArray &data);
};

#endif // CLIENTSOCKET_H
