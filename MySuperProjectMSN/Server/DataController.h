#ifndef DATACONTROLLER_H
#define DATACONTROLLER_H

#include <QCoreApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>

class DataController: public QObject
{
    Q_OBJECT
public:
    explicit DataController(QObject *parent = nullptr);
    bool static validateCredentials(const QString& email, const QString& password);
    bool static isEmailExists(QString &email);
    void static saveUserInfo(const QJsonObject &userInfo);
    QJsonObject static getUserInfo(const QString &email);
    QJsonObject static getAllUserInfo(const QString &email);
    void static saveEvent(const QString &type, const QString &sender, const QString &receiver, const QString &message);

protected:

private:
    QString filePath;
    QJsonObject loadUserInfo();
    void updateUserInfo();
    void displayUserInfo();
};

#endif //DataController
