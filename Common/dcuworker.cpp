#include "dcuworker.h"
#include <QBuffer>
#include <QtEndian>

int DCUWorker::autoCommandValue = 0;
DCUWorker::DCUWorker()
{
    _BusSerial = new BusSerial(20);
    _DevSerial = new DeviceSerial(_BusSerial);
    //_DistanceController = new DistanceController(_DevSerial);

    errorCnt = 0;
    endErrorCnt = 0;
    velocityCmdFlag = false;
    commandQueue.append("sr\r\n");
    //This is wrong
    targetTime = 4;
    refVelocity = 0;
    //Read current velocity
    commandQueue.append("cvl\r\n");
    commandQueue.append("gov1\r\n");
    commandQueue.append(QString("gvt= %1, %2\r\n").arg(refVelocity).arg(targetTime));

    SensorConnected = false;
    count = 0;

    //Recording document
    documentID = 1;
    recordFlag = false;
    xlsx = nullptr;
    //Initialize auto mode --> turn it off
    autoModeFlag = true;
    //When button is pressed, stop auto driving
    stopAutoDriveFlag = false;
    emergencyCnt = 0;
}


DCUWorker::~DCUWorker()
{

}

void DCUWorker::UpdateSensorConnectionStatus(bool _sensorConnected) {
    SensorConnected = _sensorConnected;
}

void DCUWorker::onTimeout() {
    if (!_BusSerial->isOpen()){
        return;
    }
    //Check whether the sensor is open
//    if (!SensorConnected) {
//        //Test
//        targetVelocity = 0;
//        //return;
//    }
    count++;
//    QString test = "sr\n";
    if(errorCnt > _BusSerial->GetRstError()){
        _DevSerial->SetTxRxFlag(true);
    }
    if(endErrorCnt > 10) {
        _DevSerial->SetEndCommandFlag(true);
    }


    //On the Tx flag run the distance controller.
        if(_DevSerial->GetTxRxFlag()) {

            errorCnt = 0;


            //Check the sensor connection
            if(_DevSerial->GetEndCommandFlag()) {
                endErrorCnt = 0;
                commandQueue.append("sr\r\n");
                //_DevSerial->GetVelocityTimeCommand(&refVelocity, &targetTime);

                //commandQueue.clear();
                if (autoModeFlag) {
                    //Enable the gpo 1
                    commandQueue.append(QString("gov1=%1\r\n").arg(autoCommandValue));
                    autoModeFlag = false;
                }
                else {
                    commandQueue.append(QString("gov1\r\n"));
                }

                if (StateMachine::emergencyBellFlag && emergencyCnt ==0) {
                    commandQueue.append("si=1\r\n");
                    emergencyCnt = 1;
                    emergencyOffCnt = 0;
                }
                else if(StateMachine::emergencyBellFlag == false && emergencyOffCnt == 0){
                    commandQueue.append("si=0\r\n");
                    emergencyCnt = 0;
                    emergencyOffCnt = 1;
                }
                commandQueue.append("cvl\r\n");

                if(stopAutoDriveFlag) {
                    commandQueue.append("gvt=0,0\r\n;gov1=0\r\n");
                    stopAutoDriveFlag = false;
                }
                QString test1 = QString("gvt= %1, %2\r\n").arg(targetVelocity).arg(targetTime);
                commandQueue.append(test1);
                _DevSerial->SetEndCommandFlag(false);
            }
            else {
                endErrorCnt++;
            }

            if (!commandQueue.isEmpty()) {
                QString command = commandQueue.dequeue();
                QByteArray test = command.toUtf8();

                bool writeSuccess = _BusSerial->WriteByteArray(test);
                if(writeSuccess) {
                    _DevSerial->SetTxRxFlag(false);
                }

            }
        }
        else {
            errorCnt++;
        }




}
void DCUWorker::UpdateData(float _relativeDistance, float _errorDistance,
                           float _refAccel, float _refVelocity) {
    targetAcceleration = _refAccel;
    refVelocity = _refVelocity;
    targetVelocity = round(refVelocity);
    relativeDistance = _relativeDistance;
    errorDistance = _errorDistance;
    /*
     * Check for Record mode
     * If xlsx, start recording
     */
    if (!xlsx)
        return;
    static long int cellNum = 2;
    QString cellA = QString("A%1").arg(cellNum);
    QString cellB = QString("B%1").arg(cellNum);
    QString cellC = QString("C%1").arg(cellNum);
    QString cellD = QString("D%1").arg(cellNum);
    //QString cellE = QString("E%1").arg(cellNum);
    cellNum ++;

    QString relativeDistanceString = QString::number(relativeDistance);
    QString errorDistanceString = QString::number(errorDistance);
    QString refAccelString = QString::number(targetAcceleration);
    QString refVelString = QString::number(refVelocity);

    WriteThinBorderStyleCell(xlsx, cellA, relativeDistanceString);
    WriteThinBorderStyleCell(xlsx, cellB, errorDistanceString);
    WriteThinBorderStyleCell(xlsx, cellC, refAccelString);
    WriteThinBorderStyleCell(xlsx, cellD, refVelString);
    //WriteThinBorderStyleCell(xlsx, cellE,QString::number(relativeDistance));


    //velocityCmdFlag = true;
}
void DCUWorker::connectTriggered(QByteArray comPort) {
    if(_BusSerial->isOpen()){
            debugger->PrintErrorMsg("Disconnecting to DCU");
            _BusSerial->Close();
            //QByteArray lastCmd = "gvt=0,100\r\n";
//            _BusSerial->ClearRxBuff();
//            _BusSerial->ClearTxBuff();
//            _BusSerial->WriteByteArray(lastCmd);


            emit DisconnectDCU();
            return;
    }
    else{
        DCUConnected = _BusSerial->Open(comPort, QSerialPort::Baud115200);

        if (!DCUConnected){
            debugger->PrintErrorMsg("Could not connect to DCU");
            emit DisconnectDCU();
            return;//Failed
        }
        else{
            debugger->PrintErrorMsg("Connected to DCU");
            return;//Connected
        }
    }
}

void DCUWorker::StartAutoMode() {
    //Toggle the automode flag;
    autoModeFlag = true;
    }

void DCUWorker::SetDebugger(Debugger *_debugger) {
    debugger = _debugger;
}

DeviceSerial* DCUWorker::getDevSerialInstance() {
    return _DevSerial;
}

void DCUWorker::StopAutoDriving() {
    stopAutoDriveFlag = true;
}

void DCUWorker::StartRecording() {
    if (xlsx)
        return;
    xlsx = new QXlsx::Document();

    Format::BorderStyle bs = Format::BorderThin;
    QColor color(Qt::yellow);
    format.setBorderStyle(bs);
    format.setPatternBackgroundColor(color);
    xlsx->addSheet("Data Log");
    xlsx->setColumnWidth(1, 6, 20);
    xlsx->write("A1", "Relative Distance",format);
    xlsx->write("B1", "Error Distance", format);
    xlsx->write("C1", "Target Acceleration", format);
    xlsx->write("D1", "Target Velocity", format);
    xlsx->write("E1", "Currnent Speed", format);


}

void DCUWorker::StopRecording() {

    if(!xlsx)
        return;
    QDateTime local(QDateTime::currentDateTime());
    QString dateTime = local.toString("yy_MM_dd_");
    QString textTitle = dateTime + QString("DataLog%1.xlsx").arg(documentID);
    //qDebug() <<"Current date " << local.toString("MM_dd_yyyy");
    xlsx->saveAs(textTitle);
    delete xlsx;
    xlsx = nullptr;
    documentID++;
}

void DCUWorker::WriteThinBorderStyleCell(Document* xlsx, const QString &cell, const QString &text) {
    Format format;
    format.setBorderStyle(Format::BorderThin);


    xlsx->write(cell, text,format);
}
