#include "ncDef.h"

namespace TIGER_VMSLM
{
    CMarkConnectParas *MarkConnectPara()
    {
        static CMarkConnectParas g_MarkConnectPara;
        return &g_MarkConnectPara;
    }

    CGalvoMotorParas *motorParas()
    {
        static CGalvoMotorParas g_motorParas;
        return &g_motorParas;
    }

    CLaserParas *laserParas()
    {
        static CLaserParas g_laserParas;
        return &g_laserParas;
    }

    CMarkDatas *getMarkDatas()
    {
        static CMarkDatas g_markDatas;
        return &g_markDatas;
    }

    CPLCParas *plcParas()
    {
        static CPLCParas g_plcParas;
        return &g_plcParas;
    };
}