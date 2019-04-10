#include "steermanager.h"
#include "Common/common.h"
#include <QDebug>

SteerManager* SteerManager::_instance = nullptr;

SteerManager::SteerManager()
{
    //When initialized, check the m_pidController instance.
    m_centerSteer = 2920; // From experiment
    // If m_pidcontroller exists, delete it and recreate it
    if (m_pidController) {
        qDebug() << "PID Controller existed. \n Start a new instance of controller";
        delete m_pidController;
    }
    m_pidController = new PIDController();

}



SteerManager* SteerManager::Instance(){
    if (_instance == nullptr)
        _instance = new SteerManager();
    return _instance;
}

void SteerManager::destroyInstance()
{
    if (_instance == nullptr)
        return;

    //delete m_pidController;
    delete _instance;
    _instance = nullptr;
}

//
SteerManager::~SteerManager() {
    delete m_pidController;
}

bool SteerManager::hasInstance()
{
    if (_instance)
        return true;
    else
        return false;
}

void SteerManager::calculateCurrentSteerAngle(int single_turn_cnt, int multi_turn_cnt)
{
    int steer_pos = single_turn_cnt + 1024 * multi_turn_cnt;
    m_currentSteer = steer_pos - m_centerSteer;
    //m_currentSteerAngle = static_cast<double>(steer_pos / STEER_SCALE_CONSTANT) ;
    //qDebug() << "Steer angle received" << m_currentSteer;
}

void SteerManager::setPIDGain(float kp, float ki, float kd)
{
    if (m_pidController) {
        m_pidController->setGains(kp, ki, kd);
    }
}


void SteerManager::setCurrentSteer(int steer)
{
    m_currentSteer = steer;
}

int SteerManager::getCurrentSteer() {
    return m_currentSteer;
}

void SteerManager::setTargetSteer(int target){
    m_targetSteer = target;
}

int SteerManager::getTargetSteer(){
    return m_targetSteer;
}

void SteerManager::setTargetVoltage(float target){
    m_targetVoltage = target;
}

float SteerManager::getTargetVoltage(){
    return m_targetVoltage;
}

void SteerManager::setBrakeVoltage(float voltage) {
    m_brakeVoltage = voltage;
}
float SteerManager::getBrakeVoltage() {
  return m_brakeVoltage;
}

void SteerManager::runPIDController(float step_time)
{
    m_targetVoltage = m_pidController->calculate(m_targetSteer, m_currentSteer, step_time);
}

void SteerManager::setTargetSteerAngle(double steerAngle)
{
    m_targetSteerAngle = steerAngle;
}

void SteerManager::incrementTargetSteerAngle(float stepAngle) {
    m_targetSteer += stepAngle;
    qDebug() << "Current Target Steer " << m_targetSteer;

}

void SteerManager::setCurrentSteerAngle(double steerAngle)
{
    m_currentSteerAngle = steerAngle;
}

double SteerManager::getTargetSteerAngle()
{
    return m_targetSteerAngle;
}

double SteerManager::getCurrentSteerAngle()
{
    return m_currentSteerAngle;
}

double SteerManager::getOutputVoltage()
{
    return m_outputVoltage;
}
void SteerManager::setOutputVoltage(double outputVoltage)
{
    m_outputVoltage = outputVoltage;
}
