#include "distancecontroller.h"
#include <QDebug>
/*
 * Sequence
 * Read key data from DCU
 * -> Determine the key state (drive state)
 * -> If auto mode is ready, send message to DCU to activate the auto mode
 * -> Call UpdateParameter to read all the input data such as relative distance, velocity
 * -> Call GetTargetVelocity to determine the optimal target velocity or acceleration to drive
 *
 */


DistanceController::DistanceController()
{
    dt = 2;
    initIIRFilter();
}

void DistanceController::InitController(float _pGain, float _iGain, float _dGain, int _pidLimit) {
    pGain = _pGain;
    iGain = _iGain;
    dGain = _dGain;
    pidLimit = _pidLimit;
    i_error = 0;
    //maxVelocity
}

/*
 * Check the init condition as follows : DCU Connection, Sensor Connection, Key State
 * Return false if auto mode is not ready.
 * Return true if auto mode is ready
 */

void DistanceController::initIIRFilter() {
    const int order = 5;
    const float samplingrate = 100; // Hz 0.01 time interval
    const float cutoff_frequency = 3; // Hz
    f.setup (order, samplingrate, cutoff_frequency);

    f.reset ();
    //
    prevDistance = 0;
}

float DistanceController::GetReferenceAcceleration(float _relativeDistance, float _targetDistance, int _dt) {

    dt = _dt;

     /*
     * Calculate the relative velocity
     * Calculate the relative distance difference
     */
    float relativeDistanceError = _relativeDistance - prevDistance;
    prevDistance = _relativeDistance;
    float relativeVelocity;
    if (dt <= 0)
        relativeVelocity = 0;
    else
        relativeVelocity = relativeDistanceError / dt;
    /*
     * Calculate the relative filtered velocity
     *
     * err_d = LeadingVelocity - HostVelocity
     */
//    if(relativeVelocity != 0){
//        filteredVelocity = f.filter(relativeVelocity);

//    }
//    else
//        filteredVelocity = 0;
    float distance_error = _relativeDistance - _targetDistance;
    //Limit
    //float out_prev = out;
    //bool LimitRef = Limit_int(&out, pidLimit, -pidLimit );
    //round up
    referenceAcceleration = pGain * distance_error + dGain * relativeVelocity;// + i_error;

//    sat_err = referenceAcceleration_k1 - referenceAcceleration;

//    //i error
//    //Cut off the saturation error
////    if (sat_err > 0 && distance_error > 0);
////    else if(sat_err < 0 && distance_error < 0);
////    else i_error +=  iGain * distance_error;
    return referenceAcceleration;
}

float DistanceController::GetReferenceVelocity(float referenceAcceleration, float currentReferenceVelocity) {
    targetVelFl = currentReferenceVelocity + referenceAcceleration * 2;
    if (targetVelFl < 0 )
        targetVelFl = 0;
    else if (targetVelFl > 1700)
        targetVelFl = 1700;


    return targetVelFl;
}


