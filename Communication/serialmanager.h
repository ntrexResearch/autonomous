#ifndef SERIALMANAGER_H
#define SERIALMANAGER_H
#include <QtSerialPort/qserialport.h>
#include <QQueue>
#include <QString>
#include <QDebug>
#include <QRegExp>
#include <QByteArray>

class SerialManager
{
public:
    SerialManager();

    int Send(QByteArray buff);
    bool Receive(QByteArray* _rxBuff);

    int getTxBufferSize();

    bool Open(QString port, QSerialPort::BaudRate baud);
    void Close();
    bool isOpen();

private:
    QSerialPort* mSerial = nullptr;
    QQueue<QByteArray> txQueue;
    QByteArray rxBuff;

};

#endif // SERIALMANAGER_H
