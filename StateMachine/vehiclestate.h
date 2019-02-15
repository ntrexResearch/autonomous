#pragma once

#ifndef STATE_H
#define STATE_H

#include "statemachine.h"
#include <QString>
#include <QDebug>
#include <QObject>
/*
 *Key State Description
 * Key State bool array 에 따라 주행 모드가 결정된.
 * 주행 모드는 크게 OFF, Manual, Auto 로 나뉜.
 *  KeyState = [0    0] --- OFF
 *           = [0    1] --- Manual
 *           = [1    0] --- Auto
 * Auto Mode는 또 상황별 여러 모드로 나뉘게되는
 * Normal 주행 모드 , Complete Stop 모, Complete Go 로 크게 나뉜다.
 * KeyState 이 auto가 되면 Complete Start에서 시작하게된.
 *
 */


class StateMachine;

class State : public QObject
{
    Q_OBJECT
public:
    State();
    virtual ~State();
    virtual void Init(float _gainData[4][3], int _pidLimit) = 0;
    virtual bool Update(StateMachine*) = 0;

    virtual QString getStateName() = 0;
    //Offstate index 0
    //Manual state index 1
    //Auto init state index 2
    //Auto vehicle detected following state index 3
    //Auto stop state index 4
    //Auto emergency state index 5
    //Auto vehicle not detected state index 6
 //   virtual float* RunDistanceController(float distance_error, float _targetDistance, int dt) = 0;

    void enableDebug(bool);

    virtual float GetReferenceAcceleration(float _relativeDistance, int dt) = 0;
    virtual float GetReferenceVelocity(float _referenceAcceleration, float _currentReferenceVelocity) = 0;

public slots:

protected:
    void ChangeState(StateMachine*, State*);
    //Same operations for all states


private:

    //Keep the data

    bool debug;

    char index;
};

#endif // STATE_H
