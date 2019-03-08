#include "canmonitordialog.h"
#include "ui_canmonitordialog.h"


#include <QCanBus>
#include <QCanBusFrame>

CanMonitorDialog::CanMonitorDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::CanMonitorDialog)
{
    m_ui->setupUi(this);

    initGui();


}

void CanMonitorDialog::initGui()
{
    m_statusBar = new QStatusBar;
    m_status = new QLabel;
    m_written = new QLabel;
    m_statusBar->addPermanentWidget(m_status);
    m_statusBar->addWidget(m_written);
    m_ui->centralWidget->layout()->addWidget(m_statusBar);
    // Disable the send frame box before connection is made.
    m_ui->sendFrameBox->setEnabled(false);
    //m_ui->receivedMessagesEdit->
    m_ui->receivedMessagesEdit->setMaximumBlockCount(2000);
}

SendFrameBox* CanMonitorDialog::getSendFrameBox()
{
    return m_ui->sendFrameBox;
}

void CanMonitorDialog::closeEvent(QCloseEvent *event)
{
    emit closed();
    event->accept();
}


void CanMonitorDialog::keyPressEvent(QKeyEvent *event){
    if (event->key() == Qt::Key_Escape) {
        close();
        event->accept();
        return;
    }else {
        QDialog::keyPressEvent(event);
    }
}

CanMonitorDialog::~CanMonitorDialog()
{
    delete m_ui;
}


void CanMonitorDialog::showReceivedFrames(QString rxFrame)
{
    m_ui->receivedMessagesEdit->appendPlainText(rxFrame);


}


void CanMonitorDialog::showCanState(QString msg) const
{   
    m_status->setText(msg);
}

void CanMonitorDialog::updateConnectionState(bool state){
    m_ui->sendFrameBox->setEnabled(state);
}

void CanMonitorDialog::processWrittenFrames(qint64 count)
{
    m_written->setText(tr("%1 frames written").arg(count));
}

void CanMonitorDialog::sendFrame(const QCanBusFrame &frame) const
{
    if (!m_canDevice)
        return;

    m_canDevice->writeFrame(frame);
}

void CanMonitorDialog::on_clearLogButton_released()
{
    m_ui->receivedMessagesEdit->clear();
}
