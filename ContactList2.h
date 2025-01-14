// ContactList.h
#pragma once

#include <QList>
#include <QFile>
#include <QTextStream>
#include "Contact2.h"
#include "DatabaseManager.h"

class ContactList {
private:
    QList<Contact> contacts;
    DatabaseManager* dbManager;

public:
    ContactList();
    ~ContactList();

    void addContact(const Contact& contact);
    void removeContact(int index);
    void editContact(int index, const Contact& contact);
    QList<Contact> getContacts() const;
    Contact getContact(int index) const;

    void clear();
    void saveToFile(const QString& filename);
    void loadFromFile(const QString& filename);

    void saveToDatabase();
    void loadFromDatabase();
};
