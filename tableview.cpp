#include "tableview.h"

TableView::TableView(QWidget *parent) : QTableView(parent)
{
    verticalHeader()->setDefaultSectionSize(verticalHeader()->fontMetrics().height()*1.5);
}

void TableView::resizeToContents()
{
    if (!model()) return;
    int n=model()->columnCount();
    int m=model()->rowCount();
    int max=0;
    QStringList l;
    QString s;
    /*for (int i=0; i<model()->rowCount(); i++){
        max=1;
        for (int j=0; j<n; j++){
            s=model()->data(model()->index(i,j)).toString();
            l=s.split("\n");
            if (max<l.size()) max=l.size();
        }
        setRowHeight(i,max*ui->tableView->fontMetrics().height()+12);
    }*/
    for (int i=0; i<n; i++){
        s=model()->headerData(i,Qt::Horizontal).toString();
        max=horizontalHeader()->fontMetrics().width(s);
        for (int j=0; j<m; j++){
            s=model()->data(model()->index(j,i)).toString();
            l=s.split("\n");
            for (int k=0; k<l.size(); k++){
                if (max<fontMetrics().width(l.at(k)))
                    max=fontMetrics().width(l.at(k));
            }
        }
        setColumnWidth(i,max+12);
    }
}


DateEdit::DateEdit(QWidget *parent) : QDateEdit(parent)
{
    this->setCalendarPopup(true);
    QCalendarWidget * pCW = new QCalendarWidget(this);
    pCW->setFirstDayOfWeek( Qt::Monday );
    this->setCalendarWidget( pCW );
}

ComboBox::ComboBox(QWidget *parent) : QComboBox(parent)
{
    this->setEditable(true);
    this->completer()->setCompletionMode(QCompleter::PopupCompletion);
    this->completer()->setCaseSensitivity(Qt::CaseInsensitive);
}
