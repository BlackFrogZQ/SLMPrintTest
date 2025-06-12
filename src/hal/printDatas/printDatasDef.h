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
        int xStart;
        int xEnd;
        int y; // 所在行的y坐标
    };

    struct CLaserPara
    {
        int laserLineWidth; // 激光线宽
        int widthDpi;       // 宽度DPI
        CLaserPara(int p_laserLineWidth = 0.0765, int p_widthDpi = 300)
            : laserLineWidth(p_laserLineWidth), widthDpi(p_widthDpi)
        {
            assert(laserLineWidth >= 0);
            assert(widthDpi > 0);
        }
    };
}