#ifndef SERIALCONNECTDIALOG_H
#define SERIALCONNECTDIALOG_H

#include <QDialog>
#include <QTimer>
#include <QSerialPortInfo>
#include <QList>
#include <QStringList>

namespace Ui {
class SerialConnectDialog;
}

class SerialConnectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SerialConnectDialog(QWidget *parent = nullptr);
    ~SerialConnectDialog();

signals:
    void openBrakeDriverSerial(QString port);
    void openSensorSerial(QString port);

private slots:
    void onTimeout();

    void on_sensorConnectButton_released();
    
    void on_brakeConnectButton_released();
    
    void on_buttonBox_accepted();
    
private:
    QTimer* m_timer;
    Ui::SerialConnectDialog *ui;
    QSerialPortInfo info;
    QList<QSerialPortInfo> portList;
    QStringList portStringList;

    void portListChanged(QStringList portList);

};

#endif // SERIALCONNECTDIALOG_H
