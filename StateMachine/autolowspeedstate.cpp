#include "autolowspeedstate.h"
#include "offstate.h"
#include "manualstate.h"
#include "emergencystate.h"
#include "autodrivestate.h"

AutoLowSpeedState AutoLowSpeedState::autoLowSpeedState;

AutoLowSpeedState::AutoLowSpeedState()
{
    distanceController = new DistanceController();
}

AutoLowSpeedState::~AutoLowSpeedState()
{

}

void AutoLowSpeedState::Init(float _gainData[4][3], int _pidLimit) {
    distanceController->InitController(_gainData[2][0], _gainData[2][1], _gainData[2][2], _pidLimit);
}

AutoLowSpeedState* AutoLowSpeedState::Instance()
{
    return {&autoLowSpeedState};
}

bool AutoLowSpeedState::Update(StateMachine *_StateMachine) {
    float currentReferenceVelocity = _StateMachine->getCurrentReferenceVelocity();

    if(_StateMachine->getKeyState() == 0) {
        ChangeState(_StateMachine, OffState::Instance());
        //Reset the acceleration
        return true;
    }
    else if(_StateMachine->getGeneralOutputState() == 0) {
        //To Auto Start state//initialize
        ChangeState(_StateMachine, ManualState::Instance());
        return true;
    }
    else if (currentReferenceVelocity > StateMachine::lowSpeedThresholdVelocity) {
        ChangeState(_StateMachine, AutoDriveState::Instance());
        return true;
    }
    return false;
    //After checking the key state, update the auto mode
}

float AutoLowSpeedState::GetReferenceAcceleration(float _relativeDistance, int _dt) {
    //Run the Distance Controller
    float referenceAcceleration = distanceController->GetReferenceAcceleration(_relativeDistance, StateMachine::targetThresholdDistance, _dt);
    return referenceAcceleration;
}

float AutoLowSpeedState::GetReferenceVelocity(float _referenceAcceleration, float _currentReferenceVelocity) {
    float referenceVelocity = distanceController->GetReferenceVelocity(_referenceAcceleration, _currentReferenceVelocity);
    return referenceVelocity;
}

QString AutoLowSpeedState::getStateName() {
    return "Auto Low Speed State";
}


