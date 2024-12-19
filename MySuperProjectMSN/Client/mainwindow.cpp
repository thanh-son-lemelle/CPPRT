#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "ui_mainwindow.h"

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

    // Send message or Wizz on Chat Interface page
    connect(ui->sendButton, &QPushButton::clicked, this, &MainWindow::onSendButtonClicked);
    connect(ui->Wizz, &QToolButton::clicked, this, &MainWindow::onWizzClicked);

    // ClientSocket signal listener
    connect(clientSocket, &ClientSocket::messageReceived, this, &MainWindow::onMessageReceived);
    connect(clientSocket, &ClientSocket::connectionEstablished, this, &MainWindow::onConnectionEstablished);
    connect(clientSocket, &ClientSocket::connectionClosed, this, &MainWindow::onConnectionClosed);
    connect(clientSocket, &ClientSocket::errorOccurred, this, &MainWindow::onErrorOccurred);
    connect(clientSocket, &ClientSocket::loginSuccess, this, &MainWindow::connectButtonToChatInterface);
    connect(clientSocket, &ClientSocket::loginError, this, &MainWindow::displayInvalidPasswordEmail);
    connect(clientSocket, &ClientSocket::registrationSuccess, this, &MainWindow::connectButtonToChatInterface);
    clientSocket->connectToServer("127.0.0.1", 2512);

    // Display message chat from bottom of conversation frame
    ui->ConversationFrameLayout->setAlignment(Qt::AlignBottom);

    // Send email and password to server
    connect(ui->LoginValidationPushButton, &QPushButton::clicked, this, &MainWindow::onLoginClicked);
    connect(ui->EmailEdit, &QLineEdit::returnPressed, this, &MainWindow::onLoginClicked);
    connect(ui->PasswordEdit, &QLineEdit::returnPressed, this, &MainWindow::onLoginClicked);

    // Sender user info to Server
    connect(ui->CreateValidationPushButton, &QPushButton::clicked, this, &MainWindow::onSigninClicked);

    // Toggle password visibility (show or hide)
    onEyePen();

    // Hide warning message for invalid password or email
    ui->WarningLogin->setVisible(false);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onSendButtonClicked()
{
    QString text = ui->messageTextEdit->toPlainText();
    if (text.isEmpty())
        return;
    QLabel *messageSentLabel = new QLabel(text);
    clientSocket->sendMessage(text);
    ui->messageTextEdit->clear();

    QHBoxLayout *messageLayout = new QHBoxLayout();
    messageSentLabel->setContentsMargins(5, 5, 5, 5);
    messageLayout->setContentsMargins(10, -5, 50, 5);
    messageLayout->addWidget(messageSentLabel);
    messageLayout->setAlignment(messageSentLabel, Qt::AlignLeft);
    ui->ConversationFrameLayout->addLayout(messageLayout);
}

void MainWindow::onMessageReceived(const QString &message)
{
    QLabel *messageLabel = new QLabel(message);

    QHBoxLayout *messageLayout = new QHBoxLayout();
    messageLabel->setContentsMargins(5, 5, 5, 5);
    messageLayout->setContentsMargins(50, -5, 10, 5);
    messageLayout->addWidget(messageLabel);
    messageLayout->setAlignment(messageLabel, Qt::AlignRight);

    ui->ConversationFrameLayout->addLayout(messageLayout);
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
    connect(button, &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentIndex(pageIndex);
    });
}

void MainWindow::connectButtonToChatInterface()
{
    ui->WarningLogin->setVisible(false);
    ui->EmailEdit->text() = "";
    ui->PasswordEdit->text() = "";
    ui->stackedWidget->setCurrentIndex(2);
    // Display user name on chat interface
    ui->UserPseudo->setText(clientSocket->getUserName());

    // Display user info on profile page
    displayInfoUserProfile();
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
    else if (eyeButton == ui->EyePasswordAccount)
    {
        ui->PasswordPlaceholderAccount->setEchoMode(QLineEdit::Normal);
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
    else if (eyeButton == ui->EyePasswordAccount)
    {
        ui->PasswordPlaceholderAccount->setEchoMode(QLineEdit::Password);
    }

}

// Show or hide password depending on the eyePassword button
void MainWindow::onEyePen(){
    QList<QToolButton *> buttons = {
        ui->EyePassword,
        ui->EyeNewPassword,
        ui->EyeConfirmPassword,
        ui->EyePasswordAccount
    };

        for (QToolButton *button : buttons)
        {
            connect(button, &QToolButton::pressed, [this, button]() {
                onEyePasswordPressed(button);
            });
            connect(button, &QToolButton::released, [this, button]() {
                onEyePasswordReleased(button);
            });
    }
        QList<QPushButton *> buttonsPen = {
            ui->PenEditFirstName,
            ui->PenEditLastName,
            ui->PenEditUsername,
            ui->PenEditEmail,
            ui->PenEditPassword
        };
        for(QPushButton *button: buttonsPen){
            connect(button, &QToolButton::clicked, [this, button]() {
                editUserInfo(button);
            });
    }
}

void MainWindow::displayInfoUserProfile(){
    ui->FirstNamePlaceholderAccount->setText(clientSocket->getUserFirstName());
    ui->LastNamePlaceholderAccount->setText(clientSocket->getUserLastName());
    ui->UsernamePlaceholderAccount->setText(clientSocket->getUserName());
    ui->EmailPlaceholderAccount->setText(clientSocket->getUserEmail());
    ui->PasswordPlaceholderAccount->setText(clientSocket->getUserPassword());
}

void MainWindow::editUserInfo(QPushButton *penButton)
{
    // Define styles for read-only and editable states
    QString initBorder = "background-color: rgb(21, 23, 24); border-radius: 5px; border: 2px solid rgb(35, 35, 35);";
    QString editBorder = "background-color: rgb(21, 23, 24); border-radius: 5px; border: 2px solid rgb(255, 255, 255);";

    // List of buttons and corresponding QLineEdits
    QList<QPushButton *> buttonsPen = {
        ui->PenEditFirstName,
        ui->PenEditLastName,
        ui->PenEditUsername,
        ui->PenEditEmail,
        ui->PenEditPassword
    };
    QList<QLineEdit *> userPlaceholders = {
        ui->FirstNamePlaceholderAccount,
        ui->LastNamePlaceholderAccount,
        ui->UsernamePlaceholderAccount,
        ui->EmailPlaceholderAccount,
        ui->PasswordPlaceholderAccount
    };
    QList<QFrame *> userInputs = {
        ui->InputFirstNameAccount,
        ui->InputLastNameAccount,
        ui->InputUsernameAccount,
        ui->InputEmailAccount,
        ui->InputPasswordAccount
    };

    // Iterate through the buttons to find the corresponding QLineEdit
    for (int i = 0; i < buttonsPen.size(); ++i) {
        if (penButton == buttonsPen[i]) {
            bool isReadOnly = userPlaceholders[i]->isReadOnly();
            userPlaceholders[i]->setReadOnly(!isReadOnly);
            userInputs[i]->setStyleSheet(isReadOnly ? editBorder : initBorder);
            break;
        }
    }
}

void MainWindow::displayInvalidPasswordEmail(){
    ui->WarningLogin->setVisible(true);
}
