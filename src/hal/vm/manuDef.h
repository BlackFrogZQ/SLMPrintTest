#pragma once
/*
    加工相关参数定义，默认位置单位mm，速度单位mm/s,时间单位ms
*/
#include "system/basic.h"
#include "hal/printDatas/basicDef.h"
#include "hal/printDatas/printDatas.h"
#include "hal/printDatas/printDatasDef.h"
#include <QQueue>
namespace TIGER_SLMManuDef
{
    struct CLayerStatus
    {
        vector<vector<TIGER_PrintDatas::lineSegment>> allSegments;
        TIGER_PrintDatas::printSLCDatas SLCLayersStatus;
        TIGER_PrintDatas::layerDatas currentSLCLayer;
        void clear()
        {
            allSegments.clear();
            currentSLCLayer = TIGER_PrintDatas::layerDatas();
        }
    };
    struct CManuStatus
    {
        CLayerStatus layerStatus;
        QList<QString> printImages;
        int currentLayer = 0;
        void updateLayerStatus();
        TIGER_PrintDatas::printSLCDatas getSLCPrintDatas(const string& filename);
        void updataSLCLayerStatus();
    };
    CManuStatus *manuStatus();
}