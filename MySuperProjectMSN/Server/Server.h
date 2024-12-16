#ifndef SERVEUR_H
#define SERVEUR_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include <QHostAddress>
#include <QQueue>

class Server : public QTcpServer {
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void onReadyRead();
    void onClientDisconnected();
    void relayMessage(const QString &message, QTcpSocket *sender);
private:
    QList<QTcpSocket*> clients;
    QQueue<QString> messageQueue;
};

#endif // SERVEUR_H
