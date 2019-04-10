#include "steercontroller.h"

SteerController* SteerController::_instance = nullptr;

SteerController::SteerController()
{

}

SteerController* SteerController::Instance(){
    if (_instance == nullptr)
        _instance = new SteerController();
    return _instance;
}

void SteerController::destroyInstance()
{
    if (_instance == nullptr)
        return;
    delete _instance;
    _instance = nullptr;
}

bool SteerController::hasInstance()
{
    if (_instance)
        return true;
    else
        return false;
}

void SteerController::calculateOutputVoltage() {
    // Find the current steering
    error_p = m_targetSteerAngle - m_currentSteerAngle;
    error_i = error_i + error_p * m_stepTime;
    //error_d = (error_)
}


void SteerController::setSteerAngle(float steerAngle){

}

void SteerController::incrementSteerAngle()
{

}
