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

#pragma region "激光参数"
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
#pragma endregion

#pragma region "按图片数据获得打印参数"
    // 每一段线的起点和终点
    struct lineSegment
    {
        float xStart;
        float xEnd;
        float y; // 所在行的y坐标
    };
#pragma endregion

#pragma region "SLC打印参数"
    // 点坐标
    struct Point
    {
        float x, y;
    };

    // 单层数据
    struct Layer
    {
        float z;
        float thick;
        float lineWidth;
        vector<vector<Point>> pContours;
    };

    template<typename T>
    T read(std::string& fs, size_t& index)
    {
        T value;
        std::memcpy(&value, fs.data() + index, sizeof(T));
        index += sizeof(T);
        return value;
    }
#pragma endregion
}