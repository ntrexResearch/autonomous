#include "manualcontroldialog.h"
#include "ui_manualcontroldialog.h"
#include <QMenu>
#include <QDoubleValidator>
#include <QDebug>
#include <QKeyEvent>

ManualControlDialog::ManualControlDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ManualControlDialog)
{
    ui->setupUi(this);

    this->setWindowTitle("Manual Controller Dialog");

    auto dv = new QDoubleValidator(0.0, 10.0, 2); // [0, 10.0] with 2 decimal
    dv->setNotation(QDoubleValidator::StandardNotation);
    ui->accelLineEdit->setValidator(dv);

//    QMenu *manualControlOption = new QMenu(this);
//    manualControlOption->addMenu(tr("&Option"));

//    QAction *accelAction = new QAction(this);
//    QAction *decelAction = new QAction(this);

//    accelAction->setShortcut(QKeySequence(Qt::Key_Up));
//    decelAction->setShortcut(QKeySequence(Qt::Key_Down));

//    connect(accelAction, SIGNAL(triggered()), this, SLOT(accelerate()));

//    manualControlOption->addAction(accelAction);
//    manualControlOption->addAction(decelAction);

    //ui->dial->setFocus(Qt::FocusReason::ShortcutFocusReason);
    ui->SpeedDial->setRange(-2000, 2000);
    ui->SteerDial->setRange(-100, 100);
    ui->SpeedDial->setWrapping(false);
    ui->SteerDial->setWrapping(false);
    //ui->dial->setRepeatAction();

}

ManualControlDialog::~ManualControlDialog()
{
    delete ui;
}

void ManualControlDialog::on_applyPushButton_released()
{
    accelStep = ui->accelLineEdit->text().toFloat();
    qDebug() << accelStep;
}

void ManualControlDialog::keyPressEvent(QKeyEvent *e)
{
    if (SpeedController::hasInstance()){
        if (e->key() == Qt::Key_Up){
            SpeedController::Instance()->incrementSpeed(accelStep);
        }
        else if (e->key() == Qt::Key_Down){
            SpeedController::Instance()->incrementSpeed(-accelStep);
        }
        else if (e->key() == Qt::Key_Tab || e->key() == Qt::Key_Space){
            SpeedController::Instance()->setCurrentSpeed(0);
        }

    }
}




void ManualControlDialog::on_SteerDial_sliderMoved(int position)
{
    qDebug() << position;

}

void ManualControlDialog::on_SpeedDial_sliderMoved(int position)
{
    qDebug() << position;

}

