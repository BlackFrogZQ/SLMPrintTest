#include "spiralFill.h"

namespace TIGER_MarkGraph
{
    markGraphDatas* CSpiralFill::getGraphDatas(const CMarkShapeParas *p_markShapeParas)
    {
        assert(p_markShapeParas != nullptr);
        auto* p_graph = new markGraphDatas;

        const unsigned int rows = p_markShapeParas->rowAndCol;
        const unsigned int cols = p_markShapeParas->rowAndCol;
        const float range = static_cast<float>(p_markShapeParas->markRange);
        const float diameter = static_cast<float>(p_markShapeParas->diameter);
        const float radius = diameter / 2.0f;
        const float spacingX = range / (cols - 1);
        const float spacingY = range / (rows - 1);
        const float k = laserWidth / (2.0f * static_cast<float>(M_PI));  // 螺距系数

        for (unsigned int row = 0; row < rows; ++row)
        {
            for (unsigned int col = 0; col < cols; ++col)
            {
                float centerX = -range / 2.0f + col * spacingX;
                float centerY =  range / 2.0f - row * spacingY;

                markLineDatas spiralLine;
                float theta = 0.0f;
                const float dTheta = 0.1f;

                while (true)
                {
                    float r = k * theta;
                    if (r > radius)
                        break;

                    float x = centerX + r * std::cos(theta);
                    float y = centerY + r * std::sin(theta);
                    spiralLine.points.push_back({ x, y, 0.0f, 0.0f });

                    theta += dTheta;
                }

                if (!spiralLine.points.empty())
                    p_graph->markLines.push_back(spiralLine);
            }
        }

        return p_graph;
    }
}