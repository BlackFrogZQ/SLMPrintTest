#pragma once
#include "system/basic.h"
#include <vector>

namespace TIGER_MarkGraph
{
    constexpr double M_PI = 3.14159265358979323846;
    constexpr float laserWidth = 0.0765f;
#pragma region "打标形状参数"
    enum CMarkShape
    {
        cmsCircle,
        cmsGrid,
        cmsSpiralFill,
        cmsMax
    };
    const QStringList cmarkShape = {cnStr("圆形"), cnStr("网格"), cnStr("螺旋填充")};
    struct CMarkShapeParas
    {
        CMarkShape shapeType = cmsCircle;
        unsigned int markRange = 100;
        unsigned int rowAndCol = 10;
        unsigned int diameter = 2;
        bool showPathIndicate = true;
    };
#pragma endregion
}