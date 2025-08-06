#include "markGraphCreator.h"
#include "circle/circle.h"
#include "grid/grid.h"
#include "spiralFill/spiralFill.h"

namespace TIGER_MarkGraph
{
    IMarkGraph* CMarkGraphCreator::createMarkGraph(const CMarkShape p_shapeTypes)
    {
        switch (p_shapeTypes)
        {
        case cmsCircle:
            return new CCircle();
        case cmsGrid:
            return new CGrid();
        case cmsSpiralFill:
            return new CSpiralFill();
        default:
            return nullptr;
        }
    }
}
