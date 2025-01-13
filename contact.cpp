#include "contact.h"

bool Contact::isValid() const {
    return isValidName(firstName) && isValidName(lastName) && isValidName(fatherName) &&
        isValidEmail(email) && isValidBirthDate(birthDate) &&
        std::all_of(phoneNumbers.begin(), phoneNumbers.end(), [this](const std::string& phone) {
        return isValidPhoneNumber(phone);
            });
}

bool Contact::isValidName(const std::string& name) const {
    std::regex nameRegex(R"(^[A-Za-z][A-Za-z0-9- ]*[A-Za-z0-9]$)");
    return std::regex_match(name, nameRegex);
}

bool Contact::isValidEmail(const std::string& email) const {
    std::regex emailRegex(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    return std::regex_match(email, emailRegex);
}

bool Contact::isValidBirthDate(const std::tm& date) const {
    std::time_t now = std::time(nullptr);
    std::tm currentDate;
    localtime_s(&currentDate, &now);
    return std::mktime(const_cast<std::tm*>(&date)) < std::mktime(&currentDate);
}

bool Contact::isValidPhoneNumber(const std::string& phoneNumber) const {
    std::regex phoneRegex(R"(^(\+7|8)?[\s\-]?\(?[489][0-9]{2}\)?[\s\-]?[0-9]{3}[\s\-]?[0-9]{2}[\s\-]?[0-9]{2}$)");
    return std::regex_match(phoneNumber, phoneRegex);
}

bool Contact::operator==(const Contact& other) const {
    return firstName == other.firstName &&
        lastName == other.lastName &&
        fatherName == other.fatherName &&
        address == other.address &&
        std::mktime(const_cast<std::tm*>(&birthDate)) == std::mktime(const_cast<std::tm*>(&other.birthDate)) &&
        email == other.email &&
        phoneNumbers == other.phoneNumbers &&
        salary == other.salary;
}
