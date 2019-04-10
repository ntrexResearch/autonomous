#ifndef SERIALMONITORDIALOG_H
#define SERIALMONITORDIALOG_H

#include <QDialog>

namespace Ui {
class SerialMonitorDialog;
}

class SerialMonitorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SerialMonitorDialog(QWidget *parent = nullptr);
    ~SerialMonitorDialog();

private:
    Ui::SerialMonitorDialog *ui;
};

#endif // SERIALMONITORDIALOG_H
