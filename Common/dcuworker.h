#ifndef DCUWORKER_H
#define DCUWORKER_H

#include <QObject>
#include <QByteArray>
#include "Communication/deviceserial.h"
#include "Communication/busserial.h"
//#include "Controller/distancecontroller.h"
#include <QDebug>
#include "debugger.h"
#include <QQueue>

#include <QtXlsx>
#include <xlsxformat.h>
#include <xlsxcellrange.h>
#include <xlsxdocument.h>
#include <xlsxworksheet.h>
#include <QDateTime>

QTXLSX_USE_NAMESPACE

class DCUWorker :public QObject
{
    Q_OBJECT
public:
    DCUWorker();
    ~DCUWorker();
    void SetDebugger(Debugger* _debugger);
    void WriteThinBorderStyleCell(Document* xlsx, const QString &cell, const QString &text);
    static int autoCommandValue;

public slots:
    void onTimeout();
    void connectTriggered(QByteArray comPort);
    DeviceSerial* getDevSerialInstance();
    //void UpdateReferenceVelocity(int refVel);
    void UpdateSensorConnectionStatus(bool);

    void StartRecording();
    void StopRecording();

    void UpdateData(float _relativeDistance, float _errorDistance,
                    float _refAccel, float _refVelocity);
    void StartAutoMode();
    void StopAutoDriving();

signals:
    void DisconnectDCU();


private:
    DeviceSerial *_DevSerial;
    BusSerial *_BusSerial;
    bool DCUConnected;
    bool SensorConnected;
    int errorCnt;

    int endErrorCnt;

    int targetVelocity;
    float refVelocity;
    float targetAcceleration;
    float currentSpeed;
    float relativeDistance;
    float errorDistance;

    char emergencyCnt;
    char emergencyOffCnt;

    int targetTime;

    bool velocityCmdFlag;
    unsigned char command;
    Debugger* debugger;

    QQueue <QString> commandQueue;
    //QQueue <QString> buttonCommandQueue;

    QByteArray txData;


    QXlsx::Document *xlsx;
    int documentID;
    bool recordFlag;

    bool autoModeFlag;

    bool stopAutoDriveFlag;
private:

    int count;
    Format format;



};

#endif // DCUWORKER_H
