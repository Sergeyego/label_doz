#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "model.h"
#include "lblengine.h"

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

private:
    Ui::MainWidget *ui;
    ModelDoz *modelDoz;
    ModelEl *modelEl;
    ModelRcp *modelRcp;
    ModelRab *modelRabDoz;
    ModelRab *modelRabTech;

private slots:
    void refreshDoz();
    void createLbl();
};

#endif // MAINWIDGET_H
