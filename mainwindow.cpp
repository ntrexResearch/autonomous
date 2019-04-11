#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "canconnectdialog.h"
#include "canmonitordialog.h"
#include "manualcontroldialog.h"
#include "serialconnectdialog.h"
#include "serialmonitordialog.h"
#include "QLabel"

#include <QCanBus>
#include <QCanBusFrame>
#include <QCloseEvent>
#include <QDesktopServices>
#include <QStateMachine>
//
#include <QFinalState>
#include "StateMachine/states.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);
    //Main engine for statemachine
    initialize();

}

MainWindow::~MainWindow()
{

    delete m_canConnectDialog;
    delete m_canMonitorDialog;
    delete m_manualControlDialog;
    delete m_serialConnectDialog;
    delete m_serialMonitorDialog;
    delete m_ui;
    //~Engine();
    delete engine;
}


void MainWindow::initialize()
{
    this->setWindowTitle("NTREX AUTONOMOUS");

    engine = Engine::Instance(this);

    m_canConnectDialog = new CanConnectDialog;
    m_canMonitorDialog = new CanMonitorDialog;
    m_manualControlDialog = new ManualControlDialog;
    m_serialConnectDialog = new SerialConnectDialog;
    m_serialMonitorDialog = new SerialMonitorDialog;

    m_ui->action_ConnectCAN->setEnabled(true);
    m_ui->action_DisconnectCAN->setEnabled(false);
    m_ui->action_MonitorCAN->setEnabled(true);
    m_ui->action_ManualControl->setEnabled(true);


    connect(m_canMonitorDialog->getSendFrameBox(), &SendFrameBox::sendFrame, CanManager::Instance(), &CanManager::sendFrame);
    connect(m_canConnectDialog, &CanConnectDialog::connectCanDevice, CanManager::Instance(), &CanManager::connectCanDevice, Qt::DirectConnection);
    connect(m_ui->action_MonitorCAN, &QAction::triggered, CanManager::Instance(), &CanManager::monitorOn);
    connect(m_canMonitorDialog, &CanMonitorDialog::closed, CanManager::Instance(), &CanManager::monitorOff );
    connect(CanManager::Instance(), &CanManager::showReceivedFrames, m_canMonitorDialog, &CanMonitorDialog::showReceivedFrames);
    connect(CanManager::Instance(), &CanManager::notifyWrittenFrames, m_canMonitorDialog, &CanMonitorDialog::processWrittenFrames);
    connect(CanManager::Instance(), &CanManager::showStateMessage, m_canMonitorDialog, &CanMonitorDialog::showCanState);
    connect(m_ui->action_ConnectCAN, &QAction::triggered, m_canConnectDialog, &CanConnectDialog::show);
    connect(m_ui->action_DisconnectCAN, &QAction::triggered, CanManager::Instance(), &CanManager::disconnectCanDevice);
    connect(m_ui->action_MonitorCAN, &QAction::triggered, m_canMonitorDialog, &CanMonitorDialog::show );
    connect(m_ui->action_connectSerial, &QAction::triggered, m_serialConnectDialog, &SerialConnectDialog::show);
    connect(CanManager::Instance(), &CanManager::sendConnectionState, m_canMonitorDialog, &CanMonitorDialog::updateConnectionState);

    connect(CanManager::Instance(), &CanManager::sendConnectionState, this, &MainWindow::updateCanConnectionState);
    connect(m_ui->AutoActivateButton, &QPushButton::released, CanManager::Instance(), &CanManager::activateDCU);
    connect(m_ui->action_ManualControl, &QAction::triggered, m_manualControlDialog, &ManualControlDialog::show);


    connect(m_serialConnectDialog, &SerialConnectDialog::openSensorSerial, engine, &Engine::openSensorSerial);
    connect(m_serialConnectDialog, &SerialConnectDialog::openBrakeDriverSerial, engine, &Engine::openBrakeDriverSerial);

    connect(m_ui->action_SerialMonitor, &QAction::triggered, m_serialMonitorDialog, &SerialMonitorDialog::show);


    connect(m_serialMonitorDialog, &SerialMonitorDialog::addBrakeTxMsg, engine, &Engine::enqueueBrakeTxMsg);

    connect(engine->getSerialManager(), &SerialManager::showRxMsg, m_serialMonitorDialog, &SerialMonitorDialog::on_newBrakeRxMsg);

    m_ui->action_ConnectCAN->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_C));
    m_ui->action_MonitorCAN->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_M));
    m_ui->action_DisconnectCAN->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_D));
    m_ui->action_ManualControl->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_X));
    m_ui->action_connectSerial->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_S));
    m_ui->action_SerialMonitor->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_S | Qt::Key_M));
    m_system_machine = new QStateMachine(this);

    VehicleState *vehicleState = new VehicleState(CanManager::Instance(), m_system_machine);
    OffState *offState = new OffState(m_system_machine);

    offState->addTransition(CanManager::Instance(), SIGNAL(manualModeOn()), vehicleState);

//    offState->addTransition(m_canConnectDialog, &QDialog::accepted, vehicleState);

    // When the user turns the system off, make sure to disable the system by issuing manual command.
    vehicleState->addTransition(m_ui->action_DisconnectCAN, SIGNAL(triggered()), offState);

    //connect(CanManager::Instance(), &CanManager::manualModeOn, this, &MainWindow::manualModeActivated);
    //connect(vehicleState, &ManualState::manualModeOff, this, &MainWindow::manualModeDeactivated);

    m_system_machine->setInitialState(offState);
    m_system_machine->start();
    //connect(CanManager::Instance(), &CanManager::sendConnectionState, )

}

void MainWindow::manualModeDeactivated()
{
    m_ui->action_ManualControl->setEnabled(false);
    m_manualControlDialog->close();
    delete m_manualControlDialog;
}

void MainWindow::manualModeActivated()
{
    // Manual controller dialog is created when the manual mode is activated
    m_manualControlDialog = new ManualControlDialog;
    m_ui->action_ManualControl->setEnabled(true);
    connect(m_ui->action_ManualControl, &QAction::triggered, m_manualControlDialog, &ManualControlDialog::show);

}
void MainWindow::closeEvent(QCloseEvent *event)
{
    m_canMonitorDialog->close();
    m_canConnectDialog->close();
    if (m_manualControlDialog)
        m_manualControlDialog->close();
    event->accept();
}

void MainWindow::updateCanConnectionState(bool state)
{
    if (state) {
        m_ui->action_ConnectCAN->setEnabled(false);
        m_ui->action_DisconnectCAN->setEnabled(true);
    }
    else {
        m_ui->action_ConnectCAN->setEnabled(true);
        m_ui->action_DisconnectCAN->setEnabled(false);
//        m_ui->action_ManualControl->setEnabled(false);
//        m_manualControlDialog->close();
//        delete m_manualControlDialog;
        //m_ui->action_MonitorCAN->setEnabled(false);
    }
}

void MainWindow::on_AutoActivateButton_released()
{
    CanManager::Instance()->sendDcuCanMsg(DCU_AUTO_REQUEST);
}

void MainWindow::on_ManualActivateButton_released()
{
    CanManager::Instance()->sendDcuCanMsg(DCU_MANUAL_REQUEST);
}

void MainWindow::on_OffButton_released()
{
    CanManager::Instance()->sendDcuCanMsg(DCU_OFF_REQUEST);
}
