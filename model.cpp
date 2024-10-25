#include "model.h"

ModelDoz::ModelDoz(QObject *parent) : QSqlQueryModel(parent)
{

}

void ModelDoz::refresh(QDate beg, QDate end)
{
    QSqlQuery query;
    query.prepare("select d.dat, d.tm, e.marka, n.nam, d.kvo_tot "
                  "from dosage as d "
                  "inner join rcp_nam as n on d.id_rcp=n.id "
                  "inner join elrtr as e on n.id_el=e.id "
                  "where d.dat between :d1 and :d2 "
                  "order by d.dat, d.tm");
    query.bindValue(":d1",beg);
    query.bindValue(":d2",end);
    if (query.exec()){
        this->setQuery(query);
        setHeaderData(0,Qt::Horizontal,tr("Дата"));
        setHeaderData(1,Qt::Horizontal,tr("Время"));
        setHeaderData(2,Qt::Horizontal,tr("Марка"));
        setHeaderData(3,Qt::Horizontal,tr("Рецептура"));
        setHeaderData(4,Qt::Horizontal,tr("Отдозировано, кг"));
    } else {
        QMessageBox::critical(NULL,tr("Ошибка"),query.lastError().text(),QMessageBox::Ok);
    }
}

QVariant ModelDoz::data(const QModelIndex &item, int role) const
{
    if (role==Qt::DisplayRole){
        if (item.column()==4){
            return QLocale().toString(QSqlQueryModel::data(item,Qt::EditRole).toDouble(),'f',2);
        }
    }
    if (role==Qt::TextAlignmentRole){
        return item.column()==4 ? int(Qt::AlignRight | Qt::AlignVCenter) : int(Qt::AlignLeft | Qt::AlignVCenter);
    }
    return QSqlQueryModel::data(item,role);
}

ModelEl::ModelEl(QObject *parent) : QSqlQueryModel(parent)
{
    refresh();
}

void ModelEl::refresh()
{
    setQuery("select marka from elrtr where id<>0 order by marka");
    if (lastError().isValid()){
        QMessageBox::critical(NULL,tr("Ошибка"),lastError().text(),QMessageBox::Ok);
    }
}

ModelRcp::ModelRcp(QObject *parent) : QSqlQueryModel(parent)
{
    refresh();
}

void ModelRcp::refresh()
{
    setQuery("select nam from rcp_nam where lev=1 order by nam");
    if (lastError().isValid()){
        QMessageBox::critical(NULL,tr("Ошибка"),lastError().text(),QMessageBox::Ok);
    }
}

ModelRab::ModelRab(QObject *parent) : QSqlQueryModel(parent)
{

}

void ModelRab::refresh(QDate date, QString prof)
{
    QSqlQuery query;
    query.prepare("select distinct ke.snam from kamin_get_empl(:d,:d) as ge "
                  "inner join kamin_empl ke on ke.id = ge.id_empl "
                  "inner join kamin_job kj on kj.id = ge.id_job "
                  "where kj.nam ilike('%"+prof+"%') "
                  "order by ke.snam");
    query.bindValue(":d",date);
    if (query.exec()){
        setQuery(query);
    } else {
        QMessageBox::critical(NULL,tr("Ошибка"),query.lastError().text(),QMessageBox::Ok);
    }
}

