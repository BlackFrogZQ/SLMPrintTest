#pragma once
#include "iMarkGraph.h"
#include "markGraphDef.h"

namespace TIGER_MarkGraph
{
    class CMarkGraphCreator
    {
    public:
        static TIGER_MarkGraph::IMarkGraph* createMarkGraph(const CMarkShapeParas& p_markShapeParas);
        static void freeMarkGraphObject();

    private:
        static TIGER_MarkGraph::IMarkGraph* m_pMarkGraph;
    };
}