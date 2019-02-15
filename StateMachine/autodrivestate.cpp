#include "autodrivestate.h"
#include "offstate.h"
#include "manualstate.h"
#include "emergencystate.h"
#include "middlerangestate.h"
#include "autolowspeedstate.h"

AutoDriveState AutoDriveState::_AutoDriveState;

/*
 * Set the threshold distance for
 */
AutoDriveState::AutoDriveState()
{
    distanceController = new DistanceController();
}

AutoDriveState::~AutoDriveState()
{

}

void AutoDriveState::Init(float _gainData[4][3], int _pidLimit) {
    distanceController->InitController(_gainData[0][0], _gainData[0][1], _gainData[0][2], _pidLimit);
}

AutoDriveState* AutoDriveState::Instance(){
    return {&_AutoDriveState};
}

bool AutoDriveState::Update(StateMachine *_StateMachine) {
    float relativeDistance = _StateMachine->getRelativeDistance();
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
    else if(currentReferenceVelocity < StateMachine::lowSpeedThresholdVelocity) {
        ChangeState(_StateMachine, AutoLowSpeedState::Instance());
        return true;
    }
    else if (relativeDistance < StateMachine::emergencyThresholdDistance) {
        ChangeState(_StateMachine, EmergencyState::Instance());
        return true;
    }
    else if (relativeDistance > StateMachine::middleRangeThresholdDistance) {
        ChangeState(_StateMachine, MiddleRangeState::Instance());
        return true;
    }
    return false;
    //After checking the key state, update the auto mode
}

float AutoDriveState::GetReferenceAcceleration(float _relativeDistance, int _dt) {
    //Run the Distance Controller
    float referenceAcceleration = distanceController->GetReferenceAcceleration(_relativeDistance, StateMachine::targetThresholdDistance, _dt);
    return referenceAcceleration;
}

float AutoDriveState::GetReferenceVelocity(float _referenceAcceleration, float _currentReferenceVelocity) {
    float referenceVelocity = distanceController->GetReferenceVelocity(_referenceAcceleration, _currentReferenceVelocity);
    return referenceVelocity;
}

QString AutoDriveState::getStateName() {
    return "Auto Drive State";
}


