#pragma once
#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "vehiclestate.h"
#include <QObject>
#include <Controller/distancecontroller.h>

class State;

class StateMachine : public QObject
{
    Q_OBJECT
public:
    StateMachine();

	//Data including key state, dir state, brake info
    //void UpdateSwPara(char* data);
    char GetKeyState();
    State* GetStateInstance();
    void GetReferenceAcceleration(float _relativeDistance,int dt);
    void GetReferenceVelocity();

    void UpdateGainData(float*, float*, float*, int _pidLimit);

    void UpdateData(float relativeDistance, int dt);
    /*
     * Key State
     * Off 0
     * Manual 1
     * Auto 2
     *
     */

    void setKeyState(char);
    void setDirState(char);
    void setBrakeState(char);
    void setEmergencyState(char);
    void setCurrentVelocity(float curVel);
    void setRelativeVelocity(float relVel);
    void setRelativeDistance(float relDist);


    char getKeyState();
    char getDirState();
    char getBrakeState();
    char getEmergencyState();
    char getGeneralOutputState();

    float getCurrentVelocity();
    float getRelativeVelocity();
    float getRelativeDistance();

    void setCurrentReferenceVelocity(float);
    float getCurrentReferenceVelocity();

    static float emergencyThresholdDistance;
    static float virtualLeadThresholdDistance;
    static float targetThresholdDistance;
    static float middleRangeThresholdDistance;
    static float lowSpeedThresholdVelocity;

    static bool emergencyBellFlag;

public slots:
    //void UpdateData(float curVel, float relVel, float relDist);
    void UpdateStateParameters(char data);
    void UpdateGeneralOutputData(char data);

signals:
    void UpdateControllerData(float, float, float, float);
    //void UpdateReferenceVelocity(int refVelocity, int time);
private:
    friend class State;
    void ChangeState(State*);
    State* _state;
    float _currentVelocity;
    float _relativeVelocity;
    //float _relativeDistance;

    //Follow the naming convention
    float referenceVelocityFl;
    float referenceAcceleration;
    float relativeDistance;
    float relativeVelocity;
    float currentVelocity;

    int referenceVelocity;
    float targetAccel;
    
    int dt;

    char keyState;
    char dirState;
    char brake;
    char emergency;
    char generalOutput;

    float gainData[4][3];
    int pidLimit;
    float lowPassGain;

};

#endif // STATEMACHINE_H
