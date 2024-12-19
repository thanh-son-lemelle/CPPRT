#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::MainWindow),
                                          clientSocket(new ClientSocket(this))
{
    ui->setupUi(this);

    // Define initial page as LogIn
    ui->stackedWidget->setCurrentIndex(0);

    // Switch between pages: SignUp(1), Login(0), ChatInterface(2) and Profile (3)
    connectButtonToPage(ui->LogInPushButtonCreate, 0);
    connectButtonToPage(ui->SignInPushButtonLogin, 1);
    connectButtonToPage(ui->BtnEditProfile, 3);
    connectButtonToPage(ui->BtnDisconnect, 0);
    connectButtonToPage(ui->BtnQuitProfile, 2);
    connectButtonToPage(ui->SubmitAccountPushButton, 2);
    // connectButtonToPage(ui->CreateValidationPushButton, 0);

    connect(ui->sendButton, &QPushButton::clicked, this, &MainWindow::onSendButtonClicked);
    connect(clientSocket, &ClientSocket::messageReceived, this, &MainWindow::onMessageReceived);
    connect(clientSocket, &ClientSocket::connectionEstablished, this, &MainWindow::onConnectionEstablished);
    connect(clientSocket, &ClientSocket::connectionClosed, this, &MainWindow::onConnectionClosed);
    connect(clientSocket, &ClientSocket::errorOccurred, this, &MainWindow::onErrorOccurred);
    connect(clientSocket, &ClientSocket::loginSuccess, this, &MainWindow::connectButtonToChatInterface);
    connect(clientSocket, &ClientSocket::registrationSuccess, this, &MainWindow::connectButtonToChatInterface);
    connect(ui->Wizz, &QToolButton::clicked, this, &MainWindow::onWizzClicked);
    clientSocket->connectToServer("10.10.11.20", 2512);

    // Dans le constructeur de MainWindow
    QStandardItemModel *model = new QStandardItemModel(this);
    ui->ConversationList->setModel(model);
    ui->ConversationList->scrollToBottom();

    // Send email and password to server
    connect(ui->LoginValidationPushButton, &QPushButton::clicked, this, &MainWindow::onLoginClicked);
    connect(ui->EmailEdit, &QLineEdit::returnPressed, this, &MainWindow::onLoginClicked);
    connect(ui->PasswordEdit, &QLineEdit::returnPressed, this, &MainWindow::onLoginClicked);

    connect(ui->CreateValidationPushButton, &QPushButton::clicked, this, &MainWindow::onSigninClicked);

    // Toggle password visibility (show or hide)
    onEye();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onSendButtonClicked()
{
    QString text = ui->messageTextEdit->toPlainText();
    if (text.isEmpty()) return;
    clientSocket->sendMessage(text);

    QStandardItem *sentItem = createMessageItem(text, true);
    QStandardItemModel *model = qobject_cast<QStandardItemModel *>(ui->ConversationList->model());
    model->appendRow(sentItem);

    ui->messageTextEdit->clear();
    ui->ConversationList->scrollToBottom();
}


void MainWindow::onMessageReceived(const QJsonObject &object)
{
    QString username = object["username"].toString();
    QString message = object["message"].toString();

    QString fullMessage = username + "\n" + message;
    QStandardItem *receivedItem = createMessageItem(fullMessage, false);
    QStandardItemModel *model = qobject_cast<QStandardItemModel *>(ui->ConversationList->model());
    model->appendRow(receivedItem);

    ui->ConversationList->scrollToBottom();
}


void MainWindow::onConnectionEstablished()
{
    qDebug() << "Connected to server.";
}

void MainWindow::onConnectionClosed()
{
    qDebug() << "Disconnected from server.";
}

void MainWindow::onErrorOccurred(const QString &error)
{
    qDebug() << "Error: " << error;
}

// Switch to the desire page using the index page
void MainWindow::connectButtonToPage(QAbstractButton *button, int pageIndex)
{
    connect(button, &QPushButton::clicked, this, [=]()
            { ui->stackedWidget->setCurrentIndex(pageIndex); });
}

void MainWindow::connectButtonToChatInterface()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::onWizzClicked()
{
    QSequentialAnimationGroup *shakeAnimation = new QSequentialAnimationGroup(this);
    QPoint originalPos = ui->ChatPanel->pos();

    for (int i = 0; i < 6; ++i)
    {
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

void MainWindow::onLoginClicked()
{

    QString email = ui->EmailEdit->text();
    QString password = ui->PasswordEdit->text();

    if (email.isEmpty() || password.isEmpty())
        return;
    clientSocket->sendLoginRequest(email, password);

    // connectButtonToPage(ui->LoginValidationPushButton, 2);
}

void MainWindow::onSigninClicked()
{
    QString firstName = ui->FirstNamePlaceholder->text();
    QString lastName = ui->LastNamePlaceholder->text();
    QString username = ui->UsernamePlaceholder->text();
    QString email = ui->EmailPlaceholder->text();
    QString password = ui->PasswordPlaceholder->text();
    QString confirmPassword = ui->ConfirmPasswordPlaceholder->text();

    if (email.isEmpty() || password.isEmpty() || firstName.isEmpty() || lastName.isEmpty() || username.isEmpty())
    {
        qDebug() << "All fields must be completed.";
        return;
    };
    if (password != confirmPassword)
    {
        qDebug() << "Password doesn't match";
        return;
    }
    clientSocket->sendRegistrationRequest(firstName, lastName, email, password, username);
}

// Slot to shoow the password
void MainWindow::onEyePasswordPressed(QToolButton *eyeButton)
{
    if (eyeButton == ui->EyePassword)
    {
        ui->PasswordEdit->setEchoMode(QLineEdit::Normal);
    }
    else if (eyeButton == ui->EyeNewPassword)
    {
        ui->PasswordPlaceholder->setEchoMode(QLineEdit::Normal);
    }
    else if (eyeButton == ui->EyeConfirmPassword)
    {
        ui->ConfirmPasswordPlaceholder->setEchoMode(QLineEdit::Normal);
    }
}

// Slot to hide the password
void MainWindow::onEyePasswordReleased(QToolButton *eyeButton)
{
    if (eyeButton == ui->EyePassword)
    {
        ui->PasswordEdit->setEchoMode(QLineEdit::Password);
    }
    else if (eyeButton == ui->EyeNewPassword)
    {
        ui->PasswordPlaceholder->setEchoMode(QLineEdit::Password);
    }
    else if (eyeButton == ui->EyeConfirmPassword)
    {
        ui->ConfirmPasswordPlaceholder->setEchoMode(QLineEdit::Password);
    }
}

// Show or hide password depending on the eyePassword button
void MainWindow::onEye()
{
    QList<QToolButton *> buttons = {ui->EyePassword, ui->EyeNewPassword, ui->EyeConfirmPassword};

    for (QToolButton *button : buttons)
    {
        connect(button, &QToolButton::pressed, [this, button]()
                { onEyePasswordPressed(button); });
        connect(button, &QToolButton::released, [this, button]()
                { onEyePasswordReleased(button); });
    }
}

QStandardItem* MainWindow::createMessageItem(const QString &message, bool sent) {
    QStandardItem *item = new QStandardItem(message);

    if (sent) {
        item->setTextAlignment(Qt::AlignLeft);
    } else {
        item->setTextAlignment(Qt::AlignRight);
    }
    return item;
}
