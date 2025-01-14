#pragma once
#include <QString>
#include <QRegularExpression>
#include <QDate>
#include <QDebug>

class Contact {
public:
    QString firstName;
    QString lastName;
    QString fatherName;
    QString address;
    QDate birthDate;
    QString email;
    QStringList phoneNumbers;
    int salary;

    bool isValid() const;
    bool operator==(const Contact& other) const;
private:
    bool isValidName(const QString& name) const;
    bool isValidEmail(const QString& email) const;
    bool isValidBirthDate(const QDate& date) const;
};
