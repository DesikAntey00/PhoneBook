#include <iostream>
#include "ContactList.h"

void printMenu() {
    std::cout << "1. Add Contact\n";
    std::cout << "2. Edit Contact\n";
    std::cout << "3. Delete Contact\n";
    std::cout << "4. Save Contacts to File\n";
    std::cout << "5. Load Contacts from File\n";
    std::cout << "6. Sort Contacts\n";
    std::cout << "7. Search Contacts\n";
    std::cout << "8. Exit\n";
}

void addContact(ContactList& contactList) {
    Contact contact;
    std::cout << "Enter First Name: ";
    std::getline(std::cin, contact.firstName);
    if (!contact.isValidName(contact.firstName)) {
        std::cout << "Invalid First Name. Please enter a valid name.\n";
        return;
    }
    std::cout << "Enter Last Name: ";
    std::getline(std::cin, contact.lastName);
    if (!contact.isValidName(contact.lastName)) {
        std::cout << "Invalid Last Name. Please enter a valid name.\n";
        return;
    }
    std::cout << "Enter Father Name: ";
    std::getline(std::cin, contact.fatherName);
    if (!contact.isValidName(contact.fatherName)) {
        std::cout << "Invalid Father Name. Please enter a valid name.\n";
        return;
    }
    std::cout << "Enter Address: ";
    std::getline(std::cin, contact.address);
    std::cout << "Enter Birth Date (dd.mm.yyyy): ";
    std::string birthDateStr;
    std::getline(std::cin, birthDateStr);
    std::istringstream dateStream(birthDateStr);
    if (!(dateStream >> std::get_time(&contact.birthDate, "%d.%m.%Y")) || !contact.isValidBirthDate(contact.birthDate)) {
        std::cout << "Invalid Birth Date. Please enter a valid date.\n";
        return;
    }
    std::cout << "Enter Email: ";
    std::getline(std::cin, contact.email);
    if (!contact.isValidEmail(contact.email)) {
        std::cout << "Invalid Email. Please enter a valid email.\n";
        return;
    }
    std::cout << "Enter Salary: ";
    std::cin >> contact.salary;
    std::cin.ignore();
    std::cout << "Enter Phone Numbers (separated by semicolon): ";
    std::string phoneNumbersStr;
    std::getline(std::cin, phoneNumbersStr);
    std::istringstream iss(phoneNumbersStr);
    std::string phone;
    while (std::getline(iss, phone, ';')) {
        if (!contact.isValidPhoneNumber(phone)) {
            std::cout << "Invalid Phone Number: " << phone << ". Please enter valid phone numbers.\n";
            return;
        }
        contact.phoneNumbers.push_back(phone);
    }
    contactList.addContact(contact);
    contactList.printContacts();
}

void editContact(ContactList& contactList) {
    int index;
    std::cout << "Enter index of contact to edit: ";
    std::cin >> index;
    std::cin.ignore();
    if (index >= 0 && index < contactList.getContacts().size()) {
        Contact contact = contactList.getContact(index);
        int fieldChoice;
        std::string birthDateStr;
        std::istringstream dateStream;
        std::string phoneNumbersStr;
        std::istringstream iss;
        std::string phone;

        do {
            std::cout << "Choose field to edit:\n";
            std::cout << "1. First Name\n";
            std::cout << "2. Last Name\n";
            std::cout << "3. Father Name\n";
            std::cout << "4. Address\n";
            std::cout << "5. Birth Date\n";
            std::cout << "6. Email\n";
            std::cout << "7. Salary\n";
            std::cout << "8. Phone Numbers\n";
            std::cout << "9. Save and Exit\n";
            std::cout << "Enter your choice: ";
            std::cin >> fieldChoice;
            std::cin.ignore();

            switch (fieldChoice) {
                case 1:
                    std::cout << "Enter new First Name: ";
                    std::getline(std::cin, contact.firstName);
                    if (!contact.isValidName(contact.firstName)) {
                        std::cout << "Invalid First Name. Please enter a valid name.\n";
                    }
                    break;
                case 2:
                    std::cout << "Enter new Last Name: ";
                    std::getline(std::cin, contact.lastName);
                    if (!contact.isValidName(contact.lastName)) {
                        std::cout << "Invalid Last Name. Please enter a valid name.\n";
                    }
                    break;
                case 3:
                    std::cout << "Enter new Father Name: ";
                    std::getline(std::cin, contact.fatherName);
                    if (!contact.isValidName(contact.fatherName)) {
                        std::cout << "Invalid Father Name. Please enter a valid name.\n";
                    }
                    break;
                case 4:
                    std::cout << "Enter new Address: ";
                    std::getline(std::cin, contact.address);
                    break;
                case 5:
                    std::cout << "Enter new Birth Date (dd.mm.yyyy): ";
                    std::getline(std::cin, birthDateStr);
                    dateStream.clear();
                    dateStream.str(birthDateStr);
                    if (!(dateStream >> std::get_time(&contact.birthDate, "%d.%m.%Y")) || !contact.isValidBirthDate(contact.birthDate)) {
                        std::cout << "Invalid Birth Date. Please enter a valid date.\n";
                    }
                    break;
                case 6:
                    std::cout << "Enter new Email: ";
                    std::getline(std::cin, contact.email);
                    if (!contact.isValidEmail(contact.email)) {
                        std::cout << "Invalid Email. Please enter a valid email.\n";
                    }
                    break;
                case 7:
                    std::cout << "Enter new Salary: ";
                    std::cin >> contact.salary;
                    std::cin.ignore();
                    break;
                case 8:
                    std::cout << "Enter new Phone Numbers (separated by semicolon): ";
                    std::getline(std::cin, phoneNumbersStr);
                    iss.clear();
                    iss.str(phoneNumbersStr);
                    contact.phoneNumbers.clear();
                    while (std::getline(iss, phone, ';')) {
                        if (!contact.isValidPhoneNumber(phone)) {
                            std::cout << "Invalid Phone Number: " << phone << ". Please enter valid phone numbers.\n";
                            return;
                        }
                        contact.phoneNumbers.push_back(phone);
                    }
                    break;
                case 9:
                    contactList.editContact(index, contact);
                    break;
                default:
                    std::cout << "Invalid choice\n";
            }
        } while (fieldChoice != 9);
    } else {
        std::cout << "Invalid index\n";
    }
    contactList.printContacts();
}

void deleteContact(ContactList& contactList) {
    int index;
    std::cout << "Enter index of contact to delete: ";
    std::cin >> index;
    std::cin.ignore();
    if (index >= 0 && index < contactList.getContacts().size()) {
        contactList.removeContact(index);
    } else {
        std::cout << "Invalid index\n";
    }
    contactList.printContacts();
}

void saveContactsToFile(ContactList& contactList) {
    std::string filename;
    std::cout << "Enter filename to save contacts: ";
    std::getline(std::cin, filename);
    contactList.saveToFile(filename);
    contactList.printContacts();
}

void loadContactsFromFile(ContactList& contactList) {
    std::string filename;
    std::cout << "Enter filename to load contacts: ";
    std::getline(std::cin, filename);
    contactList.loadFromFile(filename);
    contactList.printContacts();
}

void sortContacts(ContactList& contactList) {
    std::string field;
    std::cout << "Enter field to sort by (lastName, birthDate, age, salary): ";
    std::getline(std::cin, field);
    contactList.sortByField(field);
    contactList.printContacts();
}

void searchContacts(ContactList& contactList) {
    std::string query;
    std::cout << "Enter search query: ";
    std::getline(std::cin, query);

    std::vector<Contact> results;
    std::tm searchDate = {};
    bool isDateSearch = false;

    // Check if the query is a date
    std::istringstream dateStream(query);
    if (dateStream >> std::get_time(&searchDate, "%d.%m.%Y")) {
        isDateSearch = true;
    }

    if (isDateSearch) {
        for (const auto& contact : contactList.getContacts()) {
            if (std::mktime(const_cast<std::tm*>(&contact.birthDate)) == std::mktime(const_cast<std::tm*>(&searchDate))) {
                results.push_back(contact);
            }
        }
    } else {
        results = contactList.searchContacts(query);
    }

    std::cout << "Search Results:\n";
    for (const auto& contact : results) {
        std::cout << contact.firstName << " " << contact.lastName << " " << contact.fatherName << " "
                  << contact.address << " " << std::put_time(&contact.birthDate, "%d.%m.%Y") << " "
                  << contact.email << " " << contact.salary << " ";
        for (const auto& phone : contact.phoneNumbers) {
            std::cout << phone << " ";
        }
        std::cout << "\n";
    }
    contactList.printContacts();
}

int main() {
    ContactList contactList;
    int choice;
    do {
        printMenu();
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cin.ignore();
        switch (choice) {
            case 1:
                addContact(contactList);
                break;
            case 2:
                editContact(contactList);
                break;
            case 3:
                deleteContact(contactList);
                break;
            case 4:
                saveContactsToFile(contactList);
                break;
            case 5:
                loadContactsFromFile(contactList);
                break;
            case 6:
                sortContacts(contactList);
                break;
            case 7:
                searchContacts(contactList);
                break;
            case 8:
                std::cout << "Exiting...\n";
                break;
            default:
                std::cout << "Invalid choice\n";
        }
    } while (choice != 8);
    return 0;
}
