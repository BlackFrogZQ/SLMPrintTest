#pragma once
#include <QList>
#include <QSizeF>
#include <QBitArray>
#include <iostream>
using namespace std;

namespace TIGER_PrintDatas
{
    using CPicData = QBitArray;
    using CPicDatas = QList<QBitArray>;
    using CFrameData = QList<int>;

    // 每一段线的起点和终点
    struct lineSegment
    {
        float xStart;
        float xEnd;
        float y; // 所在行的y坐标
    };

    struct CLaserPara
    {
        float laserLineWidth = 0.0765; // 激光线宽
        int widthDpi = 300;       // 宽度DPI
        CLaserPara(float p_laserLineWidth = 0.0765, int p_widthDpi = 300)
            : laserLineWidth(p_laserLineWidth), widthDpi(p_widthDpi)
        {
            assert(laserLineWidth >= 0);
            assert(widthDpi > 0);
        }
    };
}