#pragma once
#include <vector>
class QImage;
namespace TIGER_PrintDatas
{
    struct lineSegment;
}
namespace TIGER_PrintDatas
{
    class CPrintDatas
    {
    public:
        CPrintDatas();
        ~CPrintDatas();
        std::vector<std::vector<TIGER_PrintDatas::lineSegment>> setDatas(QImage p_buffer);
        // QList<CPrintRegion> calcuPrintRegions();
    };

    CPrintDatas *printDatas();
}