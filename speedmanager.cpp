#include "speedmanager.h"
#include <QDebug>
#include "Common/common.h"

SpeedManager* SpeedManager::_instance = nullptr;

SpeedManager::SpeedManager()
{

}

SpeedManager* SpeedManager::Instance()
{
    if (_instance == nullptr)
        _instance = new SpeedManager();
    return _instance;
}

void SpeedManager::destroyInstance()
{
    if (_instance == nullptr)
        return;
    delete _instance;
    _instance = nullptr;
}

bool SpeedManager::hasInstance()
{
    if (_instance)
        return true;
    else
        return false;
}

int SpeedManager::calculateVoltageFromSpeed()
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

bool SpeedManager::speedChanged() {
    return m_speedChangedFlag;
}

float SpeedManager::currentSpeed() const{

    return m_speed;
}

void SpeedManager::setCurrentSpeed(float speed){
    m_speed = limit_int(speed, -MAX_MMPS, MAX_MMPS);
    m_speedChangedFlag = true;
}

void SpeedManager::incrementSpeed(float speedStep){
    m_speed = limit_int(m_speed + speedStep, -MAX_MMPS, MAX_MMPS);
    m_speedChangedFlag = true;
}
