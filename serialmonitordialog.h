#ifndef SERIALMONITORDIALOG_H
#define SERIALMONITORDIALOG_H

#include <QDialog>
#include <QDebug>
namespace Ui {
class SerialMonitorDialog;
}

class SerialMonitorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SerialMonitorDialog(QWidget *parent = nullptr);
    ~SerialMonitorDialog();

signals:
    void addBrakeTxMsg(QString msg);

public slots:
    void on_newBrakeRxMsg(QString msg);

private slots:
    void on_brakeSendButton_released();

    void on_brakeClearButton_released();

private:
    Ui::SerialMonitorDialog *ui;
};

#endif // SERIALMONITORDIALOG_H
