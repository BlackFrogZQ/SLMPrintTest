#include "printDatas.h"
#include "printDatasDef.h"
#include <QImage>

namespace TIGER_PrintDatas
{
    CPrintDatas::CPrintDatas()
    {
    }

    CPrintDatas::~CPrintDatas()
    {
    }

    vector<vector<lineSegment>> CPrintDatas::setDatas(QImage p_buffer)
    {
        assert(!p_buffer.isNull());
        if (p_buffer.format() != QImage::Format_Grayscale8)
        {
            p_buffer = p_buffer.convertToFormat(QImage::Format_Grayscale8);
        }

        const int width = p_buffer.width();
        const int height = p_buffer.height();
        const int cx = width / 2;
        const int cy = height / 2;

        vector<vector<lineSegment>> allSegments;
        allSegments.reserve(height);

        for (int row_y = 0; row_y < height; row_y++)
        {
            vector<lineSegment> rowSegments;
            rowSegments.reserve(width);
            bool isSegment = false;
            int xStart = 0;
            uchar *p = (uchar *)p_buffer.constScanLine(row_y);

            for (int col_x = 0; col_x < width; col_x++, p++)
            {
                if ((*p) < 128 && !isSegment)
                {
                    xStart = col_x;
                    isSegment = true;
                }
                else if (((*p) >= 128 || col_x == width - 1) && isSegment)
                {
                    int xEnd = ((*p) >= 128) ? col_x - 1 : col_x;

                    // 坐标变换：图像中心为原点，右为x正，上为y正
                    int transformedXStart = xStart - cx;
                    int transformedXEnd = xEnd - cx;
                    int transformedY = cy - row_y;

                    rowSegments.push_back({transformedXStart, transformedXEnd, transformedY});
                    isSegment = false;
                }
            }

            if (!rowSegments.empty())
            {
                allSegments.push_back(rowSegments);
            }
        }

        return allSegments;
    }

    CPrintDatas *printDatas()
    {
        static CPrintDatas p_printDatas;
        return &p_printDatas;
    }
}