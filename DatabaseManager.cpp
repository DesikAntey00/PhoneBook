#include "DatabaseManager.h"

DatabaseManager::DatabaseManager(const QString& dbName) {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName);
}

DatabaseManager::~DatabaseManager() {
    closeDatabase();
}

bool DatabaseManager::openDatabase() {
    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
        return false;
    } else {
        return true;
    }
}

void DatabaseManager::closeDatabase() {
    db.close();
}

bool DatabaseManager::createTable() {
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS contacts ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "firstName TEXT, "
               "lastName TEXT, "
               "fatherName TEXT, "
               "address TEXT, "
               "birthDate TEXT, "
               "email TEXT, "
               "phoneNumbers TEXT, "
               "salary INTEGER)");
    return !query.lastError().isValid();
}

bool DatabaseManager::addContact(const Contact& contact) {
    QSqlQuery query;
    query.prepare("INSERT INTO contacts (firstName, lastName, fatherName, address, birthDate, email, phoneNumbers, salary) "
                  "VALUES (:firstName, :lastName, :fatherName, :address, :birthDate, :email, :phoneNumbers, :salary)");
    query.bindValue(":firstName", contact.firstName);
    query.bindValue(":lastName", contact.lastName);
    query.bindValue(":fatherName", contact.fatherName);
    query.bindValue(":address", contact.address);
    query.bindValue(":birthDate", contact.birthDate.toString("yyyy-MM-dd"));
    query.bindValue(":email", contact.email);
    query.bindValue(":phoneNumbers", contact.phoneNumbers.join(";"));
    query.bindValue(":salary", contact.salary);
    return query.exec();
}

bool DatabaseManager::removeContact(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM contacts WHERE id = :id");
    query.bindValue(":id", id);
    return query.exec();
}

bool DatabaseManager::editContact(int id, const Contact& contact) {
    QSqlQuery query;
    query.prepare("UPDATE contacts SET "
                  "firstName = :firstName, "
                  "lastName = :lastName, "
                  "fatherName = :fatherName, "
                  "address = :address, "
                  "birthDate = :birthDate, "
                  "email = :email, "
                  "phoneNumbers = :phoneNumbers, "
                  "salary = :salary "
                  "WHERE id = :id");
    query.bindValue(":firstName", contact.firstName);
    query.bindValue(":lastName", contact.lastName);
    query.bindValue(":fatherName", contact.fatherName);
    query.bindValue(":address", contact.address);
    query.bindValue(":birthDate", contact.birthDate.toString("yyyy-MM-dd"));
    query.bindValue(":email", contact.email);
    query.bindValue(":phoneNumbers", contact.phoneNumbers.join(";"));
    query.bindValue(":salary", contact.salary);
    query.bindValue(":id", id);
    return query.exec();
}

QList<Contact> DatabaseManager::getAllContacts() {
    QList<Contact> contacts;
    QSqlQuery query("SELECT * FROM contacts");
    while (query.next()) {
        Contact contact;
        contact.firstName = query.value("firstName").toString();
        contact.lastName = query.value("lastName").toString();
        contact.fatherName = query.value("fatherName").toString();
        contact.address = query.value("address").toString();
        contact.birthDate = QDate::fromString(query.value("birthDate").toString(), "yyyy-MM-dd");
        contact.email = query.value("email").toString();
        contact.phoneNumbers = query.value("phoneNumbers").toString().split(";", QString::SkipEmptyParts);
        contact.salary = query.value("salary").toInt();
        contacts.append(contact);
    }
    return contacts;
}

void DatabaseManager::clear() {
    QSqlQuery query;
    query.exec("DELETE FROM contacts");
}
