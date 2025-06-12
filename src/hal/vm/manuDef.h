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
        QQueue<TIGER_PrintDatas::CPrintRegion> regions;
        QQueue<QList<QQueue<TIGER_PrintDatas::CFrameData>>> regionFrames;
        TIGER_PrintDatas::CPrintRegion currentRegion;
        QList<QQueue<TIGER_PrintDatas::CFrameData>> currentFrames;
        vector<vector<TIGER_PrintDatas::lineSegment>> allSegments;
        void clear()
        {
            regions.clear();
            regionFrames.clear();
            currentRegion = TIGER_PrintDatas::CPrintRegion();
            currentFrames.clear();
            allSegments.clear();
        }
    };
    struct CManuStatus
    {
        QList<QString> printImages;
        int currentLayer = 0;
        CLayerStatus layerStatus;
        void updateLayerStatus();
    };
    CManuStatus *manuStatus();
}