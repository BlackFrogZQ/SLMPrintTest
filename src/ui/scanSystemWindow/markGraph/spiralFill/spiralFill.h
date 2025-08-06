#pragma once
#include "../iMarkGraph.h"

namespace TIGER_MarkGraph
{
    class CSpiralFill : public IMarkGraph
    {
    public:
        virtual markGraphDatas* getGraphDatas(const CMarkShapeParas *p_markShapeParas) override;
    };
}