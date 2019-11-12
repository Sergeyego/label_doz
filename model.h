#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>
#include <QDate>
#include <QLocale>

class ModelDoz : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit ModelDoz(QObject *parent = 0);
    void refresh(QDate beg, QDate end);
    QVariant data(const QModelIndex &item, int role=Qt::DisplayRole) const;

signals:

public slots:
};

class ModelEl : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit ModelEl(QObject *parent = 0);
public slots:
    void refresh();
};

class ModelRcp : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit ModelRcp(QObject *parent = 0);
public slots:
    void refresh();
};

class ModelRab : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit ModelRab(int id_rab, QObject *parent = 0);
private:
    int id;
public slots:
    void refresh();
};

#endif // MODEL_H
