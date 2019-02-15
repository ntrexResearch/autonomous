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

    timer.start(1000);
    exec();
}

Worker* Thread::getWorker() {
    return &worker;
}
