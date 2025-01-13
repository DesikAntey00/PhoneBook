#include "contactList.h"
#include <algorithm>
#include <iomanip>
#include <sstream>

void ContactList::addContact(const Contact& contact) {
    if (contact.isValid() && isUnique(contact)) {
        contacts.push_back(contact);
    }
    else if (!isUnique(contact)) {
        std::cout << "Contact already exists.\n";
    }
}

void ContactList::removeContact(int index) {
    if (index >= 0 && index < contacts.size()) {
        contacts.erase(contacts.begin() + index);
    }
}

void ContactList::editContact(int index, const Contact& contact) {
    if (index >= 0 && index < contacts.size() && contact.isValid()) {
        contacts[index] = contact;
    }
}

std::vector<Contact> ContactList::getContacts() const {
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
}

void ContactList::saveToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        for (const auto& contact : contacts) {
            file << contact.firstName << "," << contact.lastName << "," << contact.fatherName << ","
                << contact.address << "," << std::put_time(&contact.birthDate, "%d.%m.%Y") << ","
                << contact.email << "," << contact.salary << ",";
            for (const auto& phone : contact.phoneNumbers) {
                file << phone << ";";
            }
            file << "\n";
        }
        file.close();
    }
}

void ContactList::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            Contact contact;
            std::istringstream iss(line);
            std::string field;
            std::getline(iss, field, ',');
            contact.firstName = field;
            std::getline(iss, field, ',');
            contact.lastName = field;
            std::getline(iss, field, ',');
            contact.fatherName = field;
            std::getline(iss, field, ',');
            contact.address = field;
            std::getline(iss, field, ',');
            std::istringstream dateStream(field);
            dateStream >> std::get_time(&contact.birthDate, "%d.%m.%Y");
            std::getline(iss, field, ',');
            contact.email = field;
            std::getline(iss, field, ',');
            contact.salary = std::stoi(field);
            std::getline(iss, field, ';');
            contact.phoneNumbers.push_back(field);
            while (std::getline(iss, field, ';')) {
                contact.phoneNumbers.push_back(field);
            }
            addContact(contact);
        }
        file.close();
    }
}

void ContactList::sortByField(const std::string& field) {
    if (field == "lastName") {
        std::sort(contacts.begin(), contacts.end(), [](const Contact& a, const Contact& b) {
            return a.lastName < b.lastName;
            });
    }
    else if (field == "birthDate") {
        std::sort(contacts.begin(), contacts.end(), [](const Contact& a, const Contact& b) {
            return std::mktime(const_cast<std::tm*>(&a.birthDate)) < std::mktime(const_cast<std::tm*>(&b.birthDate));
            });
    }
    else if (field == "age") {
        std::sort(contacts.begin(), contacts.end(), [](const Contact& a, const Contact& b) {
            return std::mktime(const_cast<std::tm*>(&a.birthDate)) > std::mktime(const_cast<std::tm*>(&b.birthDate));
            });
    }
    else if (field == "salary") {
        std::sort(contacts.begin(), contacts.end(), [](const Contact& a, const Contact& b) {
            return a.salary < b.salary;
            });
    }
}

std::vector<Contact> ContactList::searchContacts(const std::string& query) const {
    std::vector<Contact> results;
    for (const auto& contact : contacts) {
        if (contact.firstName.find(query) != std::string::npos ||
            contact.lastName.find(query) != std::string::npos ||
            contact.fatherName.find(query) != std::string::npos ||
            contact.address.find(query) != std::string::npos ||
            contact.email.find(query) != std::string::npos ||
            std::any_of(contact.phoneNumbers.begin(), contact.phoneNumbers.end(), [&query](const std::string& phone) {
                return phone.find(query) != std::string::npos;
                }) ||
            std::to_string(contact.salary).find(query) != std::string::npos) {
            results.push_back(contact);
        }
    }
    return results;
}

void ContactList::printContacts() const {
    std::cout << "Contact List:\n";
    for (const auto& contact : contacts) {
        std::cout << contact.firstName << " " << contact.lastName << " " << contact.fatherName << " "
            << contact.address << " " << std::put_time(&contact.birthDate, "%d.%m.%Y") << " "
            << contact.email << " " << contact.salary << " ";
        for (const auto& phone : contact.phoneNumbers) {
            std::cout << phone << " ";
        }
        std::cout << "\n";
    }
}

bool ContactList::isUnique(const Contact& newContact) const {
    return std::none_of(contacts.begin(), contacts.end(), [&newContact](const Contact& contact) {
        return contact == newContact;
        });
}