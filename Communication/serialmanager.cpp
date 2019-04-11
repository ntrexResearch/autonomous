#include "serialmanager.h"

SerialManager::SerialManager(QObject *_parent) : QObject(_parent)
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

        QObject::connect(mSerial, SIGNAL(readyRead()), this, SLOT(readAvailable()));
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

int SerialManager::addTxMsg(QString msg)
{
    txQueue.enqueue(msg);
    return txQueue.size();
}


// User must check the size before using this method
QString SerialManager::popTxMsg()
{
    return txQueue.dequeue();
}

int SerialManager::Send(QString buff)
{
    if ( ! mSerial->isOpen() )
        return -1;
    buff.append('\n');
    return static_cast<int>(mSerial->write(buff.toUtf8()));
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

void SerialManager::readAvailable()
{
    int cnt = 0;
    while (!Receive(&rxBuff) && cnt < timeout){
        //qDebug () << rxBuff;
        cnt ++;
        return;
        //Set timeout
    }
    rxQueue.enqueue(QString::fromUtf8(rxBuff));
    //qDebug () << rxBuff;
    emit showRxMsg(QString::fromUtf8(rxBuff));
    rxBuff.clear();

}

QString SerialManager::getRxMsg()
{
    return QString::fromUtf8(rxBuff);
}
