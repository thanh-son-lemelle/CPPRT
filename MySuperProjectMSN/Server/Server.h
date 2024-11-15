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

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void onNewConnection();
    void onReadyRead();

private:
    QList<QTcpSocket*> clients;
};

#endif // SERVEUR_H
