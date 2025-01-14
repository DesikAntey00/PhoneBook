#pragma once

#include <QtWidgets/QMainWindow>
#include "ContactList2.h"
#include "ContactDialog2.h"
#include <QMainWindow>
#include <QTableView>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QHeaderView>
#include <QFileDialog>
#include <QPainter>
#include <QStyledItemDelegate>
#include <QMenu>
#include <QVector>

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    QTableWidget* tableWidget;
    ContactList ContactList;
    QLineEdit* searchLineEdit;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
     void updateTable();
    void updateList();
    bool isUnique(const Contact& newContact, int exception = -1);
    int findIndex(const Contact& newContact);
    Contact getContactFromRow(int row) const;
    QVector<Contact> getContactsFromTable() const;
    void updateTableFromContacts(const QVector<Contact>& contacts);
    void setRowData(int row, const Contact& contact);
    bool contactMatchesSearch(const Contact& contact, const QString& searchText) const;
    void viewDatabaseContent();

private slots:
    void addContact();
    void editContact();
    void deleteContact();
    void sortByColumnAscending();
    void sortByColumnDescending();
    void searchContacts();
    void saveContactsToFile();
    void loadContactsFromFile();
    void saveContactsToDatabase();
    void loadContactsFromDatabase();
    void sortByDate(Qt::SortOrder order);
    void sortBySalary(Qt::SortOrder order);
    void showColumnContextMenu(const QPoint& pos);
};

