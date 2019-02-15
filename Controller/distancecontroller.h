#ifndef DISTANCECONTROLLER_H
#define DISTANCECONTROLLER_H

#include <QObject>
#include "Common/common.h"
#include "Filter/Iir.h"
//#include <cmath>
class DistanceController : public QObject
{
    Q_OBJECT
public:
    DistanceController();
    float GetReferenceAcceleration(float distance_error, float _targetDistance, int dt);
    float GetReferenceVelocity(float accel, float _currentReferenceVelocity);

    void InitController(float _pGain, float _iGain, float _dGain, int _pidLimit);

private:
    /*
     * Target distance variable
     * Filtered velocity and current distance variable
     *
     *
     */

    //int targetVel;

    float targetVelFl;
    float targetAccel;

    int sat_err;
    char cmdFlag;

    float pGain;
    float iGain;
    float dGain;
    float prevError;

    int maxVelocity;


    float p_gain;
    float i_gain;
    float d_gain;
    float i_error;
    float dt;
    float freq;
    float error_prev;
    int pidLimit;


    Iir::Butterworth::LowPass<MaxOrder> f;

    float referenceAcceleration;
    float referenceAcceleration_k1; //Past value

    void initIIRFilter();
    float prevDistance;
    float filteredVelocity;
};

#endif // DISTANCECONTROLLER_H
