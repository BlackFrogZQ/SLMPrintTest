#pragma once
#include "../iMarkGraph.h"

namespace TIGER_MarkGraph
{
    class Circle : public IMarkGraph
    {
    public:
        Circle();
        ~Circle();

    protected:
        void getGraphDatas() override;
    };
}