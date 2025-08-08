#include "grid.h"

using namespace TIGER_PrintDatas;
namespace TIGER_MarkGraph
{
    layerDatas* CGrid::getGraphDatas(const CMarkShapeParas *p_markShapeParas)
    {
        assert(p_markShapeParas != nullptr);
        auto* p_graph = new layerDatas;

        const unsigned int rows = p_markShapeParas->rowAndCol;
        const unsigned int cols = p_markShapeParas->rowAndCol;
        const float range = static_cast<float>(p_markShapeParas->markRange);
        const float spacingX = range / (cols - 1);
        const float spacingY = range / (rows - 1);
        const float leftX = -range / 2.0f;
        const float topY = range / 2.0f;

        scanBlockDatas block;
        // 横线（Y不变，X从左到右）
        for (unsigned int row = 0; row < rows; ++row)
        {
            float y = topY - row * spacingY;
            scanLineDatas hLine;
            hLine.pMarkType = TIGER_VMSLM::cmtMarkTest;
            hLine.points.push_back({ leftX, y, 0, 0 });
            hLine.points.push_back({ leftX + range, y, 0, 0 });
            block.scanLines.push_back(hLine);
        }
        // 竖线（X不变，Y从上到下）
        for (unsigned int col = 0; col < cols; ++col)
        {
            float x = leftX + col * spacingX;
            scanLineDatas vLine;
            vLine.pMarkType = TIGER_VMSLM::cmtMarkTest;
            vLine.points.push_back({ x, topY, 0, 0 });
            vLine.points.push_back({ x, topY - range, 0, 0 });
            block.scanLines.push_back(vLine);
        }
        p_graph->pScanBlocks.push_back(block);

        return p_graph;
    }
}