#include "cancommwidget.h"
#include "ui_cancommwidget.h"

CanCommWidget::CanCommWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CanCommWidget)
{
    ui->setupUi(this);
}

CanCommWidget::~CanCommWidget()
{
    delete ui;
}
