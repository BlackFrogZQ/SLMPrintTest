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
        int widthDpi = 300;            // 宽度DPI
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
    struct pointDatas
    {
        float x, y;
    };

    // 轮廓数据
    struct countourDatas
    {
        vector<pointDatas> points;
        bool isClosed;
        bool isOuterContour;
        bool isModelContour;
    };

    // 单层数据
    struct layerDatas
    {
        float z;
        vector<countourDatas> pContours;
    };

    // SLC打印数据
    struct printSLCDatas
    {
        float initialHeight;
        float layerThickness;
        float lineWidth;
        float reservedSize;
        bool isModelSlice;
        vector<layerDatas> allSLCLayers;
    };

    template<typename T>
    T read(string& fs, size_t& index)
    {
        T value;
        memcpy(&value, fs.data() + index, sizeof(T));
        index += sizeof(T);
        return value;
    }
#pragma endregion
}