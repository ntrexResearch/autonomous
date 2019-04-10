#ifndef STEERMANAGER_H
#define STEERMANAGER_H
#include "Controller/pidcontroller.h"

class SteerManager
{
public:
    static SteerManager* Instance();

    static void destroyInstance();
    static bool hasInstance();

    ~SteerManager();

    void setTargetSteerAngle(double steerAngle);
    void incrementTargetSteerAngle(float stepAngle);
    void setCurrentSteerAngle(double steerAngle);

    void setCurrentSteer(int steer);
    int getCurrentSteer();

    void setTargetSteer(int target);
    int getTargetSteer();

    void setPIDGain(float kp, float ki, float kd);

    void setTargetVoltage(float target);
    float getTargetVoltage();

    void setBrakeVoltage(float voltage);
    float getBrakeVoltage();

    double getTargetSteerAngle();
    double getCurrentSteerAngle();

    void runPIDController(float step_time);

    void calculateCurrentSteerAngle(int single_turn_cnt, int multi_turn_cnt);

    double getOutputVoltage();
    void setOutputVoltage(double outputVoltage);

protected:
    SteerManager();

private:
    static SteerManager* _instance;

    PIDController *m_pidController = nullptr;

    int m_centerSteer;

    float m_targetVoltage;
    int m_currentSteer;
    int m_targetSteer = 0;

    float m_brakeVoltage= 0;

    double m_outputVoltage;
    double m_currentSteerAngle;
    double m_targetSteerAngle;


};

#endif // STEERMANAGER_H
