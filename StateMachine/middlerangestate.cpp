#include "middlerangestate.h"
#include "autodrivestate.h"
#include "offstate.h"
#include "manualstate.h"
#include "virtualleadstate.h"
#include "autolowspeedstate.h"

MiddleRangeState MiddleRangeState::middleRangeState;

MiddleRangeState::MiddleRangeState()
{
    distanceController = new DistanceController();
}

MiddleRangeState::~MiddleRangeState()
{

}

void MiddleRangeState::Init(float _gainData[4][3], int _pidLimit) {
    distanceController->InitController(_gainData[3][0], _gainData[3][1], _gainData[3][2], _pidLimit);
}

MiddleRangeState* MiddleRangeState::Instance() {
    return {&middleRangeState};
}

bool MiddleRangeState::Update(StateMachine *_stateMachine)
{
    float relativeDistance = _stateMachine->getRelativeDistance();
    float currentReferenceVelocity = _stateMachine->getCurrentReferenceVelocity();
    if(_stateMachine->getKeyState() == 0) {
        ChangeState(_stateMachine, OffState::Instance());
        //Reset the acceleration

        return true;
    }
    else if(_stateMachine->getGeneralOutputState() == 0) {
        //To Auto Start state//initialize
        ChangeState(_stateMachine, ManualState::Instance());
        return true;
    }
    else if(currentReferenceVelocity < StateMachine::lowSpeedThresholdVelocity) {
        ChangeState(_stateMachine, AutoLowSpeedState::Instance());
        return true;
    }
    else if (relativeDistance < StateMachine::middleRangeThresholdDistance) {
        ChangeState(_stateMachine, AutoDriveState::Instance());
        return true;
    }
    else if (relativeDistance > StateMachine::virtualLeadThresholdDistance) {
        ChangeState(_stateMachine, VirtualLeadState::Instance());
        return true;
    }
    return false;
}



float MiddleRangeState::GetReferenceAcceleration(float _relativeDistance, int _dt) {
    //Run the Distance Controller
    float referenceAcceleration = distanceController->GetReferenceAcceleration(_relativeDistance, StateMachine::targetThresholdDistance, _dt);
    return referenceAcceleration;
}

float MiddleRangeState::GetReferenceVelocity(float _referenceAcceleration, float _currentReferenceVelocity) {
    float referenceVelocity = distanceController->GetReferenceVelocity(_referenceAcceleration, _currentReferenceVelocity);
    return referenceVelocity;
}

QString MiddleRangeState::getStateName() {
    return "Middle Range Drive State";
}


