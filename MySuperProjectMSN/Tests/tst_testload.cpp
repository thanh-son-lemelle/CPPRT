#include <QtTest>
#include <QTcpSocket>
#include "server.h"

class TestLoad : public QObject {
    Q_OBJECT

    private:
        Server *server;
        quint16 port;

    private slots:
        void initTestCase();
        void cleanupTestCase();

        // Test cases
        void test_multipleClientsConnectAndDisconnect();
        void test_invalidConnectionHandling();
        void test_maliciousConnectionLimiting();
};

void TestLoad::initTestCase() {
    const QString validAddress = "127.0.0.1";
    const quint16 validPort = 12345;

    server->startServer(validAddress, validPort);

    QVERIFY(server->isListening());
    QCOMPARE(server->serverAddress().toString(), validAddress);
    QCOMPARE(server->serverPort(), validPort);
}

void TestLoad::cleanupTestCase() {
    server->close();
    delete server;
}

void TestLoad::test_multipleClientsConnectAndDisconnect() {
    QList<QTcpSocket*> clients;
    const long clientCount = 5;

    for (int i = 0; i < clientCount; ++i) {
        QTcpSocket *client = new QTcpSocket(this);
        qDebug() << "server listening on port: "<< server->serverAddress() <<server->serverPort();
        client->connectToHost(QHostAddress::Any, 1234);
        qDebug() << server->clients.size();
        QVERIFY(client->waitForConnected(1000));
        clients.append(client);
    }

    qDebug() <<"clients size = "<<server->clients.size()<< "clientCount = "<< clientCount;
    QCOMPARE(server->clients.size(), clientCount);


    for (QTcpSocket *client : clients) {
        client->disconnectFromHost();
        QVERIFY(client->waitForDisconnected(10000));
        delete client;
    }

    QTest::qWait(100);
    QCOMPARE(server->clients.size(), 0);
}

void TestLoad::test_invalidConnectionHandling() {
    QTcpSocket client;
    client.connectToHost("invalid.host", port);
    QVERIFY(!client.waitForConnected(1000));

    QTcpSocket closedClient;
    closedClient.connectToHost(QHostAddress::LocalHost, port);
    QVERIFY(closedClient.waitForConnected(1000));
    closedClient.close();

    QTest::qWait(100);
    QCOMPARE(server->clients.size(), 0);
}

void TestLoad::test_maliciousConnectionLimiting() {
    const int maxConnections = 10;
    int maliciousAttempts = 15;

    QList<QTcpSocket*> clients;
    for (int i = 0; i < maliciousAttempts; ++i) {
        QTcpSocket *client = new QTcpSocket(this);
        client->connectToHost(QHostAddress::LocalHost, port);

        if (i < maxConnections) {
            QVERIFY(client->waitForConnected(1000));
            clients.append(client);
        } else {
            QVERIFY(!client->waitForConnected(1000));
            delete client;
        }
    }

    QCOMPARE(server->clients.size(), maxConnections);

    for (QTcpSocket *client : clients) {
        client->disconnectFromHost();
        QVERIFY(client->waitForDisconnected(1000));
        delete client;
    }

    QTest::qWait(100);
    QCOMPARE(server->clients.size(), 0);
}

QTEST_MAIN(TestLoad)
#include "tst_testload.moc"

