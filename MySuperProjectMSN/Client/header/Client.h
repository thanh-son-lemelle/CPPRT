#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>

class Client : public QObject {
    Q_OBJECT

public:
    explicit Client(QObject *parent = nullptr);
    void connectSocket();

signals:
    void messageReceived(const QString &message);

public slots:
    void sendMessage(const QString &message);

private slots:
    void onReadyRead();

private:
    QTcpSocket *socket;
    QString username;
};

#endif // CLIENT_H
