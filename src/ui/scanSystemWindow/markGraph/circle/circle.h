#pragma once
#include "../iMarkGraph.h"

namespace TIGER_MarkGraph
{
    class CCircle : public IMarkGraph
    {
    public:
        virtual TIGER_PrintDatas::layerDatas* getGraphDatas(const CMarkShapeParas *p_markShapeParas) override;
    };
}