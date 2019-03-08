#ifndef CANMANAGER_H
#define CANMANAGER_H
#include <QCanBusDevice> // CAN Bus Error
#include "common.h"
#include <QObject>
#include <QThread>
#include <QTimer>

class CanConnectDialog;
class QCanBusFrame;


class CanManager : public QObject
{
    Q_OBJECT
public:
    static CanManager* Instance();
    ~CanManager();
    //void putCanManagerInAThread();

signals:
    void notifyWrittenFrames(qint64);
    void showReceivedFrames(QString);
    void showStateMessage(QString) const;
    void sendConnectionState(bool);

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
    QCanBusFrame frame;

    bool m_monitorFlag = false;
    bool m_connectionState = false;
};

#endif // CANMANAGER_H
