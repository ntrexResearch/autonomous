#ifndef STATES_H
#define STATES_H

#include <QState>
#include <QSignalTransition>
#include <QKeyEventTransition>
#include <QSet>
#include "../Communication/canmanager.h"


class QStateMachine;

class VehicleState : public QState{
public:
    explicit VehicleState(CanManager *manager, QState *parent=0);
    ~VehicleState();

protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;

private:
    // member variable..?
    QStateMachine *m_machine;
    CanManager *m_manager;
    int currentVehicleMode;
    // int current distance or current_angle
    friend class ManualState;
    friend class AutoOnState;
    friend class AutoReadyState;


};

class ManualState : public QState
{
public:
    ManualState(CanManager *manager, VehicleState *state, QState *parent=0);

protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;

private:
    //Here disable the brake and steering motors
    // There should not be any Driving speed control control in this mode
    void initializeManualState();
    //Beside the init action, nothing should be done.
    // Just look for the transition steps
    VehicleState *m_state;
    CanManager *m_manager;

};

//class AutoReadyState : public QState
//{
//public:
//    AutoReadyState(VehicleState *state, QState *parent = 0);

//protected:
//    void onEntry(QEvent *) override;
//    void onExit(QEvent *) override;

//private:
//    VehicleState *m_state;
//};


//In this auto on state, I will control the speed and the steering motor
//
class AutoOnState : public QState
{
public:
    AutoOnState(CanManager *manager, VehicleState *state, QState *parent=0);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    VehicleState *m_state;
    CanManager *m_manager;

};

class OffState : public QState
{
public:
    OffState(QState *parent=0);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
};

class TestState : public QState
{
public:
    TestState(QState *parent=0);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
};
#endif // STATES_H
