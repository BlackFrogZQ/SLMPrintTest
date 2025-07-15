#pragma once
#include "scanPathDef.h"
#include "../printDatas.h"

namespace TIGER_PrintDatas
{
    struct scanLineDatas;
    struct layerDatas;
    class IFillType;
    class CScanPath : public CPrintDatas
    {
    public:
        CScanPath();
        ~CScanPath();
        virtual printSLCDatas getScanBlock() override;

    protected:
        void getSingleScanBlock(layerDatas& p_layer, const int p_layerId, const int p_contourId, const RegionType p_regionType);
        scanLineDatas addLineDatas(std::vector<pointDatas> p_points, contourType p_contourType);

    private:
        std::vector<bool> m_used;
        IFillType *m_pModelFillType;
    };
}