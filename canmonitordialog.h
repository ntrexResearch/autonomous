#ifndef CANMONITORDIALOG_H
#define CANMONITORDIALOG_H
#include <QCanBusDevice> // CAN Bus Error
#include <QLabel>
#include <QDialog>
#include <QStatusBar>
#include "sendframebox.h"

namespace Ui {
class CanMonitorDialog;
}

class CanMonitorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CanMonitorDialog(QWidget *parent = nullptr);
    ~CanMonitorDialog();
    SendFrameBox *getSendFrameBox();

signals:
    void closed();

public slots:
    void sendFrame(const QCanBusFrame &frame) const;
    void showCanState(QString) const;
    void processWrittenFrames(qint64);
    void showReceivedFrames(QString);
    void updateConnectionState(bool);

private slots:
    void on_clearLogButton_released();

protected:
    void closeEvent(QCloseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::CanMonitorDialog *m_ui;
    QLabel *m_status = nullptr;
    QLabel *m_written = nullptr;
    QStatusBar *m_statusBar = nullptr;
    QCanBusDevice *m_canDevice = nullptr;

    void initGui();

};

#endif // CANMONITORDIALOG_H
