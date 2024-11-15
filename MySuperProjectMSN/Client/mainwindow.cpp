#include <QTcpSocket>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);

    // Connectez-vous au serveur au démarrage
    socket->connectToHost("127.0.0.1", 1234);

    connect(ui->sendButton, &QPushButton::clicked, this, &MainWindow::onSendButtonClicked);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::onReadyRead);
}

MainWindow::~MainWindow()
{
    delete ui;
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
