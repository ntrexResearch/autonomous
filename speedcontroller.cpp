#include "speedcontroller.h"
#include <QDebug>
#include "common.h"

SpeedController* SpeedController::_instance = nullptr;

SpeedController::SpeedController()
{

}

SpeedController* SpeedController::Instance()
{
    if (_instance == nullptr)
        _instance = new SpeedController();
    return _instance;
}

void SpeedController::destroyInstance()
{
    delete _instance;
    _instance = nullptr;
}

bool SpeedController::hasInstance()
{
    if (_instance)
        return true;
    else
        return false;
}

int SpeedController::calculateVoltageFromSpeed()
{
//    float constant = MAX_VOLTAGE / MAX_MMPS / RPDO_SCALE;
//    qDebug() << constant;
    qDebug() << m_speed;
    int driving_voltage = static_cast<int>(m_speed * CONVERSION_CONSTANT);
    qDebug() << "Driving voltage is " << driving_voltage;

    // Reset the speed changed flag
    m_speedChangedFlag = false;
    return driving_voltage;
}

bool SpeedController::speedChanged() {
    return m_speedChangedFlag;
}

float SpeedController::currentSpeed() const{

    return m_speed;
}

void SpeedController::setCurrentSpeed(float speed){
    m_speed = limit_int(speed, -MAX_MMPS, MAX_MMPS);
    m_speedChangedFlag = true;
}

void SpeedController::incrementSpeed(float speedStep){
    m_speed = limit_int(m_speed + speedStep, -MAX_MMPS, MAX_MMPS);
    m_speedChangedFlag = true;
}
