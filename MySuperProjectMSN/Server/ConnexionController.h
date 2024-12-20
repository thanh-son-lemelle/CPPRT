#ifndef CONNEXIONCONTROLLER_H
#define CONNEXIONCONTROLLER_H

#include <QCoreApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>

class ConnexionController: public QObject
{
    Q_OBJECT
public:
    explicit ConnexionController(QObject *parent = nullptr);
    bool static validateCredentials(const QString& email, const QString& password);
    bool static isEmailExists(QString &email);
    void static saveUserInfo(const QJsonObject &userInfo);
    QJsonObject static getUserInfo(const QString &email);

protected:

private:
    QString filePath;
    QJsonObject loadUserInfo();
    void updateUserInfo();
    void displayUserInfo();
};

#endif //CONNEXIONCONTROLLER
