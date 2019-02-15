#ifndef EMERGENCYSTATE_H
#define EMERGENCYSTATE_H
#include "StateMachine/vehiclestate.h"
#include "Controller/distancecontroller.h"

class EmergencyState : public State
{
public:
    EmergencyState();
    ~EmergencyState();

    DistanceController* GetDistanceControllerInstance();

    static EmergencyState* Instance();
    virtual bool Update(StateMachine *);
    virtual void Init(float _gainData[4][3], int _pidLimit);
    virtual QString getStateName();
    virtual float GetReferenceAcceleration(float _relativeDistance, int _dt);
    virtual float GetReferenceVelocity(float _referenceAcceleration, float _currentReferenceVelocity);
private:
    static EmergencyState emergencyState;
    DistanceController* distanceController;

};

#endif // EMERGENCYSTATE_H
