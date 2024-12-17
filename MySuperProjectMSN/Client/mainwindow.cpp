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
    connect(ui->Wizz, &QToolButton::clicked, this, &MainWindow::onWizzClicked);
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

// Switch to the desire page using the index page
void MainWindow::connectButtonToPage(QAbstractButton* button, int pageIndex) {
    connect(button, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(pageIndex);
    });
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
