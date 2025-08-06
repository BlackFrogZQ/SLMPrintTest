#pragma once
#include "iMarkGraph.h"
#include "markGraphDef.h"

namespace TIGER_MarkGraph
{
    class CMarkGraphCreator
    {
    public:
        static IMarkGraph* createMarkGraph(const CMarkShape p_shapeType);
    };
}
