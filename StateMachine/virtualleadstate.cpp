#include "virtualleadstate.h"
#include "emergencystate.h"
#include "middlerangestate.h"
#include "manualstate.h"
#include "offstate.h"

VirtualLeadState VirtualLeadState::virtualLeadState;

VirtualLeadState::VirtualLeadState()
{
    constantAcceleration = 20;
    marginVelocity = 20;
}

VirtualLeadState::~VirtualLeadState()
{

}

void VirtualLeadState::Init(float _gainData[4][3], int _pidLimit)
{
    //distanceController->InitController(_gainData[2][0], _gainData[2][1], _gainData[2][2], _pidLimit, _targetDistance);
}

VirtualLeadState* VirtualLeadState::Instance()
{
    return {&virtualLeadState};
}

bool VirtualLeadState::Update(StateMachine * _stateMachine)
{
    float relativeDistance = _stateMachine->getRelativeDistance();
    /*
     * Based on the relative distance, switch back to auto state
     */
    if(_stateMachine->getKeyState() == 0) {
        ChangeState(_stateMachine, OffState::Instance());
        return true;
    }
    else if(_stateMachine->getGeneralOutputState() == 0) {
        //To Auto Start state//initialize
        ChangeState(_stateMachine, ManualState::Instance());
        return true;
    }
    /*
     * Object is detected within the range
     */
    else if (relativeDistance < StateMachine::virtualLeadThresholdDistance) {
        ChangeState(_stateMachine, MiddleRangeState::Instance());
        return true;
    }
    return false;
}

float VirtualLeadState::GetReferenceAcceleration(float _relativeDistance, int _dt)
{
    return 0;
}

float VirtualLeadState::GetReferenceVelocity(float _referenceAcceleration, float _currentReferenceVelocity)
{
    if (_currentReferenceVelocity >= maxVelocity + marginVelocity) {
        return _currentReferenceVelocity - constantAcceleration;
    }
    else if(_currentReferenceVelocity <= maxVelocity - marginVelocity ){
        return _currentReferenceVelocity + constantAcceleration;
    }
    return _currentReferenceVelocity;
}

QString VirtualLeadState::getStateName() {
    return "Virtual Lead State";
}
