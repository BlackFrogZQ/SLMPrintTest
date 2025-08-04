#include "markGraphCreator.h"
#include "circle/circle.h"

TIGER_MarkGraph::IMarkGraph* m_pMarkGraph = nullptr;
namespace TIGER_MarkGraph
{
    IMarkGraph* CMarkGraphCreator::createMarkGraph(const CMarkShapeParas& p_markShapeParas)
    {
        switch (p_markShapeParas.shapeType)
        {
        case cmsCircle:
            m_pMarkGraph = new Circle();
            break;
        default:
            break;
        }
        assert(m_pMarkGraph != nullptr);
        return m_pMarkGraph;
    }

    void CMarkGraphCreator::freeMarkGraphObject()
    {
        if (m_pMarkGraph != nullptr)
        {
            delete m_pMarkGraph;
            m_pMarkGraph = nullptr;
        }
    }
}
