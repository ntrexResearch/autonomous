#ifndef SERIALMANAGER_H
#define SERIALMANAGER_H
#include <QtSerialPort/qserialport.h>
#include <QQueue>
#include <QString>
#include <QDebug>
#include <QRegExp>
#include <QByteArray>

class SerialManager : public QObject
{
    Q_OBJECT
public:
    SerialManager(QObject *parent = nullptr);

    int Send(QString buff);
    bool Receive(QByteArray* _rxBuff);

    int getTxBufferSize();

    bool Open(QString port, QSerialPort::BaudRate baud);
    void Close();
    bool isOpen();
    int addTxMsg(QString);
    QString getRxMsg();
    QString popTxMsg();

private slots:
    void readAvailable();

signals:
    void showRxMsg(QString);

private:
    QSerialPort* mSerial = nullptr;
    QQueue<QString> txQueue;
    QByteArray rxBuff;
    QQueue<QString> rxQueue;

    int timeout = 10;

};

#endif // SERIALMANAGER_H
