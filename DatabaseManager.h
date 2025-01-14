#pragma once

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include "Contact2.h"

class DatabaseManager {
public:
    DatabaseManager(const QString& dbName);
    ~DatabaseManager();

    bool openDatabase();
    void closeDatabase();
    bool createTable();
    bool addContact(const Contact& contact);
    bool removeContact(int id);
    bool editContact(int id, const Contact& contact);
    QList<Contact> getAllContacts();
    void clear(); // Добавлен метод clear

private:
    QSqlDatabase db;
};
