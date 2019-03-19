#ifndef MANUALCONTROLDIALOG_H
#define MANUALCONTROLDIALOG_H

#include <QDialog>
#include "speedcontroller.h"

namespace Ui {
class ManualControlDialog;
}

class ManualControlDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ManualControlDialog(QWidget *parent = nullptr);
    ~ManualControlDialog();

private slots:
    void on_applyPushButton_released();


    void on_SteerDial_sliderMoved(int position);


    void on_SpeedDial_sliderMoved(int position);


protected:
    void keyPressEvent(QKeyEvent* e) override;

private:
    float accelStep = 1;
    //float currentSpeed = 0;

    Ui::ManualControlDialog *ui;
};

#endif // MANUALCONTROLDIALOG_H
