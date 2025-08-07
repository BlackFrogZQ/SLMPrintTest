#include "scanSystemParaItem.h"
#include "hal/vm/ncDef.h"
#include "../paraNode.h"

using namespace TIGER_ParaDef;
using namespace TIGER_VMSLM;

namespace TIGER_ParaItemDef
{
    class CDZSTMarkPara : public IParaItem
    {
    public:
        CDZSTMarkPara() : IParaItem(cnStr("DZSTMarkConnectPara"), cnStr("振镜卡通信参数"), true)
        {
            CParaNode *node = nullptr;
            _AddNode(m_currentNode, pntString, "ip", "ip", true, "", &MarkConnectPara()->ip);
            _AddNode(m_currentNode, pntEnum, "MarkProtocol", "打标协议", true, "SPI*XY2-100*SL2", &MarkConnectPara()->MarkProtocol);

            for(size_t i = 0; i < cmtMarkTest; i++)
            {
                auto &markPara = getMarkDatas()->markParas[i];
                _AddRootNode2(m_currentNode, cmarkTypeEnStr[i] + "Paras", cmarkTypeStr[i] + cnStr("参数"), true, "");
                CParaNode *pMotorRootNode = node;
                _AddRootNode2(pMotorRootNode, "GalvoMotorParas", cnStr("振镜电机参数"), true, "");
                _AddNode(node, pntUInt, "MarkSpeed", "打标速度", true, "mm/s", &markPara.motorParas.MarkSpeed);
                _AddNode(node, pntUInt, "JumpSpeed", "跳转速度", true, "mm/s", &markPara.motorParas.JumpSpeed);
                _AddNode(node, pntUInt, "MarkDelay", "打标延时", true, "us", &markPara.motorParas.MarkDelay);
                _AddNode(node, pntUInt, "JumpDelay", "跳转延时", true, "us", &markPara.motorParas.JumpDelay);
                _AddNode(node, pntUInt, "PolygonDelay", "转弯延时", true, "us", &markPara.motorParas.PolygonDelay);
                _AddNode(node, pntUInt, "MarkCount", "打标次数", true, "", &markPara.motorParas.MarkCount);

                _AddRootNode2(pMotorRootNode, "LaserParas", cnStr("激光参数"), true, "");
                _AddNode(node, pntEnum, "LaserDevice", "激光器类型", true, "IPG*SPI*CO2/紫外", &markPara.laserParas.LaserDevice);
                _AddNode(node, pntDouble, "LaserOnDelay", "开激光延时", true, "us", &markPara.laserParas.LaserOnDelay);
                _AddNode(node, pntDouble, "LaserOffDelay", "关激光延时", true, "us", &markPara.laserParas.LaserOffDelay);
                _AddNode(node, pntDouble, "FPKDelay", "首脉冲抑制延时", true, "us", &markPara.laserParas.FPKDelay);
                _AddNode(node, pntDouble, "FPKLength", "首脉冲抑制长度", true, "us", &markPara.laserParas.FPKLength);
                _AddNode(node, pntDouble, "QDelay", "出光Q频率延时", true, "us", &markPara.laserParas.QDelay);
                _AddNode(node, pntDouble, "DutyCycle", "出光时占空比", true, "0~1", &markPara.laserParas.DutyCycle);
                _AddNode(node, pntDouble, "Frequency", "出光时频率", true, "kHz", &markPara.laserParas.Frequency);
                _AddNode(node, pntDouble, "StandbyFrequency", "不出光Q频率", true, "kHz", &markPara.laserParas.StandbyFrequency);
                _AddNode(node, pntDouble, "StandbyDutyCycle", "不出光Q占空比", true, "0~1", &markPara.laserParas.StandbyDutyCycle);
                _AddNode(node, pntDouble, "LaserPower", "激光能量百分比", true, "(0~100)%, 50代表50%", &markPara.laserParas.LaserPower);
                _AddNode(node, pntUInt, "AnalogMode", "激光器能量", true, "0~10V", &markPara.laserParas.AnalogMode);
                _AddNode(node, pntUInt, "Waveform", "SPI激光器波形号", true, "0~63", &markPara.laserParas.Waveform);
                _AddNode(node, pntUInt, "PulseWidthMode", "脉宽使能", true, "0, 不开启MOPA脉宽使能模式, 1, 开启MOPA激光器脉宽使能", &markPara.laserParas.PulseWidthMode);
                _AddNode(node, pntUInt, "PulseWidth", "MOPA激光器脉宽值", true, "us", &markPara.laserParas.PulseWidth);
            }
        };
    };
    IParaItem *DZSTMarkParaItem()
    {
        static CDZSTMarkPara g_DZSTMarkConnectParasNode;
        return &g_DZSTMarkConnectParasNode;
    }

    class CPLCPara : public IParaItem
    {
    public:
        CPLCPara() : IParaItem(cnStr("PLCConnectPara"), cnStr("PLC通信参数"), true)
        {
            CParaNode *node = nullptr;
            _AddNode(m_currentNode, pntString, "ip", "ip", true, "", &plcParas()->ip);
            _AddNode(m_currentNode, pntUInt, "port", "通道", true, "", &plcParas()->ipPort);
            _AddNode(m_currentNode, pntUInt, "mac", "端口", true, "", &plcParas()->ipMAC);
        };
    };
    IParaItem *PLCParaItem()
    {
        static CPLCPara g_PLCConnectParasNode;
        return &g_PLCConnectParasNode;
    }
}