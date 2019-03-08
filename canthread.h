#ifndef CANTHREAD_H
#define CANTHREAD_H
#include <QThread>
#include <QTimer>
#include <QObject>
#include "Communication/canmanager.h"

class CanThread : public QThread
{
    Q_OBJECT
public:
    CanThread();
    ~CanThread();

private:
    void run();
    CanManager *canManager;

};

#endif // CANTHREAD_H
