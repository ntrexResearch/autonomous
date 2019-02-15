#include "serialdcuthread.h"

SerialDCUThread::SerialDCUThread()
{

}

SerialDCUThread::~SerialDCUThread()
{

    exit();
}

void SerialDCUThread::run() {
    QTimer timer;

    connect(&timer, SIGNAL(timeout()), &worker, SLOT(onTimeout()));

    timer.start(10);
    exec();
}

DCUWorker* SerialDCUThread::getWorker() {
    return &worker;
}

