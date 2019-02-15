#include "offstate.h"
#include "manualstate.h"
#include "autolowspeedstate.h"

OffState OffState::_OffState;

OffState::OffState()
{

}

OffState::~OffState()
{

}

void OffState::Init(float gainData[4][3], int _pidLimit) {

}
OffState* OffState::Instance() {
    return {&_OffState};
}

bool OffState::Update(StateMachine *_StateMachine) {
    if(_StateMachine->getKeyState() == 1 && _StateMachine->getGeneralOutputState() == 0 ) {

        ChangeState(_StateMachine, ManualState::Instance());
        return true;
    }
    else if(_StateMachine->getKeyState() == 1 && _StateMachine->getGeneralOutputState() == 1) {
        //To Auto Start state//initialize
        ChangeState(_StateMachine, AutoLowSpeedState::Instance());
        return true;
    }
    return false;
}

float OffState::GetReferenceAcceleration(float _relativeDistance, int dt){
    return 0;
}

float OffState::GetReferenceVelocity(float _referenceAcceleration, float _currentReferenceVelocity) {
    return 0;
}

QString OffState::getStateName()
{
    return "Off State";
}
