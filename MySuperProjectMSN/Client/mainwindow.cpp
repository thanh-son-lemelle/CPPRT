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

    ui->ConversationFrameLayout->setAlignment(Qt::AlignBottom);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onSendButtonClicked() {
    QString text = ui->messageTextEdit->toPlainText();
    if (text.isEmpty()) return;
    QLabel *messageSentLabel = new QLabel(text);
    clientSocket->sendMessage(text);
    ui->messageTextEdit->clear();

    QHBoxLayout *messageLayout = new QHBoxLayout();
    messageSentLabel->setContentsMargins(5,5,5,5);
    messageLayout->setContentsMargins(10,-5,50,5);
    messageLayout->addWidget(messageSentLabel);
    messageLayout->setAlignment(messageSentLabel, Qt::AlignLeft);
    ui->ConversationFrameLayout->addLayout(messageLayout);
}

void MainWindow::onMessageReceived(const QString &message) {
    QLabel *messageLabel = new QLabel(message);

    QHBoxLayout *messageLayout = new QHBoxLayout();
    messageLabel->setContentsMargins(5,5,5,5);
    messageLayout->setContentsMargins(50,-5,10,5);
    messageLayout->addWidget(messageLabel);
    messageLayout->setAlignment(messageLabel, Qt::AlignRight);

    ui->ConversationFrameLayout->addLayout(messageLayout);
}

void MainWindow::onConnectionEstablished() {
    qDebug() << "Connected to server.";
}

void MainWindow::onConnectionClosed() {
    qDebug() << "Disconnected from server.";
}

void MainWindow::onErrorOccurred(const QString &error) {
    qDebug() << "Error: " << error;
}
