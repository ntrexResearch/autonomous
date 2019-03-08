#include "engine.h"
#include <QDebug>
#include <stdio.h>
#include <iostream>
#include <QApplication>
#include <limits>

Engine* Engine::_instance = 0;

Engine::Engine(QObject *_parent) :QObject(_parent)
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(TimerRun()));
    timer_count = 0;

//    timerInterval = 100;
    timerInterval = 20;
    dt = timerInterval / 1000.0; // Time step


    DCUConnected = false;
    SensorConnected = false;
    stateMachine = new StateMachine();
//    _distanceController = new DistanceController();

    _sensorBusSerial = new SensorDeviceSerial;

    elapsedTime = 0;
    elapsedTimerInitFlag = true;
    relativeDistance = 0;
    //Test
    timer->start(timerInterval);//


}

State* Engine::getStateInstance() {
    return stateMachine->GetStateInstance();
}

Engine* Engine::Instance(){
    if(_instance == 0) {
        _instance = new Engine(0);
    }
    return _instance;
}


void Engine::TimerRun() {


    //For test0710
    if(!_sensorBusSerial->isOpen()){
        relativeDistance = mockRelativeDistance;//slider info
    }
    else {
        char SensorID = 1;
        float tmp = _sensorBusSerial->Read(SensorID);
        relativeDistance = (tmp == -1) ?  70: tmp ;

    }
    //Sensor ID set

    //Mark the first time the timer runs
    if(elapsedTimerInitFlag){
        elapsedTime = 0;
        elapsedTimer.start();
        elapsedTimerInitFlag = false;
    }
    else{
        elapsedTime = elapsedTimer.elapsed();
        //qDebug()<<"Elapsed Time "<< elapsedTime;
        elapsedTimer.restart();
    }

    stateMachine->GetReferenceAcceleration(relativeDistance, elapsedTime);
    stateMachine->GetReferenceVelocity();

}

void Engine::setDebugger(Debugger *_debugger){
    debugger = _debugger;
}

bool Engine::ConnectToSensors(char* comPort) {
    if (_sensorBusSerial->isOpen()) {
        debugger->PrintErrorMsg("Disconnecting from sensors..");
        _sensorBusSerial->Close();
        timer->stop();

        emit UpdateSensorConnectionStatus(false);
        return false;
    }
    else{
        prependCharArray(comPort, "\\\\.\\");
        if (_sensorBusSerial->Open(comPort, 115200)) {
            //Start the timer
            timer->start(timerInterval);//
            debugger->PrintErrorMsg("Connected to sensors");
            emit UpdateSensorConnectionStatus(true);
            return true;
        }
        else{
            debugger->PrintErrorMsg("Failed to connect to sensors");
            return false;
        }
    }
}

bool Engine::SendMsg(QString msg) {

    return true;
}

StateMachine* Engine::getStateMachineInstance() {
    return stateMachine;
}

/*
 * Transfer the gain data to the statemachine
 */
void Engine::UpdatePIDParam(float* _pGain, float* _iGain, float* _dGain, int _pidLimit)
{
    stateMachine->UpdateGainData(_pGain, _iGain, _dGain, _pidLimit);

    //stateMachine->Update
}

void Engine::UpdateVirtualSensorData(float mockSensorData) {
    mockRelativeDistance = mockSensorData;
}
