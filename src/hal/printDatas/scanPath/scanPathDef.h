#pragma once
#include <vector>

namespace TIGER_VMSLM
{
    enum CMarkType : int;
}
namespace TIGER_PrintDatas
{
#pragma region "扫描路径类型"
    enum ModelFillType
    {
        mftLineFill = 0,
        mftContourFill,
        mftBlockFill,
        mftMax
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
        TIGER_VMSLM::CMarkType pMarkType;
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