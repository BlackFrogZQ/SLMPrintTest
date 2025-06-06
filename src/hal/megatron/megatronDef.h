#pragma once
#include "system/basic.h"

namespace TIGER_Megatron
{
#define PI 3.1415926
#define layerCount 1
#define Mark2D 0
typedef unsigned char byte2;
typedef int32_t GMC_STATUS;
constexpr int cReconnectInterval = 1000;   // 硬件掉线后重连时间，单位ms
constexpr int cLaserWidth = 1;             // 激光宽度，单位mm
constexpr int cImagePixWidth = 1;          // 图像像素宽度，单位mm

    struct CMegatronIP
    {
        QString name;
        QString ip;
        int ipPort;
        CMegatronIP(QString p_name = "", QString p_ip = "", int p_ipPort = 502)
            : name(p_name),
              ip(p_ip),
              ipPort(p_ipPort) {}
    };
}