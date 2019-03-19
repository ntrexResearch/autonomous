#ifndef CANMANAGER_H
#define CANMANAGER_H
#include <QCanBusDevice> // CAN Bus Error
#include "common.h"
#include <QObject>
#include <QThread>
#include <QTimer>

// define all the command id and value
// abs enc value
// 200D status value
// sevcon speed value and etc
#define DCU_AUTO_REQUEST 1
#define DCU_MANUAL_REQUEST 2
#define SEVCON_INIT_REQUEST 10
#define SEVCON_DRIVE_CMD    11


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

    bool m_monitorFlag = false;
    bool m_connectionState = false;
};

#endif // CANMANAGER_H
