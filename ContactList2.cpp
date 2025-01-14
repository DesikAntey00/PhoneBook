#include "ContactList2.h"

ContactList::ContactList() {
    dbManager = new DatabaseManager("contacts.db");
    if (dbManager->openDatabase()) {
        dbManager->createTable();
    }
}

ContactList::~ContactList() {
    delete dbManager;
}

void ContactList::addContact(const Contact& contact) {
    if (contact.isValid()) {
        contacts.append(contact);
        dbManager->addContact(contact);
    }
}

void ContactList::removeContact(int index) {
    if (index >= 0 && index < contacts.size()) {
        Contact contact = contacts.at(index);
        contacts.removeAt(index);
        dbManager->removeContact(index);
    }
}

void ContactList::editContact(int index, const Contact& contact) {
    if (index >= 0 && index < contacts.size() && contact.isValid()) {
        contacts[index] = contact;
        dbManager->editContact(index, contact);
    }
}

QList<Contact> ContactList::getContacts() const {
    return contacts;
}

Contact ContactList::getContact(int index) const {
    if (index >= 0 && index < contacts.size()) {
        return contacts.at(index);
    }
    return Contact();
}

void ContactList::clear() {
    contacts.clear();
    dbManager->clear();
}

void ContactList::saveToFile(const QString& filename) {
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (Contact& contact : contacts) {
            out << contact.firstName << "," << contact.lastName << "," << contact.fatherName << "," << contact.address << "," << contact.birthDate.toString() << ","  /* << contact.email << ","<< contact.salary*/ << "\n";
            for (const QString& phone : contact.phoneNumbers) {
                out << phone << ";";
            }
            out << "\n";
        }
        file.close();
    }
    else {
        qDebug() << "Failed to open file for writing:" << filename;
    }
}

void ContactList::loadFromFile(const QString& filename) {
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            Contact contact;
            QString line = in.readLine();
            QStringList fields = line.split(",");
            if (fields.size() >= 7) {
                contact.firstName = fields[0].trimmed();
                contact.lastName = fields[1].trimmed();
                contact.fatherName = fields[2].trimmed();
                contact.address = fields[3].trimmed();
                contact.birthDate = QDate::fromString(fields[4].trimmed(), "dd.MM.yyyy");
                contact.email = fields[5].trimmed();
                contact.salary = fields[6].trimmed().toInt();
                line = in.readLine();
                contact.phoneNumbers = line.split(";", QString::SkipEmptyParts);
                addContact(contact);
            }
        }
        file.close();
    }
}

void ContactList::saveToDatabase() {
    dbManager->clear();
    for (const Contact& contact : contacts) {
        dbManager->addContact(contact);
    }
}

void ContactList::loadFromDatabase() {
    contacts = dbManager->getAllContacts();
}
