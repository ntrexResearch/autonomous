#include "serialmonitordialog.h"
#include "ui_serialmonitordialog.h"

SerialMonitorDialog::SerialMonitorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SerialMonitorDialog)
{
    ui->setupUi(this);
    ui->brakeRxTextDisplay->setMaximumBlockCount(2000);
}

SerialMonitorDialog::~SerialMonitorDialog()
{
    delete ui;
}

void SerialMonitorDialog::on_brakeSendButton_released()
{
    QString txMsg = ui->brakeTxLineEdit->text();
    txMsg.append("\r\n");
    emit addBrakeTxMsg(txMsg);
}

void SerialMonitorDialog::on_brakeClearButton_released()
{
    ui->brakeRxTextDisplay->clear();
}


void SerialMonitorDialog::on_newBrakeRxMsg(QString msg)
{
    msg.remove(-2, 2);
    //qDebug() <<msg;
    ui->brakeRxTextDisplay->appendPlainText(msg);
}
