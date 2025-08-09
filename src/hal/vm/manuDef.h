#pragma once
#include "system/basic.h"
#include "hal/printDatas/basicDef.h"
#include "hal/printDatas/printDatas.h"
#include "hal/printDatas/printDatasDef.h"
#include <QQueue>

using namespace TIGER_PrintDatas;
namespace TIGER_SLMManuDef
{
    struct CLayerStatus
    {
        std::vector<std::vector<lineSegment>> allSegments;
        printSLCDatas allSLCLayers;
        layerDatas currentSLCLayer;
        void clear()
        {
            allSegments.clear();
            allSLCLayers = printSLCDatas();
            currentSLCLayer = layerDatas();
        }
    };
    struct CManuStatus
    {
        CLayerStatus layerStatus;
        QList<QString> printImages;
        int currentLayer = 0;
        void updateLayerStatus();
        printSLCDatas getSLCPrintDatas(const QString& p_modelFilename);
        void updataSLCLayerStatus();
        bool existSLCDatas = false;
    };
    CManuStatus *manuStatus();
}