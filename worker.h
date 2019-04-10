#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QList>
#include <QStringList>
#include <QDebug>
#include <QThread>

class Worker : public QObject
{
    Q_OBJECT
public:
    Worker();
    ~Worker();

public slots:
    virtual void onTimeout();
    void doWork(const QString &param) {
        qDebug() <<param;
        QString result;
        qDebug() << "Different Thread" << QThread::currentThread();
        emit resultReady(result);
    }

signals:
    //void finished();
    void resultReady(const QString &result);
    void error(QString err);

private:
    QStringList portStringList;

};

class Controller : public QObject
{
    Q_OBJECT
    QThread workerThread;

public:
    Controller() {
        Worker *worker = new Worker;
        worker->moveToThread(&workerThread);
        //qDebug() << "Test";
        connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
        connect(this, &Controller::operate, worker, &Worker::doWork);
        connect(worker, &Worker::resultReady, this,&Controller::handleResults);
        workerThread.start();
        emit operate("Hello world thread");
    }
    void test() {
        qDebug() <<"whatup";
    }
    ~Controller() {
        workerThread.quit();
        workerThread.wait();
    }
public slots:
    void handleResults(const QString &);

signals:
    void operate(const QString &);
};

#endif // WORKER_H
