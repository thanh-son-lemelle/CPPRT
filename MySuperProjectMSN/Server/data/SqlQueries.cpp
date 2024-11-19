#include "DataBaseConnexion.cpp"
#include <QCoreApplication>
#include <QtSql/QSqlQuery>
#include <iostream>

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    // Create a database connection instance
    DatabaseConnexion db("localhost", "root", "VannyLamorte25!", "msn");

    // Check the connection before executing queries
    if (db.connect()) {
        // Execute a query to fetch only the first name from the name field
        QSqlQuery query = db.fetch("SELECT SUBSTRING_INDEX(name, ' ', 1) AS first_name FROM user");
        while (query.next()) {
            QString firstName = query.value("first_name").toString();
            std::cout << "First Name: " << firstName.toStdString() << std::endl;
        }

        // Disconnect from the database
        db.disconnect();
    } else {
        std::cerr << "Unable to connect to the database." << std::endl;
    }

    return 0;
}
