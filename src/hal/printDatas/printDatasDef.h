﻿#pragma once
#include "scanPath/scanPathDef.h"
#include <QList>
#include <QSizeF>
#include <QBitArray>
#include <iostream>

namespace TIGER_PrintDatas
{
#pragma region "激光参数"
    struct CLaserPara
    {
        float laserWidth = 0.0765f;
        int widthDpi = 300;
        CLaserPara(float p_laserLineWidth = 0.0765, int p_widthDpi = 300)
            : laserWidth(p_laserLineWidth), widthDpi(p_widthDpi)
        {
            assert(laserWidth >= 0);
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
        bool operator==(pointDatas &p_coutour);
    };

    // 轮廓数据
    struct countourDatas
    {
        std::vector<pointDatas> points;
        bool isClockwise(const std::vector<pointDatas>& contour);
        bool isClosed;
        bool isOuterContour;
        bool isModelContour;
    };

    // 单层轮廓数据
    struct layerDatas
    {
        float z;
        bool existModel;
        bool existSupport;
        std::vector<countourDatas> pContours;
        std::vector<scanBlockDatas> pScanBlocks;
    };

    // SLC打印数据
    struct printSLCDatas
    {
        float initialHeight;
        float layerThickness;
        float lineWidth;
        float reservedSize;
        std::vector<layerDatas> pLayerDatas;
    };

    template<typename T>
    T read(std::string& fs, std::size_t& index)
    {
        T value;
        memcpy(&value, fs.data() + index, sizeof(T));
        index += sizeof(T);
        return value;
    }
#pragma endregion
}