#ifndef ENGINE_H
#define ENGINE_H
#include <QObject>
#include <QTimer>
#include <QDebug>
#include <QElapsedTimer>
#include <QMutex>
#include "canthread.h"
#include "worker.h"

#include "Communication/serialmanager.h"

#include "steermanager.h"
//#include "pidcontroller.h"

class Engine : public QObject
{
    Q_OBJECT
public:
    static Engine* Instance(QObject *_parent);
    void setMutex(QMutex *mutex);
    ~Engine();

    void setSharedNumber(int* number);
    void setMyNumber(int number);

    SerialManager *getSerialManager();
public slots:
    void run();
    void openSensorSerial(QString port);
    void openBrakeDriverSerial(QString port);
    void enqueueBrakeTxMsg(QString);


protected:
    Engine(QObject *_parent);

private:
    QTimer *timer;
    static Engine* _instance;
    //PIDController * m_pidController = nullptr;
    float step_time; // in miliseconds
    //Thread canThread;
    //SteerController *m_steerController;
    QElapsedTimer elapsedTimer;
    CanThread *canThread;


    SerialManager *m_serialManager;
    int* shared_number_test = nullptr;
    int my_number_test;
    int number;

    //Controller *testThreadController;

    QMutex *m_mutex;

    void initialize();
};

#endif // ENGINE_H
