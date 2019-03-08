#ifndef THREAD_H
#define THREAD_H

#include <QObject>
#include <QThread>
#include "worker.h"
#include <QTimer>

class Thread : public QThread
{
    Q_OBJECT
public:
    Thread();
    ~Thread();
    Worker* getWorker();

private:
    void run();
    Worker worker;


};

#endif // THREAD_H
