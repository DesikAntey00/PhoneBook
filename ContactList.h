#pragma once
#include <QList>
#include <QFile>
#include <QTextStream>
#include "Contact.h"
class ContactList {
private:
    QList<Contact> contacts;
public:
    void addContact(const Contact& contact);
    void removeContact(int index);
    void editContact(int index, const Contact& contact);
    QList<Contact> getContacts() const;
    Contact getContact(int index) const;

    void clear();
    void saveToFile(const QString& filename);
    void loadFromFile(const QString& filename);
};

