#pragma once
#include "markGraphDef.h"
#include "hal/vm/ncDef.h"

namespace TIGER_MarkGraph
{
    class IMarkGraph
    {
    public:
        virtual TIGER_PrintDatas::layerDatas* getGraphDatas(const CMarkShapeParas *p_markShapeParas) = 0;

    protected:
        TIGER_PrintDatas::layerDatas showPathIndicate();
    };
}