#include <QTcpSocket>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , client(new Client(this))
{
    ui->setupUi(this);

    connect(ui->sendButton, &QPushButton::clicked, this, &MainWindow::sendMessage);
    connect(ui->messageLineEdit, &QLineEdit::returnPressed, this, &MainWindow::sendMessage);
    connect(client, &Client::messageReceived, this, &MainWindow::displayMessage);

    ConversationFrameLayout = ui->ConversationFrameLayout;
    messageLineEdit = ui->messageLineEdit;
    sendButton = ui->sendButton;
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
}

void MainWindow::sendMessage()
{
    QString message = messageLineEdit->text();
    if (!message.isEmpty()) {
        messageLineEdit->clear();
        client->sendMessage(message);
    }
}

void MainWindow::displayMessage(const QString &message) {
    // Créer une nouvelle étiquette pour le message
    QLabel *messageLabel = new QLabel(message);

    // Ajouter l'étiquette dans le layout
    ConversationFrameLayout->addWidget(messageLabel);

    // Pousser les éléments vers le bas pour imiter une conversation de chat
    ConversationFrameLayout->setAlignment(Qt::AlignBottom);


}

