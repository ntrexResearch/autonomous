#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCanBusDevice> // CAN Bus Error
#include <QMainWindow>
#include <engine.h>
#include "canthread.h"

class CanConnectDialog;
class CanMonitorDialog;

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


    void on_AutoActivateButton_released();

    void on_ManualActivateButton_released();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    void initActionsConnections();

    Ui::MainWindow *m_ui = nullptr;

    CanConnectDialog *m_canConnectDialog = nullptr;
    CanMonitorDialog *m_canMonitorDialog = nullptr;

    Engine *engine;
    CanThread *canThread;
};

#endif // MAINWINDOW_H
