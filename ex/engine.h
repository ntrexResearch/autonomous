#ifndef ENGINE_H
#define ENGINE_H

#include <QTimer>
#include <QByteArray>
#include <QDebug>
#include <stdexcept>
#include "Communication/busserial.h"
#include "Communication/deviceserial.h"
#include "Communication/sensordeviceserial.h"
#include "debugger.h"
#include "StateMachine/statemachine.h"
//#include "Controller/distancecontroller.h"
#include "common.h"
#include <QElapsedTimer>

class Engine : public QObject
{
    Q_OBJECT

public:
    static Engine* Instance();

    bool SendMsg(QString);
    //DistanceController* getControllerInstance();
    State* getStateInstance();
    StateMachine* getStateMachineInstance();
    bool ConnectToSensors(char*);
    void setDebugger(Debugger *debugger);

public slots:
    void TimerRun();
    void UpdatePIDParam(float*, float*, float*, int);
    void UpdateVirtualSensorData(float);

signals:
    void UpdateSensorConnectionStatus(bool);
    //void TransferGainData(float*, float*, float*, int, float);
protected:
    Engine(QObject *_parent);

private:
    QTimer *timer;
    int timerInterval;
    static Engine* _instance;

    char timer_count;



    //Communication
    SensorDeviceSerial *_sensorBusSerial;
    //DistanceController *_distanceController;
    StateMachine *stateMachine;
    Debugger *debugger;
    bool DCUConnected;
    bool SensorConnected;

    QElapsedTimer elapsedTimer;
    int elapsedTime;
    bool elapsedTimerInitFlag;

    int mockRelativeDistance;
    float relativeDistance;
    float targetDistance;


    float dt;// Time step


};

#endif // ENGINE_H
