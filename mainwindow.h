#pragma once

#include <QtWidgets/QMainWindow>
#include "ContactList.h"
#include "ContactDialog.h"
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

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    QTableWidget* tableWidget;
    ContactList ContactList;
    QLineEdit* searchLineEdit;
public:
    MainWindow(QWidget *parent = nullptr);
    void updateTable();
    ~MainWindow();
    void showColumnContextMenu(const QPoint& pos);
    void updateList();
    bool isUnique(const Contact& newContact, int exception = -1);
    int findIndex(const Contact& newContact);
private slots:
    void addContact();
    void editContact();
    void deleteContact();
    void sortByColumnAscending();
    void sortByColumnDescending();
    void searchContacts();
    void saveContactsToFile();
    void loadContactsFromFile();
    void sortByDate(Qt::SortOrder order);
    void sortBySalary(Qt::SortOrder order);
};

class SalaryDelegate : public QStyledItemDelegate {
public:
    SalaryDelegate(QObject* parent = nullptr) : QStyledItemDelegate(parent) {}

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override {
        int salary = index.data(Qt::EditRole).toInt();
        int maxSalary = 200000;
        int percentage = qMin(((salary * 100) / maxSalary), 100);
        painter->save();
        painter->fillRect(option.rect, option.backgroundBrush);
        QRect filledRect = option.rect;
        filledRect.setWidth(filledRect.width() * percentage / 100);
        painter->fillRect(filledRect, Qt::yellow);
        QString text = QString::number(salary);
        int textWidth = painter->fontMetrics().horizontalAdvance(text);
        int textX = option.rect.x() + (option.rect.width() - textWidth) / 2;
        int textY = option.rect.y() + (option.rect.height() + painter->fontMetrics().height()) / 2;
        for (int i = 0; i < text.length(); ++i) {
            QChar ch = text[i];
            QColor textColor = filledRect.contains(textX, textY) ? Qt::black : Qt::white;
            painter->setPen(textColor);
            painter->drawText(textX, textY, QString(ch));
            textX += painter->fontMetrics().horizontalAdvance(ch);
        }
        painter->restore();
    }
};
