#include "serialmonitordialog.h"
#include "ui_serialmonitordialog.h"

SerialMonitorDialog::SerialMonitorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SerialMonitorDialog)
{
    ui->setupUi(this);
}

SerialMonitorDialog::~SerialMonitorDialog()
{
    delete ui;
}
