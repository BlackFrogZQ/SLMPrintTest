#pragma once
#include "iFillTypeDef.h"
#include "hal/vm/ncDef.h"
#include "../../printDatasDef.h"
#include "../scanPathDef.h"

namespace TIGER_PrintDatas
{
    class IFillType
    {
    public:
        IFillType();
        virtual ~IFillType();
        virtual std::vector<scanLineDatas> generateRegionFill(const std::vector<pointDatas>& p_outer, const std::vector<std::vector<pointDatas>>& p_holes, float p_lineWidth, float p_angleDegree, ScanDirection p_dirMode) = 0;
    };
}