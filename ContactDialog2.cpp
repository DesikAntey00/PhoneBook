#include "ContactDialog2.h"
ContactDialog::ContactDialog(QWidget* parent) : QDialog(parent) {
    QVBoxLayout* layout = new QVBoxLayout(this);

    firstNameEdit = new QLineEdit(this);
    lastNameEdit = new QLineEdit(this);
    fatherNameEdit = new QLineEdit(this);
    addressEdit = new QLineEdit(this);
    birthDateEdit = new QDateEdit(this);
    emailEdit = new QLineEdit(this);
    phoneEdit = new QLineEdit(this);
    salaryEdit = new QLineEdit(this);

    firstNameEdit->setPlaceholderText("First Name");
    lastNameEdit->setPlaceholderText("Last Name");
    fatherNameEdit->setPlaceholderText("Father Name");
    addressEdit->setPlaceholderText("Address");
    emailEdit->setPlaceholderText("Email");
    phoneEdit->setPlaceholderText("Phone Numbers");
    salaryEdit->setPlaceholderText("Salary");

    firstNameEdit->setStyleSheet("color: white;");
    lastNameEdit->setStyleSheet("color: white;");
    fatherNameEdit->setStyleSheet("color: white;");
    addressEdit->setStyleSheet("color: white;");
    emailEdit->setStyleSheet("color: white;");
    phoneEdit->setStyleSheet("color: white;");
    birthDateEdit->setStyleSheet("color: white;");
    salaryEdit->setStyleSheet("color: white;");

    layout->addWidget(firstNameEdit);
    layout->addWidget(lastNameEdit);
    layout->addWidget(fatherNameEdit);
    layout->addWidget(addressEdit);
    layout->addWidget(birthDateEdit);
    layout->addWidget(emailEdit);
    layout->addWidget(phoneEdit);
    layout->addWidget(salaryEdit);

    QPushButton* okButton = new QPushButton("OK", this);
    layout->addWidget(okButton);
    connect(okButton, &QPushButton::clicked, this, &ContactDialog::accept);

    setLayout(layout);
}

Contact ContactDialog::getContact() const {
    Contact newContact;
    newContact.firstName = firstNameEdit->text();
    newContact.lastName = lastNameEdit->text();
    newContact.fatherName = fatherNameEdit->text();
    newContact.address = addressEdit->text();
    newContact.birthDate = birthDateEdit->date();
    newContact.email = emailEdit->text();
    newContact.phoneNumbers << phoneEdit->text();
    newContact.salary = salaryEdit->text().toInt();
    qDebug() << "Getting contact from ContactDialog:" << newContact.salary << ". Expected: " << salaryEdit->text();
    return newContact;
}

void ContactDialog::setContact(const Contact& c) {
    firstNameEdit->setText(c.firstName);
    lastNameEdit->setText(c.lastName);
    fatherNameEdit->setText(c.fatherName);
    addressEdit->setText(c.address);
    birthDateEdit->setDate(c.birthDate);
    emailEdit->setText(c.email);
    phoneEdit->setText(c.phoneNumbers.join(", "));
    salaryEdit->setText(QString::number(c.salary));
}
