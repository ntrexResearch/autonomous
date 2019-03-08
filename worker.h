#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QList>
#include <QStringList>
#include <QDebug>

class Worker : public QObject
{
    Q_OBJECT
public:
    Worker();
    ~Worker();

public slots:
    virtual void onTimeout();

signals:
    //void finished();
    void error(QString err);

private:
    QStringList portStringList;

};

#endif // WORKER_H
