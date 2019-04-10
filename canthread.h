#ifndef CANTHREAD_H
#define CANTHREAD_H
#include <QThread>
#include <QTimer>
#include <QObject>
#include "Communication/canmanager.h"
#include <QMutex>

class CanThread : public QThread
{
    Q_OBJECT
public:
    CanThread(QMutex *mutex);
    ~CanThread();
    void setSharedNumber(int *number);
    //void setMyNumber(int number);

private:
    void run();
    CanManager *canManager;
    QMutex *m_mutex;


};

#endif // CANTHREAD_H
