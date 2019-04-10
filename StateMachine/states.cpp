#include "states.h"

#include <QStateMachine>
#include <QDebug>
#include "speedmanager.h"
#include "steermanager.h"

VehicleState::VehicleState(CanManager *manager, QState *parent)
    : QState(parent),
      m_machine(0),
      m_manager(manager),
      currentVehicleMode(0)
{

}

VehicleState::~VehicleState()
{
    delete m_machine;
}

void VehicleState::onEntry(QEvent *)
{
    qDebug() <<"Hello";
    // If the machine was on, stop it and restart
    if (m_machine) {

        m_machine->stop();
        // Display the statemachine information
        //Here !
        qDebug() <<"Delete old machine and restart";

        //For this , some kind of mode prep is needed
        currentVehicleMode = 0;
        delete m_machine;

    }

    m_machine = new QStateMachine;

    // This state is when the vehicle is driven manually
    ManualState *manualState = new ManualState(CanManager::Instance(), this, m_machine);

    // Initially the vehicle is in manual state
    QState *vehicleState = new QState(manualState);
    manualState->setInitialState(vehicleState);

    //
    AutoOnState *autoOnState = new AutoOnState(CanManager::Instance(), this, m_machine);

    manualState->addTransition(CanManager::Instance(), &CanManager::autoModeOn, autoOnState);

    autoOnState->addTransition(CanManager::Instance(), &CanManager::manualModeOn, manualState);

    //We have one view, it receive the key press event
//    QKeyEventTransition *pressPauto= new QKeyEventTransition(this, QEvent::KeyPress, Qt::Key_P);
//    pressPauto->setTargetState(autoOnState);
    QKeyEventTransition *pressPmanual = new QKeyEventTransition(this, QEvent::KeyPress, Qt::Key_P);
    pressPmanual->setTargetState(manualState);

    //vehicleState->addTransition(pressPmanual);

    //vehicleState->setTransition(pressPauto);
    //manualState->addTransition(pressPauto);
    //AutoReadyState *autoReadyState = new AutoReadyState(this, m_machine);
    //qDebug()<< manualState;
    //m_machine->addState(manualState);
    m_machine->setInitialState(manualState);
    m_machine->start();
    //
    //
}

void VehicleState::onExit(QEvent *)
{
    if (SpeedManager::hasInstance()){
        SpeedManager::destroyInstance();
    }
}

ManualState::ManualState(CanManager *manager, VehicleState *state, QState *parent) :
    m_manager(manager), QState(parent), m_state(state)
{

}

void ManualState::onEntry(QEvent *)
{
    if (SpeedManager::hasInstance()){
        SpeedManager::destroyInstance();
    }

    SpeedManager::Instance();

    if (SteerManager::hasInstance()) {
      SteerManager::destroyInstance();
    }
    SteerManager::Instance();
    // Also create manual controller

    //When the mode changes, recreate the controller objects


    // In this state, diable the brake and steering motor
    qDebug() << "Now on manual state";
}

void ManualState::onExit(QEvent *)
{

    SpeedManager::destroyInstance();
    qDebug() <<"NOw exiting manual state";
}


AutoOnState::AutoOnState(CanManager *manager, VehicleState *state, QState *parent) :
    m_manager(manager), QState(parent), m_state(state)
{

}

void AutoOnState::onEntry(QEvent *)
{
    // In this state, enable the brake and steering motor
    // Also run controller for steering motor
    // As well as driving speed controller based on the distance

    //But for now, receive user key input for speed control
    // Just open the key input manager or start it.
    qDebug() << "Now on Auto on State";
}

void AutoOnState::onExit(QEvent *)
{
    qDebug() << "Now exiting auto on state";
}

OffState::OffState(QState *parent)
    : QState(parent)
{

}

void OffState::onEntry(QEvent *)
{
    qDebug() << " Now on Off state";
}

void OffState::onExit(QEvent *)
{
    qDebug() <<" Now exiting Off State";
}


TestState::TestState(QState *parent)
    : QState(parent)
{

}

void TestState::onEntry(QEvent *)
{
    qDebug() << " Now on Test state";
}

void TestState::onExit(QEvent *)
{
    qDebug() <<" Now exiting Test State";
}
