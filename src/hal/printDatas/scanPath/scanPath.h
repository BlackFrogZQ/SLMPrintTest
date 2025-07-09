#pragma once
#include "scanPathDef.h"
#include "../printDatas.h"

namespace TIGER_PrintDatas
{
    struct scanLineDatas;
    struct layerDatas;
    class CScanPath : public CPrintDatas
    {
    public:
        using CPrintDatas::CPrintDatas;
        virtual void generatePath() = 0;
        virtual void getScanBlock() override;

    protected:
        void getSingleScanBlock(layerDatas& p_layer, const int p_contourId, const RegionType p_regionType);
        scanLineDatas addLineDatas(vector<pointDatas> p_points, contourType p_contourType);

    private:
        // printSLCDatas m_pSLCDatas; // SLC打印数据
        // vector<scanBlockDatas> m_scanBlocks; // 扫描块数据
        // vector<scanLineDatas> m_scanLines; // 扫描线数据
        vector<bool> m_used;
    };

    CScanPath *pathCreator(TIGER_PrintDatas::ScanPathType p_type);
}