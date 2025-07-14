#pragma once
#include "../../iFillType.h"

namespace TIGER_PrintDatas
{
    class CLineFill : public IFillType
    {
    public:
        using IFillType::IFillType;
        virtual std::vector<scanLineDatas> generateRegionFill(const std::vector<pointDatas>& p_outer, const std::vector<std::vector<pointDatas>>& p_holes, float p_lineWidth, float p_angleDegree, ScanDirection p_dirMode) override;
    };
}