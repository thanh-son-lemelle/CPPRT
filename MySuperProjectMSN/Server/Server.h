#ifndef SERVEUR_H
#define SERVEUR_H
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include <QHostAddress>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

class Server : public QTcpServer {
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void onReadyRead();
    void onClientDisconnected();
private:
    QList<QTcpSocket*> clients;
    void sendMessage (QTcpSocket *clientSocket, const QString &message);
    void sendRegistrationResponse(QTcpSocket *clientSocket, bool success, const QString &message);
    void sendLoginResponse(QTcpSocket *clientSocket, bool success, const QString &message, QJsonObject userInfo);
    void sendAllUsersResponse(QTcpSocket *clientSocket, const QString &message, QJsonObject allUsersInfo);

};

#endif // SERVEUR_H
