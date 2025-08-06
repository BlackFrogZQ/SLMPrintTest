#pragma once
#include "markGraphDef.h"
namespace TIGER_MarkGraph
{
    class IMarkGraph
    {
    public:
        virtual markGraphDatas* getGraphDatas(const CMarkShapeParas *p_markShapeParas) = 0;

    protected:
        markLineDatas showPathIndicate();
    };
}