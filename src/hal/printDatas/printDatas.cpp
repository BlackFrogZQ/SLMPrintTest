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

        vector<vector<lineSegment>> allSegments;
        allSegments.reserve(p_buffer.height());
        for (int row_y = 0; row_y < p_buffer.height(); row_y++)
        {
            vector<lineSegment> rowSegments;
            rowSegments.reserve(p_buffer.width());
            bool isSegment = false;
            int xStart = 0;
            uchar *p = (uchar *)p_buffer.constScanLine(row_y);
            for (int col_x = 0; col_x < p_buffer.width(); col_x++, p++)
            {
                if ((*p) < 128 && !isSegment)
                {
                    xStart = col_x;
                    isSegment = true;
                }
                else if (((*p) >= 128  || col_x == p_buffer.width() - 1) && isSegment)
                {
                    int xEnd = ((*p) >= 128) ? col_x - 1 : col_x;
                    rowSegments.push_back({xStart, xEnd, row_y});
                    isSegment = false;
                }
            }
            if(!rowSegments.empty())
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