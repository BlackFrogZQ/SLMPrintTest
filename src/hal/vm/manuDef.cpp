#include "manuDef.h"
#include "system/systemService.h"
#include <QImage>
using namespace TIGER_PrintDatas;
namespace TIGER_SLMManuDef
{
    static CManuStatus g_manuStatus;
    CManuStatus *manuStatus()
    {
        return &g_manuStatus;
    }

    void CManuStatus::updateLayerStatus()
    {
        assert(currentLayer >= 0 && currentLayer < printImages.size());
        layerStatus.clear();
        layerStatus.allSegments = printDatas()->getImageDatas(QImage(printImages[currentLayer]));
    }

    printSLCDatas CManuStatus::getSLCPrintDatas(const std::string& filename)
    {
        layerStatus.SLCLayersStatus = printDatas()->getSLCDatas(filename);
        return layerStatus.SLCLayersStatus;
    }

    void CManuStatus::updataSLCLayerStatus()
    {
        assert(currentLayer >= 0 && currentLayer < layerStatus.SLCLayersStatus.allSLCLayers.size());
        layerStatus.currentSLCLayer = layerStatus.SLCLayersStatus.allSLCLayers[currentLayer];
    }
}