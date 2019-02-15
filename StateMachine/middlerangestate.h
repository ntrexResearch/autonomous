#ifndef MIDDLERANGESTATE_H
#define MIDDLERANGESTATE_H

#include "vehiclestate.h"
#include <Controller/distancecontroller.h>

class MiddleRangeState : public State
{
public:
    MiddleRangeState();
    ~MiddleRangeState();
    static MiddleRangeState* Instance();
    virtual bool Update(StateMachine *);
    virtual void Init(float _gainData[4][3], int _pidLimit);
    virtual QString getStateName();

    DistanceController* GetDistanceControllerInstance();

    virtual float GetReferenceAcceleration(float _relativeDistance, int _dt);
    virtual float GetReferenceVelocity(float _referenceAcceleration, float _currentReferenceVelocity);

private:
    static MiddleRangeState middleRangeState;
    DistanceController* distanceController;
};

#endif // MIDDLERANGESTATE_H
