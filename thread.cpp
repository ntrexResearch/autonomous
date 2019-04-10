#include "thread.h"

Thread::Thread()
{

}
Thread::~Thread()
{
    exit();
}

void Thread::run(){
    QTimer timer;

    connect(&timer, SIGNAL(timeout()), &worker, SLOT(onTimeout()));
    qDebug() << QThread::currentThread();

    timer.start(10);
    exec();
}

Worker* Thread::getWorker() {
    return &worker;
}
