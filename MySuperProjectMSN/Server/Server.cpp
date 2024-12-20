#include "ConnexionController.h"
#include "server.h"
#include <QDebug>

Server::Server(QObject *parent) : QTcpServer(parent) {
    qDebug() << "Server initialized.";
}

void Server::incomingConnection(qintptr socketDescriptor) {
    qDebug() << "Incoming connection with descriptor:" << socketDescriptor;
    QTcpSocket *clientSocket = new QTcpSocket(this);
    if (clientSocket->setSocketDescriptor(socketDescriptor)) {
        clients.append(clientSocket);
        connect(clientSocket, &QTcpSocket::readyRead, this, &Server::onReadyRead);
        connect(clientSocket, &QTcpSocket::disconnected, this, &Server::onClientDisconnected);
        qDebug() << "Client connected successfully.";
    } else {
        qDebug() << "Failed to set socket descriptor.";
        delete clientSocket;
    }
}

void Server::onReadyRead() {
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket *>(sender());
    if (!clientSocket) {
        qDebug() << "Invalid client socket in onReadyRead.";
        return;
    }

    QByteArray data = clientSocket->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) {
        sendMessage(clientSocket, "Error: Invalid JSON data received.");
        return;
    }

    QJsonObject request = doc.object();
    QString requestType = request["type"].toString();

    if (requestType == "register") {
        QString email = request["email"].toString();
            qDebug() << "Register" << email;

        if (ConnexionController::isEmailExists(email)) {
            sendRegistrationResponse(clientSocket, false, "Email already exists.");
        } else {
            ConnexionController::saveUserInfo(request);
            sendRegistrationResponse(clientSocket, true, "Registration successful.");
        }

    } else if (requestType == "login") {
        QString email = request["email"].toString();
        QString password = request["password"].toString();
        qDebug() << "Login" << email << password;

        if (ConnexionController::validateCredentials(email, password)) {
            QJsonObject userInfo = ConnexionController::getUserInfo(email);
            sendLoginResponse(clientSocket, true, "Login successful.", userInfo);
            QJsonObject allUsersEmailUsername = ConnexionController::getAllUserInfo(email);
            sendAllUsersResponse(clientSocket, "Recupération ok.", allUsersEmailUsername);
        } else {
            sendLoginResponse(clientSocket, false, "Invalid credentials.", QJsonObject());
        }

    } else if (requestType == "message") {
        QString message = request["message"].toString();
        qDebug() << "Login" << message;

        /* if (validateCredentials(email, password)) {
            sendLoginResponse(clientSocket, true, "Login successful.");
        } else {
            sendLoginResponse(clientSocket, false, "Invalid credentials.");
        } */
        for (QTcpSocket *socket : clients){
            if (socket != clientSocket) {
                socket->write(data);
            }
        }
    } else {
        sendMessage(clientSocket, "Error: Unknown request type.");
    }
}

void Server::onClientDisconnected() {
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (clientSocket) {
        clients.removeAll(clientSocket);
        clientSocket->deleteLater();
        qDebug() << "Client disconnected.";
    } else {
        qDebug() << "Error: Disconnected signal from an invalid socket.";
    }
}

void Server::sendMessage(QTcpSocket *clientSocket, const QString &message) {
    QJsonObject response;
    response["type"] = "message";
    response["content"] = message;

    QJsonDocument doc(response);
    clientSocket->write(doc.toJson());
    clientSocket->flush();
}

void Server::sendRegistrationResponse(QTcpSocket *clientSocket, bool success, const QString &message) {
    QJsonObject response;
    response["type"] = "registration";
    response["status"] = success;
    response["message"] = message;

    QJsonDocument doc(response);
    clientSocket->write(doc.toJson());
    clientSocket->flush();
}

void Server::sendAllUsersResponse(QTcpSocket *clientSocket, const QString &message, QJsonObject allUsersInfo) {
    QJsonObject response;
    response["type"] = "allUsers";
    response["message"] = message;
    response["userinfo"] = allUsersInfo;

    QJsonDocument doc(response);
    clientSocket->write(doc.toJson());
    clientSocket->flush();
}

void Server::sendLoginResponse(QTcpSocket *clientSocket, bool success, const QString &message, QJsonObject userInfo) {
    QJsonObject response;
    response["type"] = "login";
    response["status"] = success ;
    response["message"] = message;
    response["userinfo"] = userInfo;

    QJsonDocument doc(response);
    clientSocket->write(doc.toJson());
    clientSocket->flush();
}

