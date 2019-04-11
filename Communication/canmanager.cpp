#include "canmanager.h"
#include <QCanBus>
#include <QCanBusFrame>
#include "../canconnectdialog.h"
//#include <mainwindow.h>
#include <QDebug>
#include <QMetaType>
#include "Common/common.h"
#include "speedmanager.h"
#include "steermanager.h"

#include <QtEndian>
#include <QThread>

CanManager* CanManager::_instance = nullptr;

CanManager::CanManager() : QObject()
{
    //QObject::connect();

    qRegisterMetaType<QCanBusDevice::CanBusError>();

    my_number_test = 1;
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

QTimer *CanManager::getTimer()
{
    return timer;
}

void CanManager::initCanManager()
{
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(manageCan()));
    timer->start(1);
}

void CanManager::setMutex(QMutex *mutex)
{
    m_mutex = mutex;
}

void CanManager::start()
{
    qDebug() << "Manager "<<QThread::currentThread();

    timer = new QTimer();
    initCanManager();
}

void CanManager::activateDCU()
{
    if ( ! m_canDevice)
        return;
    // Set dcu id and activate message

//    m_canDevice->writeFrame(frame);
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

void CanManager::setSharedNumber(int *number)
{
    shared_number_test = number;
}

void CanManager::manageCan()
{
    // Check for can device
//    while (true) {
//        QThread::msleep(1);

//    }
    //qDebug() << QThread::currentThread();

//    if (shared_number_test) {

//        m_mutex->lock();
////        qDebug()<<"Thread" << my_number_test<<" num : " << shared_number_test << " *num : " << *shared_number_test;
////qDebug() << QThread::currentThread();
////        qDebug() << "Before change, my number : " << my_number_test << " shared numb: " << *shared_number_test;
//        (*shared_number_test) = my_number_test;

//        //QThread::usleep(10);
//        if (*shared_number_test != my_number_test) {
//            qDebug() << " Collision detected at thread " << my_number_test;
//        }
//        //qDebug() << "After the change, my number : " << my_number_test << " shared numb : " << *shared_number_test;
//        m_mutex->unlock();
//        //QThread::usleep(1);


//    }
//    else {
//        qDebug() <<" Say";
//    }
//    return;
        //qDebug() << m_monitorFlag;
        //qDebug() << "Can test";
//    QCanBusDevice::CanBusDeviceState state = m_canDevice->state();
    //m_canDevice->setState(QCanBusDevice::CanBusDeviceState::ConnectedState);
//    qDebug() << "Status " << (state == QCanBusDevice::CanBusDeviceState::ConnectedState);
//    if (m_canDevice->error()) {
//        qDebug() << m_canDevice->error();
//        //m_canDevice->setState(QCanBusDevice::CanBusDeviceState::ConnectedState);
//    }
    if (m_canDevice){
        if (m_canDevice->error()) {
            qDebug() << m_canDevice->error();
            return;
            //m_canDevice->setState(QCanBusDevice::CanBusDeviceState::ConnectedState);
        }
            //Check the controller objects
            // Use the objects to control the robot
            if (SpeedManager::hasInstance()){
                //Calculate the corresponding voltage
                //Send the voltage with direction
                if (SpeedManager::Instance()->speedChanged()){
                    int voltage = SpeedManager::Instance()->calculateVoltageFromSpeed();

                    sendCanMsg(SEVCON_CAN_ID, SEVCON_DRIVE_CMD, voltage);
                }
                //qDebug() <<"Success";
            }

            // Check all the objects. If none of them is enabled, it means that the
            // can device is connected but DCU has not been activated- manual mode on.
            else {
                // Send DCU Manual request msgx
//                qDebug() <<"Requesting";

//                if (m_canDevice->)
//                sendDcuCanMsg(DCU_MANUAL_REQUEST);


            }
            if (SteerManager::hasInstance()) {
                float output_voltage = SteerManager::Instance()->getTargetVoltage();
                float brake_voltage = SteerManager::Instance()->getBrakeVoltage();
                // Make a 200D can message
                // Fix the data type
                //sendCanMsg2(MW200D_CAN_ID, STEER_DRIVE_CMD,output_voltage);
                //endCanMsg2(MW200D_CAN_ID, BRAKE_DRIVE_CMD, brake_voltage);
            }
    }
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
    //Before disconnecting from the can device, let DCU get back to Off mode state
    sendDcuCanMsg(DCU_OFF_REQUEST);
//    sendDcuCanMsg(DCU_MANUAL_REQUEST);
    m_canDevice->waitForFramesWritten(10);
    //QThread::sleep(10);
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

int CanManager::decodeFrame(const QCanBusFrame &frame)
{
    int value=0;
    const QByteArray payload = frame.payload();

    //m_canDevice->state()
    //m_canDevice->
    if(frame.isValid())
    {
        switch(frame.frameId())
        {
            case PC_CAN_ID:
            {
                int x;
                x = payload[0];
                if (x == 1) {
                    emit manualModeOn();
                    //Here send Sevcon initialize message.
                    sendCanMsg(SEVCON_CAN_ID, SEVCON_INIT_REQUEST);
                }
                else if (x == 2) {
                    emit autoModeOn();
                }

                //else if (x == )

                break;
            }

            case ABS_ENC_CAN_ID:
            {
                // Here lock the shared resource and update the current encoder
                // using the SteerManager

                // First check if steermanager exists

                m_mutex->lock();
                qint16 single_cnt = qFromLittleEndian<qint16>(payload);
                qint16 multi_cnt = qFromLittleEndian<qint16>(payload.mid(2,2));

                SteerManager::Instance()->calculateCurrentSteerAngle(single_cnt, multi_cnt);
                m_mutex->unlock();
                break;
            }
            case MW200D_CAN_ID:
            {

                break;
            }

            //case

        }

        return value;
    }

    value = 1;

    return value;

}
void CanManager::processReceivedFrames()
{
    if (!m_canDevice)
        return;

    while (m_canDevice->framesAvailable()) {
        const QCanBusFrame frame = m_canDevice->readFrame();
        decodeFrame(frame);
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


void CanManager::sendDcuCanMsg(int index){

    QCanBusFrame frame;
    QByteArray load;
    //= QCanBusFrame(DCU_CAN_ID, load);
    switch (index)
    {
        case DCU_AUTO_REQUEST:
        {
            // Format
            // First four bytes refer to the index
            // rLast two bytes refer to the value
            load = QByteArrayLiteral("\x01\x00\x00\x00\x02\x00");
            break;
        }
        case DCU_MANUAL_REQUEST:
        {
            load = QByteArrayLiteral("\x01\x00\x00\x00\x01\x00");

            break;
        }
        case DCU_OFF_REQUEST:
        {
            load = QByteArrayLiteral("\x01\x00\x00\x00\x00\x00");

            break;
        }
    }
    frame = QCanBusFrame(DCU_CAN_ID, load);
    sendFrame(frame);
}

void CanManager::sendCanMsg(quint32 id, int index, int data)
{
    QCanBusFrame frame;
    QByteArray load;
    switch (index)
    {
        case DCU_AUTO_REQUEST:
        {
            // Format
            // First four bytes refer to the index
            // Last two bytes refer to the value
            load = QByteArrayLiteral("\x01\x00\x00\x00\x02\x00");
            break;
        }
        case DCU_MANUAL_REQUEST:
        {
            load = QByteArrayLiteral("\x01\x00\x00\x00\x01\x00");

            break;
        }
        case DCU_OFF_REQUEST:
        {
            load = QByteArrayLiteral("\x01\x00\x00\x00\x00\x00");

            break;
        }
        case SEVCON_INIT_REQUEST:
        {
            // This message tells the gear is in neutral state and the speed is zero.
            // This message is required to activate the Sevcon driver.

            load = QByteArrayLiteral("\x00\x00\x00\x00\x00\x00");
            break;
        }
    case SEVCON_DRIVE_CMD:
    {
        load.resize(3);
        // Direction byte
        load[2] = (data > 0)? 0x01 : 0x02;
        data = abs(data);
        qDebug() << data;
        load[0] = (data >> 0) & 0xFF;
        load[1] = (data >> 8) & 0xFF;

        if (DEBUG_MODE){
            QByteArray::iterator end = load.end();
            qDebug() << "Sevcon drive command message data";
            for (QByteArray::iterator it = load.begin(); it != end; ++it){
                qDebug() << static_cast<unsigned char>(*it);
            }
        }

        break;
    }

    case STEER_ENABLE_REQUEST:
    {
        load.resize(6);
        load[0] = 0x14;
        load[1] = 0x65;
        load[2] = 0x00;
        load[3] = 0x02;
        load[4] = 0x01;
        load[5] = 0x00;

        break;
    }
    case STEER_DISABLE_REQUEST:
    {
        load.resize(6);
        load[0] = 0x14;
        load[1] = 0x65;
        load[2] = 0x00;
        load[3] = 0x02;
        load[4] = 0x00;
        load[5] = 0x00;
        break;
    }
    case STEER_CLEAR_FAULT:
    {
        load.resize(6);
        load[0] = 0x14;
        load[1] = 0x65;
        load[2] = 0x00;
        load[3] = 0x02;
        load[4] = 0x02;
        load[5] = 0x00;
        break;
    }

    case BRAKE_ENABLE_REQUEST:
    {
        load.resize(6);
        load[0] = 0x14;
        load[1] = 0x65;
        load[2] = 0x00;
        load[3] = 0x01;
        load[4] = 0x01;
        load[5] = 0x00;

        break;
    }
    case BRAKE_DISABLE_REQUEST:
    {
        load.resize(6);
        load[0] = 0x14;
        load[1] = 0x65;
        load[2] = 0x00;
        load[3] = 0x01;
        load[4] = 0x00;
        load[5] = 0x00;
        break;
    }
    case BRAKE_CLEAR_FAULT:
    {
        load.resize(6);
        load[0] = 0x14;
        load[1] = 0x65;
        load[2] = 0x00;
        load[3] = 0x01;
        load[4] = 0x02;
        load[5] = 0x00;
        break;
    }
    }
    frame = QCanBusFrame(id, load);
    sendFrame(frame);
}

void CanManager::sendCanMsg2(quint32 id, int index, float data)
{
    QCanBusFrame frame;
    QByteArray load;
    switch (index)
    {
        case STEER_DRIVE_CMD:
        {
            //Check the instance of the Steer Manager
            if ( ! SteerManager::hasInstance() ){
                qDebug() << "Steer manager is not created";
                return;
            }
            m_mutex->lock();
            // Need to lock and access the resource SteerManager to get the target steer
            load.resize(4);
            load[0] = 0x1C;
            load[1] = 0x72;
            load[2] = 0x00;
            load[3] = 0x02;
            load.append(reinterpret_cast<const char*>(&data), sizeof(data));
//            qDebug() <<"";
//            for (char c : load){
//                qDebug() << "Look through the values of the msg" <<static_cast<int>(c);;
//            }
//            qDebug() <<"";


//            load[4] = (voltage >> 0) & 0xFF;
//            load[5] = (voltage >> 8) & 0xFF;
//            load[6] = (voltage >> 16) & 0xFF;
//            load[7] = (voltage >> 24) & 0xFF;
            m_mutex->unlock();
            break;
        }
    case BRAKE_DRIVE_CMD:
    {
        if ( ! SteerManager::hasInstance() ){
            qDebug() << "Steer manager is not created";
            return;
        }
        m_mutex->lock();
        // Need to lock and access the resource SteerManager to get the target steer
        load.resize(4);
        qDebug() << " Btrake voltage " << data;

        load[0] = 0x1C;
        load[1] = 0x72;
        load[2] = 0x00;
        load[3] = 0x01;
        load.append(reinterpret_cast<const char*>(&data), sizeof(data));

        m_mutex->unlock();
        break;
    }

    }
    frame = QCanBusFrame(id, load);
    sendFrame(frame);
}
