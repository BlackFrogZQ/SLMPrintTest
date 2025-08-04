#pragma once
#include "markGraphDef.h"
namespace TIGER_MarkGraph
{
    class IMarkGraph
    {
    public:
        IMarkGraph();
        ~IMarkGraph();

    protected:
        virtual void getGraphDatas() = 0;

    protected:
        CMarkShapeParas m_markShapeParas;
    };
}