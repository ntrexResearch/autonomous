#include "manualstate.h"
#include "offstate.h"
#include "autolowspeedstate.h"

ManualState ManualState::_ManualState;

ManualState::ManualState()
{

}

ManualState::~ManualState()
{

}

void ManualState::Init(float gainData[4][3], int _pidLimit){

}

ManualState* ManualState::Instance() {
    return{&_ManualState};
}

bool ManualState::Update(StateMachine *_StateMachine) {
    //Based on the key state
    //When it is manual state, do not do anything.
    //Every velocity and operations will be dealt with DCU

    if(_StateMachine->getKeyState() == 0) {
        ChangeState(_StateMachine, OffState::Instance());
        return true;
    }
    else if(_StateMachine->getGeneralOutputState() == 1) {
        //To Auto Start state//initialize
        ChangeState(_StateMachine, AutoLowSpeedState::Instance());
        return true;
    }
    return false;

}
float ManualState::GetReferenceAcceleration(float _relativeDistance, int dt) {
    return 0;
}

float ManualState::GetReferenceVelocity(float _referenceAcceleration, float _currentReferenceVelocity) {
    return 0;
}

QString ManualState::getStateName(){
    return "Manual State";
}
