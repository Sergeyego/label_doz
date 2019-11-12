#include "mainwidget.h"
#include <QApplication>
#include "pgdialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    PgDialog d(QObject::tr("Этикетки для кюбелей"));
    if (d.exec()!=QDialog::Accepted) exit(1);

    MainWidget w;
    w.show();

    return a.exec();
}
