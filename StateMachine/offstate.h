#pragma once

#ifndef OFFSTATE_H
#define OFFSTATE_H

#include "vehiclestate.h"

class OffState : public State
{
public:
    OffState();
    ~OffState();
    static OffState* Instance();
    virtual void Init(float gainData[4][3], int _pidLimit);
    virtual bool Update(StateMachine *);
    virtual QString getStateName();
    virtual float GetReferenceAcceleration(float _relativeDistance, int dt);
    virtual float GetReferenceVelocity(float _referenceAcceleration, float _currentReferenceVelocity);

private:
    static OffState _OffState;
};

#endif // OFFSTATE_H
