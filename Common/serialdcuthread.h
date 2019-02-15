#ifndef SERIALDCUTHREAD_H
#define SERIALDCUTHREAD_H

#include <QObject>
#include <QThread>
#include <QTimer>
#include "dcuworker.h"

class SerialDCUThread : public QThread
{
    Q_OBJECT
public:
    SerialDCUThread();
    ~SerialDCUThread();
    DCUWorker* getWorker();
private:
    void run();
    DCUWorker worker;

};

#endif // SERIALDCUTHREAD_H
