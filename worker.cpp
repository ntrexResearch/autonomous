#include "worker.h"

Worker::Worker()
{

}

Worker::~Worker()
{

}

void Worker::onTimeout() {

    qDebug()<<"Thread";
}

void Controller::handleResults(const QString &test){
    qDebug() << test;
}
