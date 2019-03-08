#include "canmanager.h"
#include <QCanBus>
#include <QCanBusFrame>
#include "../canconnectdialog.h"
//#include <mainwindow.h>
#include <QDebug>
#include <QMetaType>

CanManager* CanManager::_instance = nullptr;

CanManager::CanManager() : QObject()
{
    //QObject::connect();

    qRegisterMetaType<QCanBusDevice::CanBusError>();
    QByteArray load = QByteArrayLiteral("\x12\x00\xa4");
    frame = QCanBusFrame(16, load);
    //timer = new QTimer();
    //initCanManager();
}

CanManager::~CanManager()
{
    delete m_canDevice;
}

CanManager* CanManager::Instance()
{
    if (_instance == nullptr)
        _instance = new CanManager();
    return _instance;
}

void CanManager::initCanManager()
{
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(manageCan()));
    timer->start(1000);
}

int CanManager::decodeFrame(const QCanBusFrame &frame)
{
    int value=0;
    const QByteArray payload = frame.payload();

    if(frame.isValid())
    {
        switch(frame.frameId())
        {

        }

        return value;
    }

    value = 1;

    return value;

}

QCanBusDevice::Filter CanManager::setCanFilter(const unsigned short &id)
{
    QCanBusDevice::Filter filter;

    filter.frameId = id;
    filter.frameIdMask = 0x7FFu; // Compare against all 11-bits of frame id.
    filter.format = QCanBusDevice::Filter::MatchBaseAndExtendedFormat;
    filter.type = QCanBusFrame::DataFrame;

    return filter;
}



void CanManager::manageCan()
{
    // Check for can device
//    while (true) {
//        QThread::msleep(1);

//    }
        //qDebug() << m_monitorFlag;
        //qDebug() << "Can test";
        if (m_canDevice){
            //qDebug() << "connected";
            //m_canDevice->writeFrame()
//            while (m_canDevice->framesAvailable())
//                m_canDevice->readFrame();
    //        m_canDevice->waitForFramesWritten(10);
            //m_canDevice->
            //if (m_canDevice->framesToWrite() == 0)
            //if (m_canDevice->error() == QCanBusDevice::NoError){
                //qDebug() <<m_canDevice->error();
                //m_canDevice->framesWritten(10);
                //m_canDevice->writeFrame(frame);
            //}
//            else {
//                //m_canDevice->
//            }
        }

        else {
            //qDebug() << "not conn";
        }
        //qDebug() << m_canDevice->
        //timer->
        //QThread::sleep(1);

    // If connected,
}

void CanManager::connectCanDevice(Settings p)
{
    // How am I going to bring Can Connect Dialog setting to this object

    // No no, this does not work.
    // Get the argument from the dialog
    //const Settings p = ;

    // Somehow bring the setting to m_current

    m_currentSettings = p;
    QString errorString;
    if (m_canDevice){

        qDebug() << "Device already connected";

        return;
    }
    m_canDevice = QCanBus::instance()->createDevice("peakcan", m_currentSettings.deviceInterfaceName, &errorString);

    if (!m_canDevice) {
        //Signal slot send signal to the CAN Window or main window

        return;
    }

    m_numberWrittenFrames = 0;

    QObject::connect(m_canDevice, &QCanBusDevice::errorOccurred, this, &CanManager::processErrors);
    QObject::connect(m_canDevice, &QCanBusDevice::framesReceived, this, &CanManager::processReceivedFrames);
    QObject::connect(m_canDevice, &QCanBusDevice::framesWritten, this, &CanManager::processWrittenFrames);
    // Set the configuration
    for (const ConfigurationItem &item : m_currentSettings.configurations)
        m_canDevice->setConfigurationParameter(item.first, item.second);
    // Apply filters to CAN Bus device.
//    QList<QCanBusDevice::Filter> filterList;

//    filterList.append(setCanFilter(0x150));
    //filterList.append(setCanFilter(E46_VEHICLE_SPEED));
//    filterList.append(setCanFilter(2));
//    filterList.append(setCanFilter(3));
//    filterList.append(setCanFilter(4));

//    m_canDevice->setConfigurationParameter(QCanBusDevice::RawFilterKey, QVariant::fromValue(filterList));

    if (!m_canDevice->connectDevice()) {

        delete m_canDevice;
        m_canDevice = nullptr;
    }
    else {
        QVariant bitRate = m_canDevice->configurationParameter(QCanBusDevice::BitRateKey);
        if (bitRate.isValid()) {
            if (m_monitorFlag)
                emit showStateMessage(tr("Plugin: %1, connected to %2 at %3 kBit/s")
                              .arg("peakcan").arg(p.deviceInterfaceName)
                              .arg(bitRate.toInt() / 1000));
        } else {
            if (m_monitorFlag)
                emit showStateMessage(tr("Plugin: %1, connected to %2")
                        .arg("peakcan").arg(p.deviceInterfaceName));

        }
        m_connectionState = true;
        emit sendConnectionState(m_connectionState);
    }
}

void CanManager::processErrors(QCanBusDevice::CanBusError error) const
{
    switch (error) {
    case QCanBusDevice::ReadError:
    case QCanBusDevice::WriteError:
    case QCanBusDevice::ConnectionError:
    case QCanBusDevice::ConfigurationError:
    case QCanBusDevice::UnknownError:
        if (m_monitorFlag)
            emit showStateMessage(m_canDevice->errorString());
        break;
    default:
        break;
    }
}


void CanManager::processWrittenFrames(qint64 count)
{
    m_numberWrittenFrames += count;
    //qDebug() << m_numberWrittenFrames;
    if (m_monitorFlag)
        emit notifyWrittenFrames(m_numberWrittenFrames);

}

void CanManager::disconnectCanDevice()
{
    if (!m_canDevice)
        return;
    m_canDevice->disconnectDevice();
    delete m_canDevice;
    m_canDevice = nullptr;
    m_connectionState = false;
    emit sendConnectionState(m_connectionState);
}

static QString frameFlags(const QCanBusFrame &frame)
{
    QString result = QLatin1String(" --- ");

    if (frame.hasBitrateSwitch())
        result[1] = QLatin1Char('B');
    if (frame.hasErrorStateIndicator())
        result[2] = QLatin1Char('E');
    if (frame.hasLocalEcho())
        result[3] = QLatin1Char('L');

    return result;
}


void CanManager::monitorOn()
{
    m_monitorFlag = true;
}

void CanManager::monitorOff()
{
    m_monitorFlag = false;
}


void CanManager::processReceivedFrames()
{
    if (!m_canDevice)
        return;

    while (m_canDevice->framesAvailable()) {
        const QCanBusFrame frame = m_canDevice->readFrame();

        QString view;
        if (frame.frameType() == QCanBusFrame::ErrorFrame)
            view = m_canDevice->interpretErrorFrame(frame);
        else
            view = frame.toString();

        const QString time = QString::fromLatin1("%1.%2  ")
                .arg(frame.timeStamp().seconds(), 10, 10, QLatin1Char(' '))
                .arg(frame.timeStamp().microSeconds() / 100, 4, 10, QLatin1Char('0'));

        const QString flags = frameFlags(frame);

        //qDebug() << time + flags + view;
        if (m_monitorFlag)
            emit showReceivedFrames(time + flags + view);
    }
}

void CanManager::sendFrame(const QCanBusFrame &frame) const
{
    if (!m_canDevice)
        return;
    m_canDevice->writeFrame(frame);
}
