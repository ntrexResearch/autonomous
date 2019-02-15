#pragma once

#ifndef AUTOINITSTATE_H
#define AUTOINITSTATE_H

#include "vehiclestate.h"
#include <Controller/distancecontroller.h>


class AutoDriveState : public State
{
public:
    AutoDriveState();
    ~AutoDriveState();
    static AutoDriveState* Instance();
    virtual bool Update(StateMachine *);
    virtual void Init(float gainData[4][3], int _pidLimit);
    virtual QString getStateName();

    DistanceController* GetDistanceControllerInstance();

    virtual float GetReferenceAcceleration(float _relativeDistance, int dt);
    virtual float GetReferenceVelocity(float _referenceAcceleration, float _currentReferenceVelocity);

private:
    static AutoDriveState _AutoDriveState;
    DistanceController* distanceController;
};

#endif // AUTOINITSTATE_H
