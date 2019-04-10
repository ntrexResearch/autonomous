#include "serialmanager.h"

SerialManager::SerialManager()
{

}


bool SerialManager::Open(QString port, QSerialPort::BaudRate baud)
{
    if (mSerial) {
        qDebug() << "Serial object already exists";
        if (mSerial->isOpen())
            return false;
    }
    else {
        mSerial = new QSerialPort;
    }
    mSerial->setPortName(port);
    mSerial->setBaudRate(QSerialPort::Baud115200);
    mSerial->setDataBits(QSerialPort::Data8);
    mSerial->setFlowControl(QSerialPort::NoFlowControl);
    mSerial->setParity(QSerialPort::NoParity);
    mSerial->setStopBits(QSerialPort::OneStop);

    if(mSerial->open(QIODevice::ReadWrite)) {
        //qDebug() << "Device " << DEVICE_NAME << " Successfully Opened.. ";
        return true;
    }
    else {
        qDebug() << "Failed making connection.. ";
        return false;
    }
}

void SerialManager::Close()
{
    if (mSerial->isOpen())
        mSerial->close();
    delete mSerial;
}

bool SerialManager::isOpen()
{
    if (mSerial)
        return mSerial->isOpen();
    else
        return false;
}

int SerialManager::getTxBufferSize()
{
    return txQueue.size();
}

int SerialManager::Send(QByteArray buff)
{
    if ( ! mSerial->isOpen() )
        return -1;
    buff.append('\n');
    return static_cast<int>(mSerial->write(buff));
}

bool SerialManager::Receive(QByteArray *buff)
{
    if ( ! mSerial->isOpen()) return false;
    (*buff) += mSerial->readAll();
    if ((*buff).endsWith('\n'))
        return true;
    else if ((*buff).contains('\n')){
        qDebug() << " Does it happen? ";
        return false;
    }
    else
        return false;
}
