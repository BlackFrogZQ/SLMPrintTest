#pragma once
#include "system/basic.h"
#include "hal/printDatas/printDatasDef.h"

using namespace TIGER_PrintDatas;
namespace TIGER_VMSLM
{
#pragma region "振镜卡通信参数"
    enum CMarkProtocol: int
    {
        cmpSPI = 0,
        cmpXY2_100,
        cmpSL2,
        cmpUnKnow
    };
    struct CMarkConnectParas
    {
        QString ip = "172.18.34.227";
        CMarkProtocol MarkProtocol = cmpXY2_100;  // 打标协议
    };
    CMarkConnectParas *MarkConnectPara();
#pragma endregion

#pragma region "振镜电机参数"
    struct CGalvoMotorParas
    {
        unsigned int MarkSpeed;       // 打标速度(mm/s)
        unsigned int JumpSpeed;       // 跳转速度(mm/s)
        unsigned int MarkDelay;       // 打标延时(us)
        unsigned int JumpDelay;       // 跳转延时(us)
        unsigned int PolygonDelay;    // 转弯延时(us)
        unsigned int MarkCount;       // 打标次数
        CGalvoMotorParas::CGalvoMotorParas()
        {
            MarkSpeed = 1000;
            JumpSpeed = 2000;
            MarkDelay = 0;
            JumpDelay = 0;
            PolygonDelay = 0;
            MarkCount = 1;
        }
    };
    CGalvoMotorParas *motorParas();
#pragma endregion

#pragma region "激光参数"
    enum CLaserDevice: int
    {
        cltIPG = 0,
        cltSPI,
        cltCO2,
        cltUnKnow
    };
    struct CLaserParas
    {
        CLaserDevice LaserDevice = cltIPG;       // 激光器类型
        double LaserOnDelay = 111;               // 开激光延时(单位us)
        double LaserOffDelay = 111;              // 关激光延时(单位us)
        double FPKDelay;                         // 首脉冲抑制延时(单位us)
        double FPKLength;                        // 首脉冲抑制长度(单位us)
        double QDelay;                           // 出光Q频率延时(单位us)
        double DutyCycle = 0.5;                  // 出光时占空比，(0~1)
        double Frequency = 20;                   // 出光时频率kHz
        double StandbyFrequency = 20;            // 不出光Q频率(单位kHz);
        double StandbyDutyCycle = 0.5;           // 不出光Q占空比(0~1);
        double LaserPower = 50;                  // 激光能量百分比(0~100)，50代表50%
        unsigned int AnalogMode = 0;             // 1代表使用模拟量输出来控制激光器能量（0~10V）
        unsigned int Waveform = 1;               // SPI激光器波形号（0~63）
        unsigned int PulseWidthMode;             // 0,不开启MOPA脉宽使能模式， 1,开启MOPA激光器脉宽使能
        unsigned int PulseWidth;                 // MOPA激光器脉宽值 单位（us）
        CLaserParas::CLaserParas()
        {}
    };
    CLaserParas *laserParas();
#pragma endregion

#pragma region "打标参数"
    enum CMarkType : int
    {
        cmtOuterContour = 0,
        cmtInnerContour,
        cmtSupportContour,
        cmtScanFill,
        cmtMarkTest,
        cmtMax
    };
    const QStringList cmarkTypeStr = {cnStr("外轮廓"), cnStr("内轮廓"), cnStr("支撑"), cnStr("扫描填充")};
    const QStringList cmarkTypeEnStr = {"OuterContour", "InnerContour",  "SupportContour",  "ScanFill"};
    struct CMarkParas
    {
        CMarkType markType;
        CGalvoMotorParas motorParas;
        CLaserParas laserParas;
    };
    struct CMarkDatas
    {
        CMarkParas markParas[cmtMax];
        layerDatas printDatas;
    };
    CMarkDatas *getMarkDatas();
#pragma endregion

#pragma region "PLC参数"
    struct CPLCParas
    {
        QString ip = "192.168.0.32";
        int ipPort = 502;
        int ipMAC = 64;
    };
    CPLCParas *plcParas();
#pragma endregion

#pragma region "状态"
    enum CVMStatus : int
    {
        vmsIdle = 0,          // 空闲
        vmsManu,              // 手动
        vmsPause,             // 暂停
        vmsSpread,            // 铺粉
        vmsScan,              // 扫描
        vmsMark,              // 打标
        vmsAutoFlush,         // 自动清洗
        vmsManuOnce,          // 手动一次
        vmsStatusBar,         // 状态栏
        vmsZero,              // 归零
        vmsMax                // 最大状态值
    };
    const QStringList cvmStatusCn = {
        cnStr("空闲"),
        cnStr("手动"),
        cnStr("暂停"),
        cnStr("铺粉"),
        cnStr("打标"),
        cnStr("打标"),
        cnStr("自动清洗"),
        cnStr("手动一次"),
        cnStr("状态栏"),
        cnStr("归零")
    };
#pragma endregion
}