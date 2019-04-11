#include "engine.h"
#include "Common/common.h"
#include <QThread>

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
    timer->start(1);
    elapsedTimer.start();
    m_mutex = new QMutex();
    // Create the CAN thread with a shared mutex object
    canThread = new CanThread(m_mutex);
    //shared_number_test =
    number = 30;
    shared_number_test = &number;
    //qDebug() <<"SDfasdfas;dfj";
    qDebug() <<"Init " << *shared_number_test;
    canThread->setSharedNumber(shared_number_test);
    canThread->start();

    m_serialManager = new SerialManager;


    //testThreadController = new Controller;
    //testThreadController->test();
    my_number_test = 2;

}

Engine::~Engine()
{
    delete _instance;

    delete canThread;

    delete timer;
    delete m_mutex;

    delete m_serialManager;
}

void Engine::setMutex(QMutex *mutex){
    m_mutex = mutex;
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
 * State machine
 */
void Engine::run()
{


    step_time = elapsedTimer.elapsed();
    //elapsedTimer.
    // Run the steer controller that takes input of the absolute angle and outputs the voltage
    m_mutex->lock();
    if (SteerManager::hasInstance())
    {

        //SteerManager::Instance()->runPIDController(step_time);
        // Set the mutex

    }else {

    }

    if (m_serialManager->isOpen()){


        //qDebug() << "Size of the buffer " << m_serialManager->getTxBufferSize();
        if (m_serialManager->getTxBufferSize() > 0){
            m_serialManager->Send(m_serialManager->popTxMsg());
        }
    }
    m_mutex->unlock();

    // Start measuring the time
    elapsedTimer.start();
}

SerialManager *Engine::getSerialManager()
{
    return m_serialManager;
}

void Engine::openBrakeDriverSerial(QString port)
{
    if ( ! m_serialManager->isOpen() )
        m_serialManager->Open(port, QSerialPort::Baud115200);
}
void Engine::enqueueBrakeTxMsg(QString msg)
{
    if ( m_serialManager->isOpen() )
        m_serialManager->addTxMsg(msg);
}

void Engine::openSensorSerial(QString port)
{
    port = "test";
}

void Engine::setSharedNumber(int* number) {
    shared_number_test = number;
}
void Engine::setMyNumber(int number)
{
    my_number_test = number;
}
