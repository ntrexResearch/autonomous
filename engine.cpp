#include "engine.h"

Engine* Engine::_instance = nullptr;

Engine::Engine(QObject *_parent) : QObject(_parent)
{
    // Initialize the engine
    initialize();
}

void Engine::initialize()
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(run()));
    timer->start(100);
}


/*
 * Engine Instance access function
 */
Engine* Engine::Instance(QObject* _parent)
{
    //Engine Instance's parent is the mainwindow qobject
    if (_instance == nullptr)
        _instance = new Engine(_parent);
    return _instance;
}


/*
 * Engine main routine is included here.
 */
void Engine::run()
{
    //
    qDebug() << "Test";
}
