#include "DZSTMarkDef.h"

namespace TIGER_DZSTMarkDef
{
    CGalvoScanningSystemParas *scanSystemParas()
    {
        static CGalvoScanningSystemParas g_scanSystemParas;
        return &g_scanSystemParas;
    }

    CLaserParas *laserParas()
    {
        static CLaserParas g_laserParas;
        return &g_laserParas;
    }

    CMarkShapeParas *markShapeParas()
    {
        static CMarkShapeParas g_markShapeParas;
        return &g_markShapeParas;
    }
}