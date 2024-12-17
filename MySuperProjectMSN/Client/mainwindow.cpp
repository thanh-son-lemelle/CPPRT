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

    connect(ui->sendButton, &QPushButton::clicked, this, &MainWindow::onSendButtonClicked);
    connect(clientSocket, &ClientSocket::messageReceived, this, &MainWindow::onMessageReceived);
    connect(clientSocket, &ClientSocket::connectionEstablished, this, &MainWindow::onConnectionEstablished);
    connect(clientSocket, &ClientSocket::connectionClosed, this, &MainWindow::onConnectionClosed);
    connect(clientSocket, &ClientSocket::errorOccurred, this, &MainWindow::onErrorOccurred);
    connect(ui->Wizz, &QToolButton::clicked, this, &MainWindow::onWizzClicked);

    // Switch between SignIn and LogIn page
    connect(ui->LogInPushButtonCreate, &QPushButton::clicked, this,[=](){
    ui->stackedWidget->setCurrentIndex(0);
    });

    connect(ui->SignInPushButtonLogin, &QPushButton::clicked, this,[=](){
        ui->stackedWidget->setCurrentIndex(1);
    });

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
void MainWindow::SwitchPage(int PageIndex)
{
    ui->stackedWidget->setCurrentIndex(PageIndex);
    qDebug()<< "Page Index: "<< PageIndex;
}


void MainWindow::onWizzClicked() {
    QSequentialAnimationGroup *shakeAnimation = new QSequentialAnimationGroup(this);
    QPoint originalPos = ui->ChatPanel->pos();

    for (int i = 0; i < 6; ++i) {
        int offset = (i % 2 == 0) ? 10 : -10;
        QPropertyAnimation *shake = new QPropertyAnimation(ui->ChatPanel, "pos", this);
        shake->setDuration(100);
        shake->setStartValue(originalPos);
        shake->setEndValue(originalPos + QPoint(offset, 0));
        shake->setEasingCurve(QEasingCurve::InOutSine);
        shakeAnimation->addAnimation(shake);
    }

    QPropertyAnimation *returnToOriginal = new QPropertyAnimation(ui->ChatPanel, "pos", this);
    returnToOriginal->setDuration(100);
    returnToOriginal->setStartValue(ui->ChatPanel->pos());
    returnToOriginal->setEndValue(originalPos);
    returnToOriginal->setEasingCurve(QEasingCurve::InOutSine);
    shakeAnimation->addAnimation(returnToOriginal);

    shakeAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}
