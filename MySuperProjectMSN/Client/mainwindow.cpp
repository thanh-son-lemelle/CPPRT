#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    clientSocket(new ClientSocket(this))
{
    ui->setupUi(this);

    connect(ui->sendButton, &QPushButton::clicked, this, &MainWindow::onSendButtonClicked);
    connect(clientSocket, &ClientSocket::messageReceived, this, &MainWindow::onMessageReceived);
    connect(clientSocket, &ClientSocket::connectionEstablished, this, &MainWindow::onConnectionEstablished);
    connect(clientSocket, &ClientSocket::connectionClosed, this, &MainWindow::onConnectionClosed);
    connect(clientSocket, &ClientSocket::errorOccurred, this, &MainWindow::onErrorOccurred);

    clientSocket->connectToServer("127.0.0.1", 1234);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onSendButtonClicked() {
    clientSocket->sendMessage(ui->messageLineEdit->text());
    ui->messageLineEdit->clear();
}

void MainWindow::onMessageReceived(const QString &message) {
    ui->chatTextEdit->append(message);
}

void MainWindow::onConnectionEstablished() {
    ui->statusbar->showMessage("Connected to server.");
}

void MainWindow::onConnectionClosed() {
    ui->statusbar->showMessage("Disconnected from server.");
}

void MainWindow::onErrorOccurred(const QString &error) {
    ui->statusbar->showMessage("Error: " + error);
}
