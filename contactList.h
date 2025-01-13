#pragma once
#include <vector>
#include <fstream>
#include <sstream>
#include "contact.h"

class ContactList {
private:
    std::vector<Contact> contacts;

public:
    void addContact(const Contact& contact);
    void removeContact(int index);
    void editContact(int index, const Contact& contact);
    std::vector<Contact> getContacts() const;
    Contact getContact(int index) const;
    void clear();
    void saveToFile(const std::string& filename);
    void loadFromFile(const std::string& filename);
    void sortByField(const std::string& field);
    std::vector<Contact> searchContacts(const std::string& query) const;
    void printContacts() const;
    bool isUnique(const Contact& newContact) const;
};
