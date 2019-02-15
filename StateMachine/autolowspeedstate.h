#ifndef AUTOLOWSPEEDSTATE_H
#define AUTOLOWSPEEDSTATE_H

#include "vehiclestate.h"
#include <Controller/distancecontroller.h>

class AutoLowSpeedState : public State
{
public:
    AutoLowSpeedState();
    ~AutoLowSpeedState();
    static AutoLowSpeedState* Instance();
    virtual bool Update(StateMachine*);
    virtual void Init(float gainData[4][3], int _pidLimit);
    virtual QString getStateName();

    DistanceController* GetDistanceControllerInstance();

    virtual float GetReferenceAcceleration(float _relativeDistance, int _dt);
    virtual float GetReferenceVelocity(float _referenceAcceleration, float _currentReferenceVelocity);

private:
    static AutoLowSpeedState autoLowSpeedState;
    DistanceController* distanceController;
};

#endif // AUTOLOWSPEEDSTATE_H
