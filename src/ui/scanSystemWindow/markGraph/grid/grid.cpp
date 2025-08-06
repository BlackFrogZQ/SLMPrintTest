#include "grid.h"

namespace TIGER_MarkGraph
{
    markGraphDatas* CGrid::getGraphDatas(const CMarkShapeParas *p_markShapeParas)
    {
        assert(p_markShapeParas != nullptr);
        auto* p_graph = new markGraphDatas;

        const unsigned int rows = p_markShapeParas->rowAndCol;
        const unsigned int cols = p_markShapeParas->rowAndCol;
        const float range = static_cast<float>(p_markShapeParas->markRange);
        const float spacingX = range / (cols - 1);
        const float spacingY = range / (rows - 1);
        const float leftX = -range / 2.0f;
        const float topY = range / 2.0f;

        // 横线（Y不变，X从左到右）
        for (unsigned int row = 0; row < rows; ++row)
        {
            float y = topY - row * spacingY;
            markLineDatas hLine;
            hLine.points.push_back({ leftX, y, 0, 0 });
            hLine.points.push_back({ leftX + range, y, 0, 0 });
            p_graph->markLines.push_back(hLine);
        }
        // 竖线（X不变，Y从上到下）
        for (unsigned int col = 0; col < cols; ++col)
        {
            float x = leftX + col * spacingX;
            markLineDatas vLine;
            vLine.points.push_back({ x, topY, 0, 0 });
            vLine.points.push_back({ x, topY - range, 0, 0 });
            p_graph->markLines.push_back(vLine);
        }

        return p_graph;
    }
}