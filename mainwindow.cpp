#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "canconnectdialog.h"
#include "canmonitordialog.h"

#include <QCanBus>
#include <QCanBusFrame>
#include <QCloseEvent>
#include <QDesktopServices>

#include "QLabel"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);
    //Main engine for statemachine
    engine = Engine::Instance(this);
    canThread = new CanThread();

    m_canConnectDialog = new CanConnectDialog;
    m_canMonitorDialog = new CanMonitorDialog;


    initActionsConnections();
    canThread->start();

}

MainWindow::~MainWindow()
{

    delete m_canConnectDialog;
    delete m_canMonitorDialog;

    delete m_ui;

    delete engine;
    delete canThread;
}


void MainWindow::initActionsConnections()
{
    m_ui->action_ConnectCAN->setEnabled(true);
    m_ui->action_DisconnectCAN->setEnabled(true);

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
    connect(CanManager::Instance(), &CanManager::sendConnectionState, m_canMonitorDialog, &CanMonitorDialog::updateConnectionState);
}


//void MainWindow::connectCanDevice()
//{
//    const CanConnectDialog::Settings p = m_canConnectDialog->settings();

//    QString errorString;
//    m_canDevice = QCanBus::instance()->createDevice("peakcan", p.deviceInterfaceName, &errorString);

//    if (!m_canDevice) {
//        m_status->setText(tr("Error creating device '%1', reason: '%2'")
//                          .arg("peakcan").arg(errorString));
//        return;
//    }

//    m_numberWrittenFrames = 0;

//    connect(m_canDevice, &QCanBusDevice::errorOccurred, this, &MainWindow::processErrors);
//    connect(m_canDevice, &QCanBusDevice::framesReceived, this, &MainWindow::processReceivedFrames);
//    connect(m_canDevice, &QCanBusDevice::framesWritten, this, &MainWindow::processWrittenFrames);

//    // Set the configuration
//    for (const CanConnectDialog::ConfigurationItem &item : p.configurations)
//        m_canDevice->setConfigurationParameter(item.first, item.second);

//    if (!m_canDevice->connectDevice()) {
//        m_status->setText(tr("Connection error: %1").arg(m_canDevice->errorString()));

//        delete m_canDevice;
//        m_canDevice = nullptr;
//    }
//    else {
//        m_ui->action_ConnectCAN->setEnabled(false);
//        m_ui->action_DisconnectCAN->setEnabled(true);
//        //Send frame box
//        m_ui->sendFrameBox->setEnabled(true);


//        QVariant bitRate = m_canDevice->configurationParameter(QCanBusDevice::BitRateKey);
//        if (bitRate.isValid()) {
//            m_status->setText(tr("Plugin: %1, connected to %2 at %3 kBit/s")
//                              .arg("peakcan").arg(p.deviceInterfaceName)
//                              .arg(bitRate.toInt() / 1000));
//        } else {
//            m_status->setText(tr("Plugin: %1, connected to %2")
//                              .arg("peakcan").arg(p.deviceInterfaceName));
//        }
//    }
//}



//void MainWindow::disconnectCanDevice()
//{
//    if (!m_canDevice)
//        return;
//    m_canDevice->disconnectDevice();
//    delete m_canDevice;
//    m_canDevice = nullptr;

//    m_ui->action_ConnectCAN->setEnabled(true);
//    m_ui->action_DisconnectCAN->setEnabled(false);
//    m_ui->sendFrameBox->setEnabled(false);

//    m_status->setText(tr("Disconnected"));
//}



void MainWindow::closeEvent(QCloseEvent *event)
{
    m_canMonitorDialog->close();
    m_canConnectDialog->close();
    event->accept();
}



void MainWindow::on_AutoActivateButton_released()
{

}

void MainWindow::on_ManualActivateButton_released()
{

}
