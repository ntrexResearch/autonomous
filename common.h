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

// DCU CAN Bus
#define DCU_T      0x150
#define E46_VEHICLE_SPEED   0x000 // Unknown
#define E46_FUEL_LEVEL      0x613
#define E46_COOLANT_TEMP    0x329
#define E46_OIL_TEMP        0x545
// Sevcon Can Bus

// Abs. Encoder Can Bus

// 200W Motor Driver Can Bus

// PC Can Bus


#endif // COMMON_H
