#include <QCloseEvent>
#include <QDebug>
#include <QMessageBox>
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
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred), this, &MainWindow::onErrorOccurred);
    setupStatusBar();
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

void MainWindow::onErrorOccurred(QAbstractSocket::SocketError socketError) {
    qDebug() << "Socket error:" << socketError << socket->errorString();
    ui->statusbar->showMessage("Error: " + socket->errorString());

    if (socketError == QAbstractSocket::ConnectionRefusedError ||
        socketError == QAbstractSocket::RemoteHostClosedError ||
        socketError == QAbstractSocket::NetworkError) {
        reconnectTimer->start(reconnectInterval);
    }
}

void MainWindow::attemptReconnect() {
    qDebug() << "Attempting to reconnect to the server...";
    ui->statusbar->showMessage("Reconnecting...");
    socket->abort();
    socket->connectToHost("127.0.0.1", 1234);
    if (socket->state() == QAbstractSocket::ConnectedState) {
        reconnectTimer->stop();
        ui->statusbar->showMessage("Reconnected.");
    }
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

void MainWindow::setupStatusBar()
{
    // Create a QLabel for status messages
    statusLabel = new QLabel("Connecting...", this);

    // Add widgets to the status bar
    ui->statusbar->addWidget(statusLabel);        // Left-aligned widget

    // Example: Update status when connected
    connect(socket, &QTcpSocket::connected, this, [this]() {
        statusLabel->setText("Connected");
    });
}


