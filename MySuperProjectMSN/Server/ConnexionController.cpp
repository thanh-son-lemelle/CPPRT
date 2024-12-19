#include <QDebug>
#include "ConnexionController.h"
#include <QFileInfo>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

ConnexionController::ConnexionController(QObject *parent) :QObject(parent) {

}

bool ConnexionController::validateCredentials(const QString& email, const QString& password) {
    QString filePath = "../../../Server/data/userInfo.json";
    QFile file(filePath);

    // Ouvre le fichier en mode lecture seule
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open file:" << filePath;
        return false;
    }

    // Lit le contenu du fichier
    QByteArray jsonData = file.readAll();
    file.close();

    // Analyse le document JSON
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (!doc.isObject()) {
        qWarning() << "Invalid JSON format in file.";
        return false;
    }

    // Extrait le tableau "users"
    QJsonObject rootObj = doc.object();
    if (!rootObj.contains("users") || !rootObj["users"].isArray()) {
        qWarning() << "No 'users' array found in JSON.";
        return false;
    }

    QJsonArray usersArray = rootObj["users"].toArray();

    // Parcours des utilisateurs pour vérifier l'email et le mot de passe
    for (const QJsonValue& value : usersArray) {
        if (!value.isObject()) {
            continue;
        }

        QJsonObject userObj = value.toObject();
        QString storedEmail = userObj["email"].toString();
        QString storedPassword = userObj["password"].toString();

        if (storedEmail == email && storedPassword == password) {
            qDebug() << "User authenticated successfully.";
            return true;  // Match trouvé
        }
    }

    qDebug() << "Authentication failed: Invalid email or password.";
    return false;  // Aucune correspondance trouvée
}

bool ConnexionController::isEmailExists(QString &email){

    QString filePath = "../../../Server/data/userInfo.json";
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Unable to open file:" << filePath;
        return false;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (!doc.isObject()) {
        qWarning() << "Invalid JSON format in file.";
        return false;
    }

    QJsonObject rootObj = doc.object();
    if (!rootObj.contains("users") || !rootObj["users"].isArray()) {
        qWarning() << "No 'users' array found in JSON.";
        return false;
    }

    QJsonArray usersArray = rootObj["users"].toArray();

    for (const QJsonValue &value : usersArray) {
        QJsonObject existingUser = value.toObject();
        qDebug() << existingUser["email"].toString();
        if (existingUser["email"].toString() == email) {
            return true;
        }
    }
    return false;
}

void ConnexionController::saveUserInfo(const QJsonObject &userInfo) {

    // If the file does not exist, create a new JSON file.
    QString filePath = "../../../Server/data/userInfo.json";
    QFile file(filePath);
    if (!file.exists()) {
        if (file.open(QIODevice::WriteOnly)) {
            QJsonObject emptyObject; QJsonDocument doc(emptyObject);
            file.write(doc.toJson(QJsonDocument::Indented));
            file.close();
            qDebug() << "Creating file...";
        } else {
            qWarning() << "Unable to open file for writing:" << filePath;
        }
    } else {
        qDebug() << "File already exists.";
    }

    // Create a JSON object with user information
   /* userInfo = value.toObject();
    userInfo["firstName"] = "John";
    userInfo["lastName"] = "Doe";
    userInfo["username"] = "johndoe123";
    userInfo["email"] = "johndoe@example.com";
    userInfo["password"] = "securepassword";
    userInfo["userId"] = 1;*/

    // Open the file for reading and updating
    if (file.open(QIODevice::ReadWrite)) {
        // Read the existing content of the file
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        QJsonArray usersArray;

        if (doc.isObject() && doc.object().contains("users")) {
            usersArray = doc.object()["users"].toArray();
        }

        // Add the new user to the users array
        usersArray.append(userInfo);

        // Write the updated array back to the file
        QJsonObject root;
        root["users"] = usersArray;
        QJsonDocument updatedDoc(root);
        file.resize(0);  // Clear the file before writing the updated data
        file.write(updatedDoc.toJson(QJsonDocument::Indented));
        file.close();

        qDebug() << "User added successfully!";
    } else {
        qWarning() << "Unable to open file for reading and writing:" << filePath;
    }
}

QJsonObject ConnexionController::getUserInfo(const QString &email) {
    QString filePath = "../../../Server/data/userInfo.json";
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Unable to open file:" << filePath;
        return QJsonObject();
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (!doc.isObject()) {
        qWarning() << "Invalid JSON format in file.";
        return QJsonObject();
    }

    QJsonObject rootObj = doc.object();
    if (!rootObj.contains("users") || !rootObj["users"].isArray()) {
        qWarning() << "No 'users' array found in JSON.";
        return QJsonObject();
    }

    QJsonArray usersArray = rootObj["users"].toArray();

    for (const QJsonValue &value : usersArray) {
        if (!value.isObject()) {
            continue;
        }

        QJsonObject userObj = value.toObject();
        if (userObj["email"].toString() == email) {
            qDebug() << "User found:" << userObj;
            return userObj;
        }
    }

    qWarning() << "User not found with email:" << email;
    return QJsonObject();
}
