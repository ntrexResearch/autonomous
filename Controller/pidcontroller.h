#ifndef PIDCONTROLLER_H
#define PIDCONTROLLER_H

#define PID_MAX 100.0
#define PID_MIN -100.0
#define PID_KP 1.0f
#define PID_KD 0.1f
#define PID_KI 0.1f

class PIDImpl;
class PIDController
{
public:
    // Kp - proportional gain
    // Ki - integral gain
    // Kd - derivative gain
    // dt - loop interval time
    // max - maximum value of manipulated variable
    // min - minimum value of manipulated variable
    PIDController(float max = PID_MAX, float min = PID_MIN, float Kp = PID_KP
            , float Kd = PID_KD, float Ki = PID_KI);

    void setGains(float Kp, float Ki, float Kd);

    // Returns the manipulated variable given a target point and current process value
    float calculate( int setpoint,int pv, float dt);
    ~PIDController();

private:
    PIDImpl *pimpl;
};

#endif // PIDCONTROLLER_H
