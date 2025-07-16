#include "scanPathRenderer.h"
#include <cmath>

using namespace TIGER_PrintDatas;
namespace TIGER_OpenGL
{
    void ScanPathRenderer::drawLayer(const layerDatas& layer)
    {
        auto drawArrow = [&](float x0, float y0, float x1, float y1)
        {
            float dx = x1 - x0, dy = y1 - y0;
            float len = std::sqrt(dx * dx + dy * dy);
            if (len < 1e-5f) return;
            float nx = dx / len, ny = dy / len;
            float size = 0.1f;
            float ax = x1 - size * (nx - 0.5f * ny);
            float ay = y1 - size * (ny + 0.5f * nx);
            float bx = x1 - size * (nx + 0.5f * ny);
            float by = y1 - size * (ny - 0.5f * nx);

            glBegin(GL_TRIANGLES);
            glVertex2f(x1, y1);
            glVertex2f(ax, ay);
            glVertex2f(bx, by);
            glEnd();
        };

        for (const auto& block : layer.pScanBlocks)
        {
            for (const auto& line : block.scanLines)
            {
                const auto& pts = line.points;
                if (pts.size() < 2) continue;

                switch (line.pContourType)
                {
                case ctOuter:
                    glColor3f(0.0f, 0.0f, 1.0f);
                    break;
                case ctInner:
                    glColor3f(0.0f, 1.0f, 0.0f);
                    break;
                case ctScan:
                    glColor3f(1.0f, 0.0f, 0.0f);
                    break;
                default:
                    glColor3f(0.5f, 0.5f, 0.5f);
                    break;
                }

                for (size_t i = 1; i < pts.size(); ++i)
                {
                    glBegin(GL_LINES);
                    glVertex2f(pts[i - 1].x, pts[i - 1].y);
                    glVertex2f(pts[i].x, pts[i].y);
                    glEnd();
                    drawArrow(pts[i - 1].x, pts[i - 1].y, pts[i].x, pts[i].y);
                }
            }
        }
    }
}
