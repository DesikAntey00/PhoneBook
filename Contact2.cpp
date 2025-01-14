#include "Contact2.h"
bool Contact::isValid() const {
    bool valid = true;
    QString fullName = firstName.trimmed() + " " + lastName.trimmed() + " " + fatherName.trimmed();

    if (!isValidName(fullName)) {
        qDebug() << "Invalid full name:" << fullName;
        valid = false;
    }
    if (!isValidEmail(email)) {
        qDebug() << "Invalid email:" << email;
        valid = false;
    }
    if (!isValidBirthDate(birthDate)) {
        qDebug() << "Invalid birth date:" << birthDate.toString("dd.MM.yyyy");
        valid = false;
    }

    return valid;
}

bool Contact::isValidName(const QString& name) const {
    qDebug() << "isValidName: " << name;
    QRegularExpression expr(R"(^([A-Z][a-z0-9]*(?:[- ][A-Z][a-z0-9]*)*)*$)");
    if (!expr.isValid()) {
        qDebug() << "Invalid regex pattern:" << expr.errorString();
    }
    return expr.match(name.trimmed()).hasMatch();
}

bool Contact::isValidEmail(const QString& email) const {
    qDebug() << "isValidEmail: " << email;
    QRegularExpression expr(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    return expr.match(email.trimmed()).hasMatch();
}

bool Contact::isValidBirthDate(const QDate& date) const {
    qDebug() << "isValidBirthDate: " << date;
    return date < QDate::currentDate();
}

bool Contact::operator==(const Contact& other) const {
    return firstName == other.firstName and
           lastName == other.lastName and
           fatherName == other.fatherName and
           address == other.address and
           birthDate == other.birthDate and
           email == other.email and
           phoneNumbers == other.phoneNumbers and
           salary == other.salary;
}
