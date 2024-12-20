#ifndef SERVEUR_H
#define SERVEUR_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include <QHostAddress>

class Server : public QTcpServer {
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    QList<QTcpSocket*> clients;
    void startServer(const QString &address, quint16 port);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void onReadyRead();
    void onClientDisconnected();
private:
};

#endif // SERVEUR_H
