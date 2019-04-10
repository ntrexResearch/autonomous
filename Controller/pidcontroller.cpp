#include "pidcontroller.h"
#include <iostream>
#include <cmath>

using namespace std;

class PIDImpl
{
public:
    PIDImpl(float max , float min, float Kp, float Kd, float Ki);
    ~PIDImpl();
    float calculate( int setpoint,int pv, float dt);
    void setGains(float Kp, float Ki, float Kd);

private:
    float _max;
    float _min;
    float _Kp;
    float _Kd;
    float _Ki;
    float _pre_error;
    float _integral;
};


PIDController::PIDController(float max, float min, float Kp, float Kd, float Ki)
{
    pimpl = new PIDImpl(max, min, Kp, Kd, Ki);
}

float PIDController::calculate( int setpoint,int pv, float dt)
{
    return pimpl->calculate(setpoint, pv, dt);
}

void PIDController::setGains(float Kp, float Ki, float Kd)
{
    pimpl->setGains(Kp, Ki, Kd);
}


PIDController::~PIDController()
{
    delete pimpl;
}

/*
 * Implementation
 */

PIDImpl::PIDImpl( float max, float min, float Kp, float Kd, float Ki) :
    _max(max),
    _min(min),
    _Kp(Kp),
    _Kd(Kd),
    _Ki(Ki),
    _pre_error(0),
    _integral(0)
{

}

float PIDImpl::calculate( int setpoint,int pv, float dt)
{
    // Calculate error
    int error = setpoint - pv;

    // Proportional term
    float Pout = _Kp * error;

    // Integral term
    _integral += error * dt;
    float Iout = _Ki * _integral;

    float derivative;
    float Dout = 0.0;
    // Derivative term
    if (dt != 0.0f) {
        //("Division error: step time is zero");
       derivative = (error - _pre_error);
       Dout = _Kd * derivative;
    }

    // Calculate the total output
    float output = Pout + Iout + Dout;

    // Restrict to max/min
    if ( output > _max)
        output = _max;
    else if (output < _min)
        output = _min;

    //Save error to previous error
    _pre_error = error;

    return output;

}



void PIDImpl::setGains(float Kp, float Ki, float Kd)
{
    _Kp = Kp;
    _Ki = Ki;
    _Kd = Kd;
}


PIDImpl::~PIDImpl()
{

}
