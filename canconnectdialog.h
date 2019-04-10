#ifndef CANCONNECTDIALOG_H
#define CANCONNECTDIALOG_H

#include <QCanBusDevice>
#include <QCanBusDeviceInfo>
#include "Common/common.h"
#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class CanConnectDialog;
}
QT_END_NAMESPACE

class CanConnectDialog : public QDialog
{
    Q_OBJECT

public:



    explicit CanConnectDialog(QWidget *parent = nullptr);
    ~CanConnectDialog();

    Settings settings() const;

private slots:
    void interfaceChanged(const QString &interface);
    void ok();
    void cancel();
    void updateInterfaceList();

    void acceptDialog();

signals:
    void connectCanDevice(Settings);


protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    QString configurationValue(QCanBusDevice::ConfigurationKey key);
    void revertSettings();
    void updateSettings();

    Ui::CanConnectDialog *m_ui = nullptr;
    Settings m_currentSettings;
    QList<QCanBusDeviceInfo> m_interfaces;
    bool debug_flag;

};

#endif // CANCONNECTDIALOG_H
