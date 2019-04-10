#include "serialconnectdialog.h"
#include "ui_serialconnectdialog.h"

SerialConnectDialog::SerialConnectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SerialConnectDialog)
{
    ui->setupUi(this);
    m_timer = new QTimer();
    QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    m_timer->start(1000);
}

SerialConnectDialog::~SerialConnectDialog()
{
    delete ui;
}


void SerialConnectDialog::onTimeout()
{
    if (portList.size() != info.availablePorts().size()) {
           foreach(QSerialPortInfo item, info.availablePorts()) {
               portStringList << item.portName();
           }
           portList = info.availablePorts();
           portListChanged(portStringList);
       }
       portStringList.clear();

}

void SerialConnectDialog::portListChanged(QStringList portList)
{
    ui->sensorComboBox->clear();
    ui->brakeComboBox->clear();

    ui->sensorComboBox->addItems(portList);
    ui->brakeComboBox->addItems(portList);
}

void SerialConnectDialog::on_sensorConnectButton_released()
{
    emit openSensorSerial(ui->sensorComboBox->currentText());
}

void SerialConnectDialog::on_brakeConnectButton_released()
{
    emit openBrakeDriverSerial(ui->brakeComboBox->currentText());
}

void SerialConnectDialog::on_buttonBox_accepted()
{
    emit openBrakeDriverSerial(ui->brakeComboBox->currentText());
    emit openSensorSerial(ui->sensorComboBox->currentText());
}
