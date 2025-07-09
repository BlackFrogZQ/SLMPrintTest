#include "manuDef.h"
#include "system/systemService.h"
#include <QImage>
#include <QFileInfo>
#include <QDir>

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

    printSLCDatas CManuStatus::getSLCPrintDatas(const QString& p_modelFilename)
    {
        assert(!p_modelFilename.isEmpty());
        layerStatus.clear();
        QFileInfo fi(p_modelFilename);
        QString dir = fi.absolutePath();
        QString baseName = fi.completeBaseName();
        QString suffix = fi.suffix();
        QString supportFile = QDir(dir).filePath(baseName + "_s." + suffix);
        if (QFile::exists(supportFile))
        {
            layerStatus.allSLCLayers = printDatas()->getModelAndSupportDatas(p_modelFilename.toStdString(), supportFile.toStdString());
        }
        else
        {
            layerStatus.allSLCLayers = printDatas()->getModelDatas(p_modelFilename.toStdString(), true);
        }
        // printDatas()->getScanBlock();
        return layerStatus.allSLCLayers;
    }

    void CManuStatus::updataSLCLayerStatus()
    {
        assert(currentLayer >= 0 && currentLayer < layerStatus.allSLCLayers.pLayerDatas.size());
        layerStatus.currentSLCLayer = layerStatus.allSLCLayers.pLayerDatas[currentLayer];
    }
}