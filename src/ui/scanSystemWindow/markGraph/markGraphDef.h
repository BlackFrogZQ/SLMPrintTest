#pragma once
#include "system/basic.h"

namespace TIGER_MarkGraph
{
#pragma region "打标形状参数"
    enum CMarkShape
    {
        cmsCircle,
        cmsGrid,
        cmsSpiralFill,
        cmsMax
    };
    const QStringList cmarkSape = {cnStr("圆形"), cnStr("网格"), cnStr("螺旋填充")};
    struct CMarkShapeParas
    {
        CMarkShape shapeType = cmsCircle;
        unsigned int markRange = 100;
        unsigned int rowAndCol = 10;
        unsigned int diameter = 2;
    };
#pragma endregion
}