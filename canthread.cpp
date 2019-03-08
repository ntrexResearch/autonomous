#include "canthread.h"

CanThread::CanThread()
{
    canManager = CanManager::Instance();
}

CanThread::~CanThread()
{
    exit();
}

void CanThread::run() {
    QTimer timer;
//    connect(&timer, SIGNAL(timeout()), canManager, SLOT(manageCan()));
//    timer.start(10);
    exec();
}
