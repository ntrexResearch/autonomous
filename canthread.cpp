#include "canthread.h"
#include <QDebug>

CanThread::CanThread(QMutex *mutex)
{
    m_mutex = mutex;
    canManager = CanManager::Instance();
    canManager->setMutex(m_mutex);
    //canManager->moveToThread(this);

    canManager->start();
    //canManager->getTimer()->moveToThread(this);
}

CanThread::~CanThread()
{
    exit();
}

void CanThread::run() {
    QTimer timer;

    qDebug() << "Can thread "<<QThread::currentThread();

//    connect(&timer, SIGNAL(timeout()), canManager, SLOT(manageCan()));
//    timer.start(10);
    exec();
}


void CanThread::setSharedNumber(int *number)
{
    canManager->setSharedNumber(number);
}
