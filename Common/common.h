#ifndef COMMON_H
#define COMMON_H
#include <string.h>
#include <stdlib.h>
#include <algorithm>

#ifndef MAX
#define MAX(x,y) (((x) > (y)) ? (x) : (y))
#endif

#define L_LO		-2147483648
#define L_HI		 2147483647

#ifndef M_PI
#define M_PI		3.1415926535897932384626433832795028841971693993751f		// pi
#endif

#define M_2PI		(2*M_PI)	// 2pi

#define RAD2DEG		(180/M_PI)
#define DEG2RAD		(M_PI/180)

#define RPM2RPS		(M_2PI/60)		// rpm(분당 회전수)를 rad/s 속도로 변환하기 위한 상수
#define RPS2RPM		(60/M_2PI)		// rad/s 속도를 rpm(분당 회전수)로 변환하기 위한 상수

                                    // 	NaN								NaN								NaN							Inf								-Inf
#define IsNaN(x)	((*(int32_t *)&x == 0x7FFFFFFF) || (*(int32_t *)&x == 0xFFFFFFFF) || (*(int32_t *)&x == 0x7FC00000) || (*(int32_t *)&x == 0x7F800000) || (*(int32_t *)&x == 0xFF800000))



#define DEBUG_MODE true


// DCU CAN Bus
#define DCU_CAN_ID      0x0010
#define ABS_ENC_CAN_ID  0x0002
#define MW200D_CAN_ID   0x0001
#define SEVCON_CAN_ID   0x0300
#define DCU_CAN_DLC     0x08
#define ABS_ENC_CAN_DLC 0x08
#define MW200D_CAN_DLC  0x08
#define SEVCON_CAN_DLC  0x08

#define PC_CAN_ID       0x0040
#define PC_CAN_DLC      0x08

#define FRONT_LIDAR_CAN_RX_ID   0x740

#define FRONT_LIDAR_CAN_TX_ID   0x750


#define MAX_STEERING_SPEED 72000
#define MAX_STEERING_VOLTAGE 30.0
#define MIN_STEERING_VOLTAGE 5.0

// Modify this constant
#define STEER_SCALE_CONSTANT 1000
// Sevcon Can Bus

// Abs. Encoder Can Bus

// 200W Motor Driver Can Bus

// PC Can Bus

#ifndef _MSC_VER
typedef unsigned int UINT;
#endif



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


int limit_int(int value, int low, int high);


//For IIR filter
const int MaxOrder = 5;
const int LEDDAR_MAX_DETECTIONS = 16;
const UINT NBR_REGISTERS = 32+2;


//const int maxDistance = 100;

//const float maxVelocity = 1000;
const float maxVelocity = 1700;
//*************************************************************************
//TYPES
//*************************************************************************


struct SDetection
{
    UINT channel;
    float dDistance;
    float dAmplitude;
};

//*************************************************************************
//IMPLEMENTATION
//*************************************************************************
namespace
{
    void prependCharArray(char* s, const char* t){
        size_t len = strlen(t);
        size_t i;

        memmove(s+len, s, strlen(s)+1);

        for(i = 0; i < len; i++) {
            s[i] = t[i];
        }
    }

    bool Limit_int(int* out, int max, int min) {
        if (*out > max){
            *out = max;
            return true;
        }
        else if (*out < min){
            *out = min;
            return true;
        }
        else{
            return false;
        }
    }


    bool Limit_float(float* out, float max, float min) {
        if (*out > max){
            *out = max;
            return true;
        }
        else if (*out < min){
            *out = min;
            return true;
        }
        else {
            return false;
        }
    }

}

#endif // COMMON_H
