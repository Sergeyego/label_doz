#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QWidget>
#include <QTableView>
#include <QHeaderView>
#include <QCalendarWidget>
#include <QDateEdit>
#include <QComboBox>
#include <QCompleter>

class TableView : public QTableView
{
    Q_OBJECT
public:
    TableView(QWidget *parent=0);
public slots:
    void resizeToContents();
};

class DateEdit : public QDateEdit{
    Q_OBJECT
public:
    DateEdit(QWidget *parent=0);
};

class ComboBox : public QComboBox{
    Q_OBJECT
public:
    ComboBox(QWidget *parent=0);
};

#endif // TABLEVIEW_H
