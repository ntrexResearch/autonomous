#include "Common/engine.h"
#include <QDebug>
#include <stdio.h>
#include <iostream>

Engine* Engine::_instance = 0;
Engine::Engine(QObject *_parent) :QObject(_parent)
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(TimerRun()));
    timer_count = 0;

    //Test201804
    //InitKalmanFilter();
    //testPrevDistance= 0;

    //Test case
    //TestCase();

}

//Get the distance input via CAN

void Engine::InitKalmanFilter() {
    //Initialize all the
    //Initialize distance specific or velocity specific parameters
    //in the FSM classes

     timerInterval = 100;
     dt = timerInterval / 1000.0; // Time step
     int n_kf = 2; //Number of states
     int m_kf = 1; //Number of measurement
     Eigen::MatrixXd A(2, 2);//System dynamics matrix
     Eigen::MatrixXd C(m_kf, n_kf);//Output Matrix
     Eigen::MatrixXd Q(n_kf, n_kf); //Process noise covariance
     Eigen::MatrixXd R(m_kf, m_kf); //Measurement noise covariance
     Eigen::MatrixXd P(n_kf, n_kf); // Estimate error covariance

//     A(2, 2);//System dynamics matrix
//     C(m_kf, n_kf);//Output Matrix
//     Q(n_kf, n_kf); //Process noise covariance
//     R(m_kf, m_kf); //Measurement noise covariance
//     P(n_kf, n_kf); // Estimate error covariance

//     // Discrete LTI projectile motion, measuring position only
     A << 1, dt, 0, 1;
     C << 1, 0;

     // Reasonable covariance matrices
     Q << 1, 0, 0, 3;
     R << 3;
     P << 5, 0, 0, 5;


     kf = KalmanFilter(dt, A, C, Q, R, P);


     //Eigen::VectorXd x0(n);
       //x0 << 0, 0;
       kf.init();

       // Feed measurements into filter, output estimated states
}

Engine* Engine::Instance(){
    if(_instance == 0) {
        _instance = new Engine(0);
    }
    return _instance;
}

void Engine::TestCase() {
    const int order = 3;
        Iir::Butterworth::LowPass<order> f;
        const float samplingrate = 1000; // Hz
        const float cutoff_frequency = 5; // Hz
        f.setup (order, samplingrate, cutoff_frequency);

        f.reset ();
        double b;
        for(int i=0;i<10000;i++)
        {
            float a=0;
            if (i==10) a = 1;
            b = f.filter(a);
            //assert_print(!isnan(b),"Lowpass output is NAN\n");
        }
        //fprintf(stderr,"%e\n",b);
        //assert_print(fabs(b) < 1E-30,"Lowpass value for t->inf to high!");


}

float* Engine::getButterWorthVelocity(){
    const int order = 3;
        Iir::Butterworth::LowPass<order> f;
        const float samplingrate = 100; // Hz 0.01 time interval
        const float cutoff_frequency = 3; // Hz
        f.setup (order, samplingrate, cutoff_frequency);

        f.reset ();

        float* mJerkGenPosition = getJerkGenPosition();
        float* mDiffVelocity = (float*) malloc(160* sizeof(float*));
        float* originVel;
        float* originDist;
        float* test = (float*) malloc(160* sizeof(float*));
        float* testOrigin = test;
        originVel = mDiffVelocity;
        originDist = mJerkGenPosition;
        for(int i=0;i<150;i++)
        {

            if(i == 0){
                *mDiffVelocity = 0;

            }
            else{
                qDebug()<< *(mJerkGenPosition) <<"   " << *(mJerkGenPosition -1 );
                *mDiffVelocity = ( *(mJerkGenPosition) - *(mJerkGenPosition -1 ))/0.01;
                qDebug() << i;
                *test = f.filter(*mDiffVelocity);
                mDiffVelocity++;
                mJerkGenPosition++;
                test++;
                //assert_print(!isnan(b),"Lowpass output is NAN\n");
            }
        }
        mDiffVelocity = originVel;
        mJerkGenPosition = originDist;

        return testOrigin;

}
float* Engine::getJerkGenPosition(){
    return mJerkGen.getPosition();
}
float* Engine::getJerkGenVelocity(){
    return mJerkGen.getVelocity();
}

void Engine::getJerkGenKalmanVelocity(float* distance, float* velocity){
//    distance = mJerkGen.getPosition();
//    velocity = (float*) malloc(150 * sizeof(float*));

    float* origin_dist;
    float* origin_vel;

    origin_dist = distance;
    origin_vel = velocity;
    int counter = 0;

    Eigen::VectorXd y(1);
    Eigen::VectorXd result;

    while(counter < 150)  {
        y << *distance;
        kf.update(y);

        result = kf.state();
        *distance = (float) result(0);
        *velocity = (float) result(1);
        distance++;
        velocity++;
        counter++;

    }
    distance = origin_dist;
    velocity = origin_vel;

//    while(counter < 300) {
//        qDebug()<<counter - 150<< "  "<< *distance << " test " <<*velocity<<'\n';
//        counter++;
//        distance++;
//        velocity++;
//    }

    //free(tmp);
}
void Engine::TimerRun() {
    //Run the DistanceFSM every 5 times the timer runs
//    if(timer_count == 1) {
        double currentDistance = TestOneReading(mb);
        //Communicate with the sensor and process the data
        DistanceFSM.setMeasuredDistance(currentDistance);
        //qDebug() << "Distance detected" <<distance <<'\n';
        DistanceFSM.Update();

        //Kalman Test

//        Eigen::VectorXd y(1);
//        y << currentDistance;
//        kf.update(y);
//        //std::cout << "t = " << t << ", " << "y[" << i << "] = " << y.transpose()
//          //  << ", x_hat[" << i << "] = " << kf.state().transpose() << std::endl;
//        Eigen::VectorXd result = kf.state();
//        qDebug()<<currentDistance<<'\n';
//        qDebug() << "Test " << result(0) << " ok "<< result(1)<< '\n';
        //double calculatedVelocity = (currentDistance - testPrevDistance)/dt;
        //qDebug() <<"Calculated velocity " << calculatedVelocity << "\n";
        //testPrevDistance = currentDistance;

        emit addDataToGraph((double) result(1));
        //clear the timer count
//        timer_count = 0;
//    }
//    timer_count ++;


}

//void Engine::addDataToGraph(double velocityData) {

//}


int Engine::MakeConnection(QByteArray comPort) {
    //const char * txt = "\\.\COM10";
    //const char prefix = '\\\\.'.\";
    comPort.prepend("\\\\.\\");
    if(mb != nullptr) {
        qDebug() << "Connection already made\n";
        qDebug() << "Disconnecting...\n";
        modbus_close(mb);
        timer->stop();
        return 2;

    }
    mb = modbus_new_rtu(comPort, 115200, 'N', 8, 1);
    if (mb == nullptr)
    {
        qDebug()<<"Unable to create the libmodbus context\n";
        return 2;
    }

    //modbus_set_debug(mb, true);      // uncomment to view debug info

    // Connects to the sensor:
    if (modbus_connect(mb) != 0)
    {
        modbus_free(mb);
        qDebug()<<"Connection error\n";
        return 3;
    }
    //Start the timer
    timer->start(timerInterval);//

}
