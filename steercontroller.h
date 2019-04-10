#ifndef STEERCONTROLLER_H
#define STEERCONTROLLER_H


class SteerController
{
public:
    static SteerController* Instance();
    static void destroyInstance();
    static bool hasInstance();

    void setSteerAngle(float steerAngle);
    void incrementSteerAngle();
    void calculateOutputVoltage();

protected:
    SteerController();

private:
    static SteerController* _instance;

    float m_currentSteerAngle;
    float m_targetSteerAngle;

    float p_gain, i_gain, d_gain;
    float error_p, error_i, error_d;
    float m_stepTime;
};

#endif // STEERCONTROLLER_H
