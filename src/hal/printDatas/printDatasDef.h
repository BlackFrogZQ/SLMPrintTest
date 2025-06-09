#pragma once
#include <QList>
#include <QSizeF>
#include <QBitArray>
#include <iostream>
using namespace std;

namespace TIGER_PrintDatas
{
    using CPicData = QBitArray;
    // CPicDatas[h][w]
    using CPicDatas = QList<QBitArray>;
    using CFrameData = QList<int>;

    struct Segment
    {
        int xStart;
        int xEnd;
        int y;
    };
}