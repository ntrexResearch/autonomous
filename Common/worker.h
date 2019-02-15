#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QSerialPortInfo>
#include <QList>
#include <QStringList>

class Worker : public QObject
{
    Q_OBJECT
public:
    Worker();
    ~Worker();

public slots:
    void onTimeout();
signals:
    //void finished();
    void error(QString err);
    void portListChanged(QStringList portList);
private:
    QSerialPortInfo info;
    QList<QSerialPortInfo> portList;
    QStringList portStringList;

};

#endif // WORKER_H
