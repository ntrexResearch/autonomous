#ifndef CANMANAGER_H
#define CANMANAGER_H
#include <QCanBusDevice> // CAN Bus Error
#include "Common/common.h"
#include <QObject>
#include <QThread>
#include <QTimer>
#include <QMutex>
// define all the command id and value
// abs enc value
// 200D status value
// sevcon speed value and etc
#define DCU_AUTO_REQUEST 1
#define DCU_MANUAL_REQUEST 2
#define DCU_OFF_REQUEST 0
#define SEVCON_INIT_REQUEST 10
#define SEVCON_DRIVE_CMD    11

#define STEER_DRIVE_CMD     20
#define STEER_ENABLE_REQUEST 21
#define STEER_DISABLE_REQUEST 22
#define STEER_CLEAR_FAULT   23

#define BRAKE_DRIVE_CMD     30
#define BRAKE_ENABLE_REQUEST 31
#define BRAKE_DISABLE_REQUEST 32
#define BRAKE_CLEAR_FAULT   33


class QCanBusFrame;

class CanManager : public QObject
{
    Q_OBJECT
public:
    static CanManager* Instance();
    ~CanManager();
    //void putCanManagerInAThread();
    void sendDcuCanMsg(int index);
    void sendCanMsg(quint32 id, int index, int data=0);
    void sendCanMsg2(quint32 id, int index, float data = 0);
    void setMutex(QMutex *mutex);

    void setSharedNumber(int* number);

    void start();

signals:
    void notifyWrittenFrames(qint64);
    void showReceivedFrames(QString);
    void showStateMessage(QString) const;
    void sendConnectionState(bool);

    void autoModeOn();
    void manualModeOn();

public slots:
    // Its can_device is connected to these slots.
    // When there are events in the can_device such as read or connect, these events are called.

    //In this function, received frames are checked if they belong to the PC Can message
    // If so, do the proper actions
    void processReceivedFrames();
    void sendFrame(const QCanBusFrame &frame) const;
    void processErrors(QCanBusDevice::CanBusError) const;
    void connectCanDevice(Settings);
    void disconnectCanDevice();
    void processWrittenFrames(qint64);
    void manageCan();

    void monitorOn();
    void monitorOff();

    void activateDCU();
    QTimer *getTimer();
    //void updateSettings(Settings);

protected:
    CanManager();
//    void closeEvent(QCloseEvent *event) override;

private:
    //void initActionsConnections();



    void initCanManager();
    int decodeFrame(const QCanBusFrame &frame);
    QCanBusDevice::Filter setCanFilter(const unsigned short &id);

    qint64 m_numberWrittenFrames = 0;

    Settings m_currentSettings;

    QCanBusDevice *m_canDevice = nullptr;
    static CanManager *_instance;

    QTimer *timer;
    //QCanBusFrame frame;
    QMutex *m_mutex;
    bool m_monitorFlag = false;
    bool m_connectionState = false;

    int my_number_test;
    int *shared_number_test = nullptr;
};

#endif // CANMANAGER_H
