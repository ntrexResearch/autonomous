#include "emergencystate.h"
#include "autodrivestate.h"
#include "manualstate.h"
#include "offstate.h"

EmergencyState EmergencyState::emergencyState;

EmergencyState::EmergencyState()
{
    distanceController = new DistanceController();
}

EmergencyState::~EmergencyState()
{

}

void EmergencyState::Init(float _gainData[4][3], int _pidLimit)
{
    distanceController->InitController(_gainData[1][0], _gainData[1][1], _gainData[1][2], _pidLimit);
    StateMachine::emergencyBellFlag = true;
    //    distanceController->InitController();
}

EmergencyState* EmergencyState::Instance()
{
    return {&emergencyState};
}

bool EmergencyState::Update(StateMachine *_stateMachine)
{

    float relativeDistance = _stateMachine->getRelativeDistance();
    float currentReferenceVelocity = _stateMachine->getCurrentReferenceVelocity();
    /*
     * Based on the relative distance, switch back to auto state
     */
    if(_stateMachine->getKeyState() == 0) {
        ChangeState(_stateMachine, OffState::Instance());
        StateMachine::emergencyBellFlag = false;

        return true;
    }
    else if(_stateMachine->getGeneralOutputState() == 0) {
        //To Auto Start state//initialize
        ChangeState(_stateMachine, ManualState::Instance());
        StateMachine::emergencyBellFlag = false;

        return true;
    }
    else if (relativeDistance > StateMachine::emergencyThresholdDistance) {
        ChangeState(_stateMachine, AutoDriveState::Instance());
        StateMachine::emergencyBellFlag = false;

        return true;
    }
    return false;

}
/*
 * Distance Controller for
 */
float EmergencyState::GetReferenceAcceleration(float _relativeDistance, int _dt) {
    float referenceAcceleration = distanceController->GetReferenceAcceleration(_relativeDistance, StateMachine::targetThresholdDistance, _dt);
    return referenceAcceleration;
}

float EmergencyState::GetReferenceVelocity(float _referenceAcceleration, float _currentReferenceVelocity) {
    float referenceVelocity = distanceController->GetReferenceVelocity(_referenceAcceleration, _currentReferenceVelocity);
    return referenceVelocity;
}

QString EmergencyState::getStateName() {
    return "Emergency State";
}
