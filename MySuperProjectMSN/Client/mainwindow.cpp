#include <QTcpSocket>
#include <QCloseEvent>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);

    socket->connectToHost("127.0.0.1", 1234);

    connect(ui->sendButton, &QPushButton::clicked, this, &MainWindow::onSendButtonClicked);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &MainWindow::onDisconnected);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if(socket->state() == QAbstractSocket::ConnectedState) {
        qDebug() << "initialize disconnection";
        socket->disconnectFromHost();
        QMetaObject::invokeMethod(this, [this]() {
            if(socket->state() != QAbstractSocket::UnconnectedState) {
                socket->waitForDisconnected();
                qDebug() << "Waiting for disconnection";
            }
            }, Qt::QueuedConnection);
    }
    event->accept();
    qDebug() <<"Disconnected";
}

void MainWindow::onDisconnected() {
    qDebug() << "Socket disconnected";
}

void MainWindow::onSendButtonClicked() {
    if (socket->state() == QAbstractSocket::ConnectedState) {
        socket->write(ui->messageLineEdit->text().toUtf8());
    } else {
        qDebug() << "Not connected to the server.";
    }
}

void MainWindow::onReadyRead() {
    QByteArray data = socket->readAll();
    ui->chatTextEdit->append(QString::fromUtf8(data));
}
