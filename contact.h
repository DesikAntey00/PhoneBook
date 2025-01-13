#pragma once
#include <string>
#include <vector>
#include <regex>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>

class Contact {
public:
    std::string firstName;
    std::string lastName;
    std::string fatherName;
    std::string address;
    std::tm birthDate;
    std::string email;
    std::vector<std::string> phoneNumbers;
    int salary;

    bool isValid() const;
    bool operator==(const Contact& other) const;


    bool isValidName(const std::string& name) const;
    bool isValidEmail(const std::string& email) const;
    bool isValidBirthDate(const std::tm& date) const;
    bool isValidPhoneNumber(const std::string& phoneNumber) const;
};
