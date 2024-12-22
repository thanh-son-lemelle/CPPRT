#include "ClientSocket.h"
#include <QDebug>

ClientSocket::ClientSocket(QObject *parent) : QObject(parent),
                                              socket(new QTcpSocket(this)),
                                              reconnectTimer(new QTimer(this)),
                                              initialConnectionTimer(new QTimer(this))
{
    connect(socket, &QTcpSocket::readyRead, this, &ClientSocket::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &ClientSocket::onDisconnected);
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred),
            this, &ClientSocket::onErrorOccurred);

    // Timer to handle first server conection
    initialConnectionTimer->setInterval(2000);
    initialConnectionTimer->setSingleShot(false);
    connect(initialConnectionTimer, &QTimer::timeout, this, &ClientSocket::attemptInitialConnection);

    // Timer to handle server reconection
    reconnectTimer->setInterval(2000);
    reconnectTimer->setSingleShot(false);
    connect(reconnectTimer, &QTimer::timeout, this, &ClientSocket::reconnectToServer);
}

ClientSocket::~ClientSocket() {}

void ClientSocket::connectToServer(const QString &host, quint16 port)
{
    hostAddress = host;
    hostPort = port;
    attemptReconnect = true;

    qDebug() << "Starting initial connection attempts to server at" << host << "on port" << port;
    initialConnectionTimer->start();
}

void ClientSocket::attemptInitialConnection()
{
    qDebug() << "Attempting to connect to" << hostAddress << "on port" << hostPort;
    socket->abort();
    socket->connectToHost(hostAddress, hostPort);

    if (socket->waitForConnected(3000))
    {
        qDebug() << "Connection established.";
        initialConnectionTimer->stop();
        emit connectionEstablished();
    }
    else
    {
        qDebug() << "Connection attempt failed. Retrying...";
    }
}

void ClientSocket::onReadyRead()
{
    QByteArray data = socket->readAll();
    handleServerResponse(data);
}

void ClientSocket::onDisconnected()
{
    qDebug() << "Socket disconnected from server.";
    emit connectionClosed();
    if (attemptReconnect)
    {
        qDebug() << "Attempting to reconnect...";
        reconnectTimer->start();
    }
}

// Throw error
void ClientSocket::onErrorOccurred(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError)
    QString errorMsg = "Socket error occurred: " + socket->errorString();
    qDebug() << errorMsg;
    emit errorOccurred(errorMsg);
}

// Attempt to reconnect to server if disconnected
void ClientSocket::reconnectToServer()
{
    qDebug() << "Attempting to reconnect to" << hostAddress << "on port" << hostPort;
    socket->abort();
    socket->connectToHost(hostAddress, hostPort);

    if (socket->waitForConnected(3000))
    {
        qDebug() << "Reconnection successful.";
        reconnectTimer->stop();
        emit connectionEstablished();
    }
    else
    {
        qDebug() << "Reconnection failed. Retrying...";
    }
}

void ClientSocket::handleServerResponse(const QByteArray &data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject())
    {
        qWarning() << "Invalid JSON response from server.";
        return;
    }

    QJsonObject response = doc.object();

    QString type = response["type"].toString();
    if (type == "message")
    {
        QString content = response["message"].toString();
        QString username = response["content"].toString();
        qDebug() << "Server message:" << content;
        emit messageReceived(response);
    }
    else if (type == "registration")
    {
        bool status = response["status"].toBool();
        QString message = response["message"].toString();
        qDebug() << "Registration Status:" << status << "-" << message;
        (status == true) ? emit registrationSuccess() : emit loginError();
    }
    else if (type == "login")
    {
        bool status = response["status"].toBool();
        QString message = response["message"].toString();
        if (status)
        {
            QJsonObject userInfo = response["userinfo"].toObject();
            userEmail = userInfo["email"].toString();
            userName = userInfo["username"].toString();
            userFirstName = userInfo["firstName"].toString();
            userLastName = userInfo["lastName"].toString();
            userPassword = userInfo["password"].toString();

            qDebug() << "User Info:";
            qDebug() << "Email:" << userEmail;
            qDebug() << "lastName:" << userLastName;
            qDebug() << "firstName:" << userFirstName;
            qDebug() << "userName:" << userName;

            emit loginSuccess();
        }
        else
        {
            emit loginError();
        }
    } else if (type == "allUsers"){
        // qDebug() << data;
        QJsonObject userinfo = response["userinfo"].toObject();
        emit fetchAllUserInfo(userinfo);
    } else if (type == "wizz"){
        QString username = response["username"].toString();
        QString message = response["message"].toString();
        qDebug() << "YOU HAVE A WIZZ from" << username
                 << ":" << message;
        emit receivedWizz(username, message);
    } else {
        qWarning() << "Unknown response type:" << type;
    }
}

void ClientSocket::sendRegistrationRequest(QString firstName, QString lastName, QString email, QString password, QString username)
{
    QJsonObject request;
    request["type"] = "register";
    request["email"] = email;
    request["password"] = password;
    request["firstName"] = firstName;
    request["lastName"] = lastName;
    request["username"] = username;

    QJsonDocument doc(request);
    socket->write(doc.toJson());
}

void ClientSocket::sendLoginRequest(QString email, QString password)
{
    QJsonObject request;
    request["type"] = "login";
    request["email"] = email;
    request["password"] = password;

    QJsonDocument doc(request);
    socket->write(doc.toJson());
}

void ClientSocket::sendMessage(const QString &message)
{

    if (message != "")
    {
        if (socket->state() == QAbstractSocket::ConnectedState)
        {
            QJsonObject request;
            request["type"] = "message";
            request["username"] = getUserName();
            request["message"] = message;
            request["sender"] = getUserEmail();
            request["receiver"] = getMessageReceiver();

            QJsonDocument doc(request);
            socket->write(doc.toJson());
            qDebug() << "Message sent to server:" << message;
        }
        else
        {
            qDebug() << "Error: Cannot send message. Socket is not connected.";
            emit errorOccurred("Socket is not connected.");
        }
    }
}

void ClientSocket::sendWizz(){
    QJsonObject request;
    request["type"] = "wizz";
    request["username"] = getUserName();
    request["message"] = "You received a Wizz !";
    request["sender"] = getUserEmail();
    request["receiver"] = getMessageReceiver();
    QJsonDocument doc(request);
    socket->write(doc.toJson());
    qDebug()<<"Wizz sent";
}
