#ifndef COMMON_H
#define COMMON_H
#include <QString>
#include <QList>

#include <QCanBusDevice>
#include <QCanBusDeviceInfo>



typedef QPair<QCanBusDevice::ConfigurationKey, QVariant> ConfigurationItem;
struct Settings {
    QString deviceInterfaceName;
    QList<ConfigurationItem> configurations;
    bool useConfigurationEnabled = false;
};

Q_DECLARE_METATYPE(Settings)

#define DEBUG_MODE true


// DCU CAN Bus
#define DCU_CAN_ID      0x0010
#define ABS_ENC_CAN_ID  0x0020
#define MW200D_CAN_ID   0x0001
#define SEVCON_CAN_ID   0x0300
#define DCU_CAN_DLC     0x08
#define ABS_ENC_CAN_DLC 0x08
#define MW200D_CAN_DLC  0x08
#define SEVCON_CAN_DLC  0x08

#define PC_CAN_ID       0x0040
#define PC_CAN_DLC      0x08

// Sevcon Can Bus

// Abs. Encoder Can Bus

// 200W Motor Driver Can Bus

// PC Can Bus

int limit_int(int value, int low, int high);

#endif // COMMON_H
