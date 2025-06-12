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
        layerStatus.allSegments = printDatas()->setDatas(QImage(printImages[currentLayer]));
    }
}