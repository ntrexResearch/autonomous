#pragma once

#ifndef MANUALSTATE_H
#define MANUALSTATE_H

#include "vehiclestate.h"

class ManualState : public State
{
public:
    ManualState();
    ~ManualState();
    static ManualState* Instance();
    virtual bool Update(StateMachine *);
    virtual void Init(float gainData[4][3], int _pidLimit);
    virtual QString getStateName();
    virtual float GetReferenceAcceleration(float _relativeDistance, int dt);
    virtual float GetReferenceVelocity(float _referenceAcceleration, float _currentReferenceVelocity);

private:
    static ManualState _ManualState;
};

#endif // MANUALSTATE_H
