#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCanBusDevice> // CAN Bus Error
#include <QMainWindow>
#include <engine.h>
#include <QStateMachine>

class ManualControlDialog;
class CanConnectDialog;
class CanMonitorDialog;
class SerialConnectDialog;
class SerialMonitorDialog;

QT_BEGIN_NAMESPACE

class QCanBusFrame;
class QLabel;

namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void updateCanConnectionState(bool);

    void on_AutoActivateButton_released();

    void on_ManualActivateButton_released();

    void manualModeActivated();

    void manualModeDeactivated();

    void on_OffButton_released();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    void initialize();

    Ui::MainWindow *m_ui = nullptr;

    CanConnectDialog *m_canConnectDialog = nullptr;
    CanMonitorDialog *m_canMonitorDialog = nullptr;
    ManualControlDialog *m_manualControlDialog = nullptr;
    SerialConnectDialog *m_serialConnectDialog = nullptr;
    SerialMonitorDialog *m_serialMonitorDialog = nullptr;
    Engine *engine;


    QStateMachine *m_system_machine;
};

#endif // MAINWINDOW_H
