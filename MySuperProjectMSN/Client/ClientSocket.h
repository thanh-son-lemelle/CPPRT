#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QThread>
#include <QJsonObject>
#include <QJsonDocument>
#include <QTimer>

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
    QString getUserEmail() const { return userEmail; }
    QString getUserName() const { return userName; }
    QString getUserFirstName() const { return userFirstName; }
    QString getUserLastName() const { return userLastName; }
    QString getUserPassword() const { return userPassword; }

signals:
    void messageReceived(const QJsonObject &message);
    void serverResponse(const QString &response);
    void errorOccurred(const QString &error);
    void connectionEstablished();
    void connectionClosed();
    void loginSuccess();
    void loginError();
    void registrationSuccess();
    void registrationError();
    void fetchAllUserInfo(QJsonObject userInfo);
    void userinformations();

private slots:
    void onReadyRead();
    void onDisconnected();
    void onErrorOccurred(QAbstractSocket::SocketError socketError);

private:
    QString hostAddress;
    quint16 hostPort;
    bool attemptReconnect;
    QTcpSocket *socket;

    void attemptInitialConnection();
    void reconnectToServer();
    void handleServerResponse(const QByteArray &data);

    QString userEmail;
    QString userName;
    QString userFirstName;
    QString userLastName;
    QString userPassword;

    QTimer *reconnectTimer;
    QTimer *initialConnectionTimer;

};

#endif // CLIENTSOCKET_H
