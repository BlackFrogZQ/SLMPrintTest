#include "manuDef.h"
#include "system/systemService.h"
#include <QImage>
namespace TIGER_SLMManuDef
{
    static CManuStatus g_manuStatus;
    CManuStatus *manuStatus()
    {
        return &g_manuStatus;
    }
    void CManuStatus::updateLayerStatus()
    {
        // assert(currentLayer >= 0 && currentLayer < printImages.size());
        // printer()->setDatas(QImage(printImages[currentLayer]));
        // layerStatus.clear();
        // layerStatus.regions.append(printer()->calcuPrintRegions());
        // for (const auto &region : layerStatus.regions)
        // {
        //     layerStatus.regionFrames.enqueue(matrixPrintHead()->getFrames(region.picRect, region.positiveMove));
        // }
    }
}