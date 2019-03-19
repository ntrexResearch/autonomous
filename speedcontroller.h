#ifndef SPEEDCONTROLLER_H
#define SPEEDCONTROLLER_H

#define MOTOR_RPM 1300
#define REDUCTION_RATE 16
#define WHEEL_SIZE 531
#define MAX_INPUT 32767
#define LIMIT_MMPS 2222
#define MAX_VOLTAGE 10
#define RPDO_SCALE 0.003906356
#define MAX_MMPS 1668.185352 //(MOTOR_RPM / 60.0 / REDUCTION_RATE) * WHEEL_SIZE * 3.141592
#define CONVERSION_CONSTANT MAX_VOLTAGE / MAX_MMPS / RPDO_SCALE

class SpeedController
{
public:
//    enum Direction {
//        None = 0,
//        Backward,
//        Forward
//    };

    static SpeedController* Instance();
    static void destroyInstance();
    static bool hasInstance();

    //enum Direction currentDirection() const;
    //void setCurrentDirection(Direction direction);

    float currentSpeed() const;
    void setCurrentSpeed(float speed);
    void incrementSpeed(float speedStep);
    bool speedChanged();
    int calculateVoltageFromSpeed();
protected:
    SpeedController();

private:
    static SpeedController* _instance;
    float m_speed;
    bool m_speedChangedFlag = false;

//    QStateMachine *m_machine;

};

#endif // SPEEDCONTROLLER_H
