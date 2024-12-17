#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    clientSocket(new ClientSocket(this))
{
    ui->setupUi(this);

    // Define initial page as LogIn
    ui->stackedWidget->setCurrentIndex(2);

    // Switch between pages: SignUp(0), Login(1), ChatInterface(2) and Profile (3)
    connectButtonToPage(ui->LogInPushButtonCreate, 0);
    connectButtonToPage(ui->SignInPushButtonLogin, 1);
    connectButtonToPage(ui->LoginValidationPushButton, 2);
    connectButtonToPage(ui->BtnEditProfile, 3);
    connectButtonToPage(ui->BtnDisconnect, 0);
    connectButtonToPage(ui->BtnQuitProfile, 2);
    connectButtonToPage(ui->SubmitAccountPushButton, 2);
    connectButtonToPage(ui->CreateValidationPushButton, 1);

    connect(ui->sendButton, &QPushButton::clicked, this, &MainWindow::onSendButtonClicked);
    connect(clientSocket, &ClientSocket::messageReceived, this, &MainWindow::onMessageReceived);
    connect(clientSocket, &ClientSocket::connectionEstablished, this, &MainWindow::onConnectionEstablished);
    connect(clientSocket, &ClientSocket::connectionClosed, this, &MainWindow::onConnectionClosed);
    connect(clientSocket, &ClientSocket::errorOccurred, this, &MainWindow::onErrorOccurred);

    clientSocket->connectToServer("127.0.0.1", 1234);

    QLabel *messageLabel1 = new QLabel("TEST");
    ui->ConversationFrameLayout->addWidget(messageLabel1);

    QLabel *messageLabel2 = new QLabel("TEST2");
    ui->ConversationFrameLayout->addWidget(messageLabel2);
    ui->ConversationFrameLayout->setAlignment(Qt::AlignBottom);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onSendButtonClicked() {
    clientSocket->sendMessage(ui->messageLineEdit->text());
    ui->messageLineEdit->clear();
}

void MainWindow::onMessageReceived(const QString &message) {
    QLabel *messageLabel = new QLabel(message);
    ui->ConversationFrameLayout->addWidget(messageLabel);
}

void MainWindow::onConnectionEstablished() {
    qDebug()<< "Connected to server.";
    // ui->statusbar->showMessage("Connected to server.");
}

void MainWindow::onConnectionClosed() {
    qDebug()<< "DisConnected from server.";
    // ui->statusbar->showMessage("Disconnected from server.");
}

void MainWindow::onErrorOccurred(const QString &error) {
    qDebug()<< "Error: " << error;
    // ui->statusbar->showMessage("Error: " + error);
}

// Switch to the desire page using the index page
void MainWindow::connectButtonToPage(QAbstractButton* button, int pageIndex) {
    connect(button, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(pageIndex);
    });
}


