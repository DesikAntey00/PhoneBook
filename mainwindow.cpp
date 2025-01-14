#include "MainWindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setStyleSheet("background-color: #1E796A");
    resize(1000, 700);

    QVBoxLayout* mainLayout = new QVBoxLayout;

    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(8);
    tableWidget->setHorizontalHeaderLabels({ "First Name", "Last Name", "Father Name", "Address", "Birth Date", "Email", "Phone Numbers", "Salary" });
    tableWidget->setStyleSheet(
        "QTableWidget { color: white; border: 1px solid #1E796A; } "
        "QHeaderView::section { background-color: #1E796A; color: white; padding: 5px; font-weight: bold; } "
        "QTableWidget::item { padding: 5px; } "
        "QTableWidget::item:selected { background-color: #166A55; color: white; }"
        );
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QHeaderView* header = tableWidget->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch);
    header->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(header, &QHeaderView::customContextMenuRequested, this, &MainWindow::showColumnContextMenu);
    connect(tableWidget, &QTableWidget::cellDoubleClicked, this, &MainWindow::editContact);

    mainLayout->addWidget(tableWidget);

    searchLineEdit = new QLineEdit(this);
    searchLineEdit->setPlaceholderText("Search...");
    searchLineEdit->setStyleSheet("color: white;");
    mainLayout->addWidget(searchLineEdit);
    connect(searchLineEdit, &QLineEdit::textChanged, this, &MainWindow::searchContacts);

    QHBoxLayout* buttonLayout = new QHBoxLayout;

    QVBoxLayout* leftLayout = new QVBoxLayout;
    QPushButton* addButton = new QPushButton("Add Contact", this);
    QPushButton* editButton = new QPushButton("Edit Contact", this);
    QPushButton* deleteButton = new QPushButton("Delete Contact", this);

    addButton->setStyleSheet("color: white;");
    editButton->setStyleSheet("color: white;");
    deleteButton->setStyleSheet("color: white;");

    leftLayout->addWidget(addButton);
    leftLayout->addWidget(editButton);
    leftLayout->addWidget(deleteButton);
    buttonLayout->addLayout(leftLayout);

    QVBoxLayout* rightLayout = new QVBoxLayout;
    QPushButton* saveButton = new QPushButton("Save Contacts", this);
    QPushButton* loadButton = new QPushButton("Load Contacts", this);
    QPushButton* saveToDbButton = new QPushButton("Save to Database", this);
    QPushButton* loadFromDbButton = new QPushButton("Load from Database", this);

    saveButton->setStyleSheet("color: white;");
    loadButton->setStyleSheet("color: white;");
    saveToDbButton->setStyleSheet("color: white;");
    loadFromDbButton->setStyleSheet("color: white;");

    rightLayout->addWidget(saveButton);
    rightLayout->addWidget(loadButton);
    rightLayout->addWidget(saveToDbButton);
    rightLayout->addWidget(loadFromDbButton);
    buttonLayout->addLayout(rightLayout);

    mainLayout->addLayout(buttonLayout);

    connect(addButton, &QPushButton::clicked, this, &MainWindow::addContact);
    connect(editButton, &QPushButton::clicked, this, &MainWindow::editContact);
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::deleteContact);
    connect(saveButton, &QPushButton::clicked, this, &MainWindow::saveContactsToFile);
    connect(loadButton, &QPushButton::clicked, this, &MainWindow::loadContactsFromFile);
    connect(saveToDbButton, &QPushButton::clicked, this, &MainWindow::saveContactsToDatabase);
    connect(loadFromDbButton, &QPushButton::clicked, this, &MainWindow::loadContactsFromDatabase);

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);
}

MainWindow::~MainWindow() {}

void MainWindow::addContact() {
    ContactDialog dialog(this);
    while (true) {
        if (dialog.exec() == QDialog::Accepted) {
            Contact newContact = dialog.getContact();
            if (newContact.isValid() and isUnique(newContact)) {
                ContactList.addContact(newContact);
                tableWidget->setRowCount(0);
                for (const Contact& contact : ContactList.getContacts()) {
                    int row = tableWidget->rowCount();
                    tableWidget->insertRow(row);
                    tableWidget->setItem(row, 0, new QTableWidgetItem(contact.firstName));
                    tableWidget->setItem(row, 1, new QTableWidgetItem(contact.lastName));
                    tableWidget->setItem(row, 2, new QTableWidgetItem(contact.fatherName));
                    tableWidget->setItem(row, 3, new QTableWidgetItem(contact.address));
                    tableWidget->setItem(row, 4, new QTableWidgetItem(contact.birthDate.toString("dd.MM.yyyy")));
                    tableWidget->setItem(row, 5, new QTableWidgetItem(contact.email));
                    QString phoneNumbersJoined = contact.phoneNumbers.join(", ");
                    tableWidget->setItem(row, 6, new QTableWidgetItem(phoneNumbersJoined));
                    tableWidget->setItem(row, 7, new QTableWidgetItem(QString::number(contact.salary)));
                }
                break;
            } else {
                if (not newContact.isValid()) {
                    QMessageBox::warning(this, "Invalid Input", "Input fields were incorrect. Please correct the errors.");
                } else if (not isUnique(newContact)) {
                    QMessageBox::warning(this, "Invalid Input", "Contact is not unique.");
                }
            }
        } else {
            break;
        }
    }
}

void MainWindow::editContact() {
    int currentRow = tableWidget->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "No Selection", "Please select a contact to edit.");
        return;
    }

    Contact contact;
    contact.firstName = tableWidget->item(currentRow, 0)->text();
    contact.lastName = tableWidget->item(currentRow, 1)->text();
    contact.fatherName = tableWidget->item(currentRow, 2)->text();
    contact.address = tableWidget->item(currentRow, 3)->text();
    contact.birthDate = QDate::fromString(tableWidget->item(currentRow, 4)->text(), "dd.MM.yyyy");
    contact.email = tableWidget->item(currentRow, 5)->text();
    contact.phoneNumbers = tableWidget->item(currentRow, 6)->text().split(", ", QString::SkipEmptyParts);
    contact.salary = tableWidget->item(currentRow, 7)->text().toInt();

    ContactDialog dialog(this);
    dialog.setContact(contact);
    if (dialog.exec() == QDialog::Accepted) {
        Contact updatedContact = dialog.getContact();
        if (updatedContact == contact) {
            return;
        }
        if (updatedContact.isValid() and isUnique(updatedContact)) {
            for (int i = 0; i < ContactList.getContacts().size(); ++i) {
                if (ContactList.getContact(i) == contact) {
                    ContactList.editContact(i, updatedContact);
                    updateTable();
                    return;
                }
            }
        } else {
            if (not updatedContact.isValid()) {
                QMessageBox::warning(this, "Invalid Input", "Input fields were incorrect. Please correct the errors.");
            } else if (not isUnique(updatedContact)) {
                QMessageBox::warning(this, "Invalid Input", "Contact is not unique.");
            }
        }
    }
}

void MainWindow::deleteContact() {
    int currentRow = tableWidget->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "No Selection", "Please select a contact to delete.");
        return;
    }

    Contact contact;
    contact.firstName = tableWidget->item(currentRow, 0)->text();
    contact.lastName = tableWidget->item(currentRow, 1)->text();
    contact.fatherName = tableWidget->item(currentRow, 2)->text();
    contact.address = tableWidget->item(currentRow, 3)->text();
    contact.birthDate = QDate::fromString(tableWidget->item(currentRow, 4)->text(), "dd.MM.yyyy");
    contact.email = tableWidget->item(currentRow, 5)->text();
    contact.phoneNumbers = tableWidget->item(currentRow, 6)->text().split(", ", QString::SkipEmptyParts);
    contact.salary = tableWidget->item(currentRow, 7)->text().toInt();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Delete Contact", "Are you sure you want to delete this contact?", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        for (int i = 0; i < ContactList.getContacts().size(); ++i) {
            if (ContactList.getContact(i) == contact) {
                ContactList.removeContact(i);
                updateTable();
                return;
            }
        }
    }
}

void MainWindow::searchContacts() {
    QString searchText = searchLineEdit->text().trimmed().toLower();
    tableWidget->setRowCount(0);

    for (const Contact& contact : ContactList.getContacts()) {
        if (contact.firstName.toLower().contains(searchText) or
            contact.lastName.toLower().contains(searchText) or
            contact.fatherName.toLower().contains(searchText) or
            contact.address.toLower().contains(searchText) or
            contact.email.toLower().contains(searchText) or
            contact.phoneNumbers.join(", ").toLower().contains(searchText) or
            contact.birthDate.toString("dd.MM.yyyy").contains(searchText) or
            QString::number(contact.salary).contains(searchText)) {

            int row = tableWidget->rowCount();
            tableWidget->insertRow(row);
            tableWidget->setItem(row, 0, new QTableWidgetItem(contact.firstName));
            tableWidget->setItem(row, 1, new QTableWidgetItem(contact.lastName));
            tableWidget->setItem(row, 2, new QTableWidgetItem(contact.fatherName));
            tableWidget->setItem(row, 3, new QTableWidgetItem(contact.address));
            tableWidget->setItem(row, 4, new QTableWidgetItem(contact.birthDate.toString("dd.MM.yyyy")));
            tableWidget->setItem(row, 5, new QTableWidgetItem(contact.email));
            QString phoneNumbersJoined = contact.phoneNumbers.join(", ");
            tableWidget->setItem(row, 6, new QTableWidgetItem(phoneNumbersJoined));
            tableWidget->setItem(row, 7, new QTableWidgetItem(QString::number(contact.salary)));
        }
    }
}

void MainWindow::updateTable() {
    tableWidget->setRowCount(0);
    for (const Contact& contact : ContactList.getContacts()) {
        int row = tableWidget->rowCount();
        tableWidget->insertRow(row);
        tableWidget->setItem(row, 0, new QTableWidgetItem(contact.firstName));
        tableWidget->setItem(row, 1, new QTableWidgetItem(contact.lastName));
        tableWidget->setItem(row, 2, new QTableWidgetItem(contact.fatherName));
        tableWidget->setItem(row, 3, new QTableWidgetItem(contact.address));
        tableWidget->setItem(row, 4, new QTableWidgetItem(contact.birthDate.toString("dd.MM.yyyy")));
        tableWidget->setItem(row, 5, new QTableWidgetItem(contact.email));
        QString phoneNumbersJoined = contact.phoneNumbers.join(", ");
        tableWidget->setItem(row, 6, new QTableWidgetItem(phoneNumbersJoined));
        tableWidget->setItem(row, 7, new QTableWidgetItem(QString::number(contact.salary)));
    }

}

void MainWindow::saveContactsToFile() {
    QString filename = QFileDialog::getSaveFileName(this, "Save Contacts", "", "Text Files (*.txt);;All Files (*)");
    if (!filename.isEmpty()) {
        QFile file(filename);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            int rowCount = tableWidget->rowCount();
            int columnCount = tableWidget->columnCount();
            for (int row = 0; row < rowCount; ++row) {
                QStringList rowData;
                for (int col = 0; col < columnCount; ++col) {
                    QTableWidgetItem* item = tableWidget->item(row, col);
                    if (col == 6) {
                        continue;
                    }
                    if (item) {
                        rowData << item->text();
                    } else {
                        rowData << "";
                    }
                }
                out << rowData.join(",") << "\n";
                QTableWidgetItem* phoneItem = tableWidget->item(row, 6);
                if (phoneItem) {
                    QString phoneText = phoneItem->text();
                    out << phoneText.replace(", ", ";") << "\n";
                }
            }
            file.close();
            QMessageBox::information(this, "Success", "Contacts saved successfully.");
        } else {
            QMessageBox::warning(this, "Error", "Failed to save the file.");
        }
    }
}

void MainWindow::loadContactsFromFile() {
    QString filename = QFileDialog::getOpenFileName(this, "Load Contacts", "", "Text Files (*.txt);;All Files (*)");
    if (!filename.isEmpty()) {
        ContactList.loadFromFile(filename);
        updateTable();
        QMessageBox::information(this, "Success", "Contacts loaded successfully.");
    }
}

void MainWindow::saveContactsToDatabase() {
    ContactList.saveToDatabase();
    QMessageBox::information(this, "Success", "Contacts saved to database successfully.");
}

void MainWindow::loadContactsFromDatabase() {
    // Создаем временный список контактов
    QVector<Contact> tempContacts = ContactList.getContacts().toVector();

    // Загружаем контакты из базы данных
    ContactList.loadFromDatabase();

    // Объединяем текущий список контактов с загруженными контактами
    for (const Contact& contact : tempContacts) {
        if (isUnique(contact)) {
            ContactList.addContact(contact);
        }
    }

    // Обновляем таблицу
    updateTable();

    QMessageBox::information(this, "Success", "Contacts loaded from database successfully.");
}


void MainWindow::showColumnContextMenu(const QPoint& pos) {
    QMenu contextMenu(tr("Context menu"), this);
    contextMenu.setStyleSheet("background-color: #188A6A;");

    QAction actionSortAsc("Sort Ascending", this);
    QAction actionSortDesc("Sort Descending", this);

    connect(&actionSortAsc, &QAction::triggered, this, &MainWindow::sortByColumnAscending);
    connect(&actionSortDesc, &QAction::triggered, this, &MainWindow::sortByColumnDescending);

    contextMenu.addAction(&actionSortAsc);
    contextMenu.addAction(&actionSortDesc);

    contextMenu.exec(tableWidget->viewport()->mapToGlobal(pos) - QPoint(0, 20));
}

void MainWindow::sortByColumnAscending() {
    int column = tableWidget->currentColumn();
    if (column >= 0) {
        if (column == 4) {
            sortByDate(Qt::AscendingOrder);
            return;
        } else if (column == 7) {
            sortBySalary(Qt::AscendingOrder);
            return;
        }
        tableWidget->sortItems(column, Qt::AscendingOrder);
    }
}

void MainWindow::sortByColumnDescending() {
    int column = tableWidget->currentColumn();
    if (column >= 0) {
        if (column == 4) {
            sortByDate(Qt::DescendingOrder);
            return;
        } else if (column == 7) {
            sortBySalary(Qt::DescendingOrder);
            return;
        }
        tableWidget->sortItems(column, Qt::DescendingOrder);
    }
}

void MainWindow::updateList() {
    ContactList.clear();

    for (int row = 0; row < tableWidget->rowCount(); ++row) {
        Contact contact;
        contact.firstName = tableWidget->item(row, 0)->text();
        contact.lastName = tableWidget->item(row, 1)->text();
        contact.fatherName = tableWidget->item(row, 2)->text();
        contact.address = tableWidget->item(row, 3)->text();
        contact.birthDate = QDate::fromString(tableWidget->item(row, 4)->text(), "dd.MM.yyyy");
        contact.email = tableWidget->item(row, 5)->text();
        contact.phoneNumbers = tableWidget->item(row, 6)->text().split(", ", QString::SkipEmptyParts);
        contact.salary = tableWidget->item(row, 7)->text().toInt();

        if (contact.isValid()) {
            ContactList.addContact(contact);
        } else {
            QMessageBox::warning(this, "Invalid Contact", "One or more contacts are invalid and will not be saved.");
        }
    }
}

bool MainWindow::isUnique(const Contact& newContact, int exception) {
    bool isUnique = 1;
    int i = 0;
    for (const Contact& cont : ContactList.getContacts()) {
        if (cont == newContact and i != exception) {
            isUnique = 0;
            break;
        }
        i++;
    }
    return isUnique;
}

int MainWindow::findIndex(const Contact& findContact) {
    int answer = -1;
    int i = 0;
    for (const Contact& cont : ContactList.getContacts()) {
        if (cont == findContact) {
            answer = i;
            break;
        }
        i++;
    }
    return answer;
}

void MainWindow::sortByDate(Qt::SortOrder order) {
    int rowCount = tableWidget->rowCount();
    QVector<Contact> contacts;

    for (int row = 0; row < rowCount; ++row) {
        Contact contact;
        contact.firstName = tableWidget->item(row, 0)->text();
        contact.lastName = tableWidget->item(row, 1)->text();
        contact.fatherName = tableWidget->item(row, 2)->text();
        contact.address = tableWidget->item(row, 3)->text();

        QTableWidgetItem* dateItem = tableWidget->item(row, 4);
        if (dateItem) {
            contact.birthDate = QDate::fromString(dateItem->text(), "dd.MM.yyyy");
        }

        contact.email = tableWidget->item(row, 5)->text();
        contact.phoneNumbers = tableWidget->item(row, 6)->text().split(", ", QString::SkipEmptyParts);
        contact.salary = tableWidget->item(row, 7)->text().toInt();
        contacts.append(contact);
    }

    std::sort(contacts.begin(), contacts.end(), [order](const Contact& a, const Contact& b) {
        if (order == Qt::AscendingOrder) {
            return a.birthDate < b.birthDate;
        } else {
            return a.birthDate > b.birthDate;
        }
    });

    tableWidget->clearContents();

    for (int row = 0; row < contacts.size(); ++row) {
        const Contact& contact = contacts[row];
        tableWidget->setItem(row, 0, new QTableWidgetItem(contact.firstName));
        tableWidget->setItem(row, 1, new QTableWidgetItem(contact.lastName));
        tableWidget->setItem(row, 2, new QTableWidgetItem(contact.fatherName));
        tableWidget->setItem(row, 3, new QTableWidgetItem(contact.address));
        tableWidget->setItem(row, 4, new QTableWidgetItem(contact.birthDate.toString("dd.MM.yyyy")));
        tableWidget->setItem(row, 5, new QTableWidgetItem(contact.email));
        tableWidget->setItem(row, 6, new QTableWidgetItem(contact.phoneNumbers.join(", ")));
        tableWidget->setItem(row, 7, new QTableWidgetItem(QString::number(contact.salary)));
    }
}

void MainWindow::sortBySalary(Qt::SortOrder order) {
    int rowCount = tableWidget->rowCount();
    QVector<Contact> contacts;

    for (int row = 0; row < rowCount; ++row) {
        Contact contact;
        contact.firstName = tableWidget->item(row, 0)->text();
        contact.lastName = tableWidget->item(row, 1)->text();
        contact.fatherName = tableWidget->item(row, 2)->text();
        contact.address = tableWidget->item(row, 3)->text();

        QTableWidgetItem* dateItem = tableWidget->item(row, 4);
        if (dateItem) {
            contact.birthDate = QDate::fromString(dateItem->text(), "dd.MM.yyyy");
        }

        contact.email = tableWidget->item(row, 5)->text();
        contact.phoneNumbers = tableWidget->item(row, 6)->text().split(", ", QString::SkipEmptyParts);
        QString salaryText = tableWidget->item(row, 7)->text().trimmed();
        qDebug() << "Salary text:" << salaryText;
        contact.salary = tableWidget->item(row, 7)->text().toInt();
        contacts.append(contact);
    }

    std::sort(contacts.begin(), contacts.end(), [order](const Contact& a, const Contact& b) {
        if (order == Qt::AscendingOrder) {
            qDebug() << "Comparing salaries:" << a.salary << " and " << b.salary << ". Result: " << (a.salary < b.salary);
            return a.salary < b.salary;
        } else {
            qDebug() << "Comparing salaries:" << a.salary << " and " << b.salary << ". Result: " << (a.salary > b.salary);
            return a.salary > b.salary;
        }
    });

    tableWidget->setRowCount(0);

    for (int row = 0; row < contacts.size(); ++row) {
        const Contact& contact = contacts[row];
        int newRow = tableWidget->rowCount();
        tableWidget->insertRow(newRow);

        tableWidget->setItem(newRow, 0, new QTableWidgetItem(contact.firstName));
        tableWidget->setItem(newRow, 1, new QTableWidgetItem(contact.lastName));
        tableWidget->setItem(newRow, 2, new QTableWidgetItem(contact.fatherName));
        tableWidget->setItem(newRow, 3, new QTableWidgetItem(contact.address));
        tableWidget->setItem(newRow, 4, new QTableWidgetItem(contact.birthDate.toString("dd.MM.yyyy")));
        tableWidget->setItem(newRow, 5, new QTableWidgetItem(contact.email));
        tableWidget->setItem(newRow, 6, new QTableWidgetItem(contact.phoneNumbers.join(", ")));
        tableWidget->setItem(newRow, 7, new QTableWidgetItem(QString::number(contact.salary)));
    }
}
