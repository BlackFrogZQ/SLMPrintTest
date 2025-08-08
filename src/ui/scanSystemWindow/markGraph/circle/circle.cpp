#include "circle.h"

using namespace TIGER_PrintDatas;
namespace TIGER_MarkGraph
{
    layerDatas* CCircle::getGraphDatas(const CMarkShapeParas *p_markShapeParas)
    {
        assert(p_markShapeParas != nullptr);
        auto* p_graph = new layerDatas;

        const unsigned int rows = p_markShapeParas->rowAndCol;
        const unsigned int cols = p_markShapeParas->rowAndCol;
        const float range = static_cast<float>(p_markShapeParas->markRange);
        const float diameter = static_cast<float>(p_markShapeParas->diameter);
        const float radius = diameter / 2.0f;
        const float spacingX = range / (cols - 1);
        const float spacingY = range / (rows - 1);

        scanBlockDatas block;
        for (unsigned int row = 0; row < rows; ++row)
        {
            for (unsigned int col = 0; col < cols; ++col)
            {
                float centerX = -range / 2.0f + col * spacingX;
                float centerY =  range / 2.0f - row * spacingY;

                scanLineDatas circleLine;
                circleLine.pMarkType = TIGER_VMSLM::cmtMarkTest;
                const int pointCount = 61;
                const float angleStep = 2 * static_cast<float>(M_PI) / (pointCount - 1);

                for (int i = 0; i < pointCount; ++i)
                {
                    float angle = i * angleStep;
                    printPoint pt;
                    pt.x = centerX + radius * std::cos(angle);
                    pt.y = centerY + radius * std::sin(angle);
                    pt.z = 0.0f;
                    pt.a = 0.0f;
                    circleLine.points.push_back(pt);
                }
                block.scanLines.push_back(circleLine);
            }
        }
        p_graph->pScanBlocks.push_back(block);

        return p_graph;
    }
}