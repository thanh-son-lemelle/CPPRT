#include <QCoreApplication>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <iostream>

class DatabaseConnexion {
public:
    DatabaseConnexion(const QString& host, const QString& user, const QString& password, const QString& database, int port = 3306)
        : host_(host), user_(user), password_(password), database_(database), port_(port) {}

    bool connect() {
        db_ = QSqlDatabase::addDatabase("QMYSQL");
        db_.setHostName(host_);
        db_.setPort(port_);
        db_.setDatabaseName(database_);
        db_.setUserName(user_);
        db_.setPassword(password_);
        if (!db_.open()) {
            std::cerr << "Error connecting to the database: " << db_.lastError().text().toStdString() << std::endl;
            return false;
        }
        std::cout << "Connection successful!" << std::endl;
        return true;
    }

    void disconnect() {
        if (db_.isOpen()) {
            db_.close();
            std::cout << "Connection closed." << std::endl;
        }
    }

    bool executeQuery(const QString& queryStr) {
        if (!connect()) return false;
        QSqlQuery query;
        if (!query.exec(queryStr)) {
            std::cerr << "Error executing query: " << query.lastError().text().toStdString() << std::endl;
            disconnect();
            return false;
        }
        disconnect();
        return true;
    }

    QSqlQuery fetch(const QString& queryStr) {
        if (!connect()) return QSqlQuery();
        QSqlQuery query;
        if (!query.exec(queryStr)) {
            std::cerr << "Error executing query: " << query.lastError().text().toStdString() << std::endl;
        }
        return query;
    }

private:
    QString host_;
    QString user_;
    QString password_;
    QString database_;
    int port_;
    QSqlDatabase db_;
};
