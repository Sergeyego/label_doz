#include "mainwidget.h"
#include "ui_mainwidget.h"

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    ui->dateEditBeg->setDate(QDate::currentDate().addDays(-QDate::currentDate().day()+1));
    ui->dateEditEnd->setDate(QDate::currentDate());
    ui->dateEditDoz->setDate(QDate::currentDate());    
    modelDoz = new ModelDoz(this);
    ui->tableViewDoz->setModel(modelDoz);
    refreshDoz();
    modelEl = new ModelEl(this);
    ui->comboBoxMar->setModel(modelEl);
    modelRcp = new ModelRcp(this);
    ui->comboBoxRcp->setModel(modelRcp);
    modelRabDoz = new ModelRab(5,this);
    ui->comboBoxRab->setModel(modelRabDoz);
    modelRabTech = new ModelRab(36,this);
    ui->comboBoxTech->setModel(modelRabTech);

    if (ui->tableViewDoz->model()->rowCount()){
        ui->tableViewDoz->selectRow(0);
    }

    connect(ui->cmdUpd,SIGNAL(clicked(bool)),this,SLOT(refreshDoz()));
    connect(ui->cmdGo,SIGNAL(clicked(bool)),this,SLOT(createLbl()));

    connect(ui->checkBoxNum,SIGNAL(clicked(bool)),ui->spinNum,SLOT(setEnabled(bool)));
    connect(ui->checkBoxNum,SIGNAL(clicked(bool)),ui->spinBoxLst,SLOT(setEnabled(bool)));
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::refreshDoz()
{
    modelDoz->refresh(ui->dateEditBeg->date(),ui->dateEditEnd->date());
    ui->tableViewDoz->resizeToContents();
}

void MainWidget::createLbl()
{
    LblEngine e;
    bool prnNum=ui->checkBoxNum->isChecked();
    QString str, dat, mark, rcp, man;
    if (ui->tabWidget->currentIndex()==0){
        int r=ui->tableViewDoz->currentIndex().row();
        dat=ui->tableViewDoz->model()->data(ui->tableViewDoz->model()->index(r,0),Qt::EditRole).toDate().toString("dd.MM.yy");
        mark=ui->tableViewDoz->model()->data(ui->tableViewDoz->model()->index(r,2),Qt::EditRole).toString();
        rcp=ui->tableViewDoz->model()->data(ui->tableViewDoz->model()->index(r,3),Qt::EditRole).toString();
    } else {
        dat=ui->dateEditDoz->date().toString("dd.MM.yy");
        mark=ui->comboBoxMar->currentText();
        rcp=ui->comboBoxRcp->currentText();
    }
    if (ui->checkBoxMan->isChecked()){
        man=QString::fromUtf8("Ручная дозировка!");
    }
    str+=QString::fromUtf8("Дата дозировки ");
    str+=dat+"\n";
    str+=QString::fromUtf8("Марка ");
    str+=mark+"\n";
    str+=QString::fromUtf8("Рецептура ");
    str+=rcp+"\n";
    str+=QString::fromUtf8("Масса шихты ");
    str+=ui->lineEditKvo->text()+QString::fromUtf8(" кг")+"\n";
    str+=QString::fromUtf8("Дозировщик ");
    str+=ui->comboBoxRab->currentText()+"\n";
    str+=QString::fromUtf8("Технолог ");
    str+=ui->comboBoxTech->currentText()+"\n";

    if (prnNum){
        QFile file("data.txt");
        if ( file.open( QIODevice::WriteOnly ) ) {
            QTextStream stream( &file );
            for(int i=0; i<6*ui->spinBoxLst->value(); i++){
                stream<<i+ui->spinNum->value()<<"\n";
            }
            file.close();
        }
    }

    e.createLblDoz(str,man,prnNum);
}
