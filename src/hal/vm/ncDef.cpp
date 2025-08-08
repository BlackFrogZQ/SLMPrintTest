#include "ncDef.h"

namespace TIGER_VMSLM
{
    CMarkConnectParas *MarkConnectPara()
    {
        static CMarkConnectParas g_MarkConnectPara;
        return &g_MarkConnectPara;
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