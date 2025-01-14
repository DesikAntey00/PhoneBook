#include <QDialog>
#include <QLineEdit>
#include <QDateEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QCloseEvent>
#include "Contact.h"

class ContactDialog : public QDialog {
    Q_OBJECT

private:
    QLineEdit* firstNameEdit;
    QLineEdit* lastNameEdit;
    QLineEdit* fatherNameEdit;
    QLineEdit* addressEdit;
    QDateEdit* birthDateEdit;
    QLineEdit* emailEdit;
    QLineEdit* phoneEdit;
    QLineEdit* salaryEdit;

    Contact contact;
public:
    ContactDialog(QWidget* parent = nullptr);
    Contact getContact() const;
    void setContact(const Contact& contact);
};
