#ifndef VIRTUALLEADSTATE_H
#define VIRTUALLEADSTATE_H
#include "StateMachine/vehiclestate.h"
#include "Controller/distancecontroller.h"

class VirtualLeadState : public State
{
public:
    VirtualLeadState();
    ~VirtualLeadState();

    DistanceController* GetDistanceControllerInstance();

    static VirtualLeadState* Instance();
    virtual bool Update(StateMachine *);
    virtual void Init(float _gainData[4][3], int _pidLimit);
    virtual QString getStateName();
    virtual float GetReferenceAcceleration(float _relativeDistance, int _dt);
    virtual float GetReferenceVelocity(float _referenceAcceleration, float _currentReferenceVelocity);
private:
    static VirtualLeadState virtualLeadState;
    DistanceController* distanceController;
    float constantAcceleration;
    float marginVelocity;
};

#endif // VIRTUALLEADSTATE_H
