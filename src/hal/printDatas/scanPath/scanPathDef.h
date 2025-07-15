#pragma once
#include <vector>

namespace TIGER_PrintDatas
{
#pragma region "扫描路径类型"
    enum ModelFillType
    {
        mftLineFill = 0,
        mftContourFill,
        mftBlockFill,
        mftNone
    };

    enum RegionType
    {
        rtModelContour = 0,
        rtSupportContour,
        rtUnknown
    };

    enum contourType
    {
        ctOuter = 0,
        ctInner,
        ctScan,
        ctUnknown
    };

    struct printPoint
    {
        float x;
        float y;
        float z;
        float a;
    };

    struct scanLineDatas
    {
        contourType pContourType;
        std::vector<printPoint> points;
    };

    struct scanBlockDatas
    {
        RegionType pRegionType;
        std::vector<scanLineDatas> scanLines;
    };
#pragma endregion
}