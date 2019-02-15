#include "vehiclestate.h"


State::State()
{
    debug = true;
}

State::~State()
{

}

void State::ChangeState(StateMachine *stateMachine, State *state) {
    stateMachine->ChangeState(state);
}

void State::enableDebug(bool _debug) {
    debug = _debug;
}

