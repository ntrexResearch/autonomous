#include "canconnectdialog.h"
#include "ui_canconnectdialog.h"

#include <QCanBus>
#include <QDebug>
#include <QMetaType>
#include <QMenu>

CanConnectDialog::CanConnectDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::CanConnectDialog)
{

    m_ui->setupUi(this);

    this->setWindowTitle("CAN Connect Dialog");

    m_ui->errorFilterEdit->setValidator(new QIntValidator(0, 0x1FFFFFFFU, this));

    QMenu *file = new QMenu(this);
    file->addMenu(tr("&File"));

    QAction *okAction = new QAction(this);//file->addAction(tr("Ok"));
    okAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Space));
    QAction *closeAction = new QAction(this);//file->addAction(tr("Close"));
    closeAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Q));

    addAction(okAction);
    addAction(closeAction);

    connect(okAction, SIGNAL(triggered()), this, SLOT(ok()));
    connect(closeAction, SIGNAL(triggered()), this, SLOT(close()));

    m_ui->loopbackCombo->addItem(tr("unspecified"), QVariant());
    m_ui->loopbackCombo->addItem(tr("false"), QVariant(false));
    m_ui->loopbackCombo->addItem(tr("true"), QVariant(true));

    m_ui->receiveOwnCombo->addItem(tr("unspecified"), QVariant());
    m_ui->receiveOwnCombo->addItem(tr("false"), QVariant(false));
    m_ui->receiveOwnCombo->addItem(tr("true"), QVariant(true));

    m_ui->canFdCombo->addItem(tr("false"), QVariant(false));
    m_ui->canFdCombo->addItem(tr("true"), QVariant(true));

    m_ui->dataBitrateCombo->setFlexibleDataRateEnabled(true);
    connect(this, &QDialog::accepted, this, &CanConnectDialog::acceptDialog);
    connect(m_ui->okPushButton, &QPushButton::released, this, &CanConnectDialog::ok);
    connect(m_ui->cancelPushButton, &QPushButton::released, this, &CanConnectDialog::cancel);
    connect(m_ui->interfaceCombo, &QComboBox::currentTextChanged,
            this, &CanConnectDialog::interfaceChanged);
    connect(m_ui->interfaceRefreshButton, &QPushButton::clicked, this, &CanConnectDialog::updateInterfaceList);
    //connect(m_ui->)
    updateSettings();
    qRegisterMetaType<Settings>();

    //Debug flag set
    debug_flag = false;
    m_ui->interfaceCombo->clear();
    m_interfaces = QCanBus::instance()->availableDevices("peakcan");
    for (const QCanBusDeviceInfo &info : qAsConst(m_interfaces))
        m_ui->interfaceCombo->addItem(info.name());
}

CanConnectDialog::~CanConnectDialog()
{
    delete m_ui;
}

Settings CanConnectDialog::settings() const
{
    return m_currentSettings;
}

void CanConnectDialog::interfaceChanged(const QString &interface)
{
    for (const QCanBusDeviceInfo &info : qAsConst(m_interfaces)) {
        if (info.name() == interface) {
            if (debug_flag)
                qDebug()<<info.description();
        }
    }
}

void CanConnectDialog::updateInterfaceList()
{
    m_ui->interfaceCombo->clear();
    m_interfaces = QCanBus::instance()->availableDevices("peakcan");
    for (const QCanBusDeviceInfo &info : qAsConst(m_interfaces))
        m_ui->interfaceCombo->addItem(info.name());
}

void CanConnectDialog::keyPressEvent(QKeyEvent *event){
    if (event->key() == Qt::Key_Enter) {
        this->ok();
        event->accept();
    }else {
        QDialog::keyPressEvent(event);
    }
}

void CanConnectDialog::ok()
{
    updateSettings();
    accept();
}

void CanConnectDialog::cancel()
{
    revertSettings();
    reject();
}

QString CanConnectDialog::configurationValue(QCanBusDevice::ConfigurationKey key)
{
    QVariant result;

    for (const ConfigurationItem &item : qAsConst(m_currentSettings.configurations)) {
        if (item.first == key) {
            result = item.second;
            break;
        }
    }

    if (result.isNull() && (
                key == QCanBusDevice::LoopbackKey ||
                key == QCanBusDevice::ReceiveOwnKey)) {
        return tr("unspecified");
    }

    return result.toString();
}

void CanConnectDialog::revertSettings()
{
    m_ui->interfaceCombo->setCurrentText(m_currentSettings.deviceInterfaceName);

    QString value = configurationValue(QCanBusDevice::LoopbackKey);
    m_ui->loopbackCombo->setCurrentText(value);

    value = configurationValue(QCanBusDevice::ReceiveOwnKey);
    m_ui->receiveOwnCombo->setCurrentText(value);

    value = configurationValue(QCanBusDevice::ErrorFilterKey);
    m_ui->errorFilterEdit->setText(value);

    value = configurationValue(QCanBusDevice::BitRateKey);
    m_ui->bitrateCombo->setCurrentText(value);

    value = configurationValue(QCanBusDevice::CanFdKey);
    m_ui->canFdCombo->setCurrentText(value);

    value = configurationValue(QCanBusDevice::DataBitRateKey);
    m_ui->dataBitrateCombo->setCurrentText(value);
}

void CanConnectDialog::acceptDialog()
{
    emit connectCanDevice(m_currentSettings);
}

void CanConnectDialog::updateSettings()
{
    m_currentSettings.deviceInterfaceName = m_ui->interfaceCombo->currentText();
    // Always use the configuration provided from the user
    m_currentSettings.configurations.clear();
    if (m_ui->loopbackCombo->currentIndex() != 0){
        ConfigurationItem item;
        item.first = QCanBusDevice::LoopbackKey;
        item.second = m_ui->loopbackCombo->currentData();
        m_currentSettings.configurations.append(item);
    }

    if (m_ui->receiveOwnCombo->currentIndex() != 0) {
        ConfigurationItem item;
        item.first = QCanBusDevice::ReceiveOwnKey;
        item.second = m_ui->receiveOwnCombo->currentData();
        m_currentSettings.configurations.append(item);
    }

    if (!m_ui->errorFilterEdit->text().isEmpty()) {
        QString value = m_ui->errorFilterEdit->text();
        bool ok = false;
        int dec = value.toInt(&ok);
        if (ok) {
            ConfigurationItem item;
            item.first = QCanBusDevice::ErrorFilterKey;
            item.second = QVariant::fromValue(QCanBusFrame::FrameErrors(dec));
            m_currentSettings.configurations.append(item);
        }
    }
    const int bitrate = m_ui->bitrateCombo->bitRate();
    if (bitrate > 0) {
        const ConfigurationItem item(QCanBusDevice::BitRateKey, QVariant(bitrate));
        m_currentSettings.configurations.append(item);
    }

    // process CAN FD setting
//    ConfigurationItem fdItem;
//    fdItem.first = QCanBusDevice::CanFdKey;
//    fdItem.second = m_ui->canFdCombo->currentData();
//    m_currentSettings.configurations.append(fdItem);

//    // process data bitrate
//    const int dataBitrate = m_ui->dataBitrateCombo->bitRate();
//    if (dataBitrate > 0) {
//        const ConfigurationItem item(QCanBusDevice::DataBitRateKey, QVariant(dataBitrate));
//        m_currentSettings.configurations.append(item);
//    }

}
