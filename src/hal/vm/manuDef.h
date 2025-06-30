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
        vector<TIGER_PrintDatas::Layer> allSLCLayers;
        TIGER_PrintDatas::Layer currentSLCLayer;
        void clear()
        {
            allSegments.clear();
            allSLCLayers.clear();
            currentSLCLayer = TIGER_PrintDatas::Layer();
        }
    };
    struct CManuStatus
    {
        CLayerStatus layerStatus;
        QList<QString> printImages;
        int currentLayer = 0;
        void updateLayerStatus();
        vector<TIGER_PrintDatas::Layer> getSLCPrintDatas(const string& filename);
        void updataSLCLayerStatus();
    };
    CManuStatus *manuStatus();
}