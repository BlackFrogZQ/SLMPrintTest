#pragma once
#include "hal/printDatas/printDatasDef.h"
#include <QOpenGLFunctions>

namespace TIGER_OpenGL
{
    class ScanPathRenderer : public QOpenGLFunctions
    {
    public:
        void drawLayer(const TIGER_PrintDatas::layerDatas& layer);
    };
}