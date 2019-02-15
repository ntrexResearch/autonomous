#include "statemachine.h"
#include "offstate.h"

float StateMachine::emergencyThresholdDistance = 8;
float StateMachine::virtualLeadThresholdDistance = 40;
float StateMachine::targetThresholdDistance = 10;
float StateMachine::middleRangeThresholdDistance = 15;
float StateMachine::lowSpeedThresholdVelocity = 500;
bool StateMachine::emergencyBellFlag = false;

StateMachine::StateMachine()
{
    //Velocity and other commands according to the current state
    _state = OffState::Instance();
    setCurrentVelocity(0);
    setRelativeVelocity(0);
    setRelativeDistance(0);//?Initialize it to zero?
    //Off Key State
    setKeyState(0);
    /*
     * For debugging
     */
    _state->enableDebug(false);
    //Initialize
    generalOutput = 0;
    keyState = 0;
    referenceVelocity = 0;

    for(int i = 0; i < 4; i ++){
        for (int j = 0; j < 3; j++){
            gainData[i][j] = 0;
        }
    }
    //
    //Initialize Gain
    //
    //Gain for auto drive state
    gainData[0][0] = 0.03;
    gainData[0][1] = 0;
    gainData[0][2] = 4500;
    //Gain for emergency drive state
    gainData[1][0] = 20;
    gainData[1][1] = 0;
    gainData[1][2] = 5000;
    //Gain for low speed state
    gainData[2][0] = 3;
    gainData[2][1] = 0;
    gainData[2][2] = 5000;
    //Gain for middle range state
    gainData[3][0] = 0.25;
    gainData[3][1] = 0;
    gainData[3][2] = 5000;

}

void StateMachine::ChangeState(State *state) {
    _state = state;
}

State* StateMachine::GetStateInstance() {
    return _state;
}

void StateMachine::setCurrentVelocity(float curVel) {
    _currentVelocity = curVel;
}

void StateMachine::setRelativeVelocity(float relVel) {
    _relativeVelocity = relVel;
}

void StateMachine::setRelativeDistance(float relDist) {
    relativeDistance = relDist;
}

void StateMachine::setKeyState(char _keyState) {
    keyState = _keyState;
}

void StateMachine::setDirState(char _dirState) {
    dirState = _dirState;
}

void StateMachine::setBrakeState(char _brakeState) {
    brake = _brakeState;
}

void StateMachine::setEmergencyState(char _emergenState) {
    emergency = _emergenState;
}

char StateMachine::getKeyState() {
    return keyState;
}

char StateMachine::getDirState() {
    return dirState;
}

char StateMachine::getGeneralOutputState() {
    return generalOutput;
}

char StateMachine::getBrakeState() {
    return brake;
}

char StateMachine::getEmergencyState() {
    return emergency;
}

float StateMachine::getCurrentVelocity() {
    return _currentVelocity;
}

float StateMachine::getRelativeVelocity() {
    return relativeVelocity;
}

float StateMachine::getRelativeDistance() {
    return relativeDistance;
}

void StateMachine::setCurrentReferenceVelocity(float _referenceVelocity) {
    referenceVelocity = _referenceVelocity;
}

float StateMachine::getCurrentReferenceVelocity() {
    return referenceVelocity;
}

void StateMachine::UpdateStateParameters(char data) {
    setKeyState((data >> 1) & 1);
//    _state->setDirState((data >> 4) & 3);
//    _state->setBrakeState((data >> 3) & 1);
}

void StateMachine::UpdateGeneralOutputData(char data) {
    generalOutput = data;
}

/*
 * It is called asynchronously when dcu thread receives new status data
 */
void StateMachine::UpdateData(float _relativeDistance, int _dt) {
    relativeDistance = _relativeDistance;
    dt = _dt;
}

void StateMachine::UpdateGainData(float *_pGain, float *_iGain, float *_dGain,
                                  int _pidLimit) {
    float *headP = _pGain;
    float *headI = _iGain;
    float *headD = _dGain;
    for(int i = 0; i < 4; i ++){

        gainData[i][0] = *_pGain++;
        gainData[i][1] = *_iGain++;
        gainData[i][2] = *_dGain++;
    }
    _pGain = headP;
    _iGain = headI;
    _dGain = headD;
    pidLimit = _pidLimit;    
}

char StateMachine::GetKeyState() {

    return keyState;
}

/*
 * Based on the distance, state is updated.
 * Then, each state initializes itself for controller setup (pid gain, etc)
 * Afterwards, reference accel is computed
 */
void StateMachine::GetReferenceAcceleration(float _relativeDistance, int _dt) {
    relativeDistance = _relativeDistance;
    dt = _dt;
    //When the state changes, initialize the new state
    if(_state->Update(this))
        _state->Init(gainData, pidLimit);
    referenceAcceleration = _state->GetReferenceAcceleration(_relativeDistance, _dt);
}

/*
 * The statemachine computes the reference velocity based on the current state.
 * The reference velocity to run is updated every time continuously
 */
void StateMachine::GetReferenceVelocity() {
    referenceVelocity = _state->GetReferenceVelocity(referenceAcceleration, referenceVelocity);
    emit UpdateControllerData(relativeDistance, relativeDistance- targetThresholdDistance,
                              referenceAcceleration, referenceVelocity);
}
