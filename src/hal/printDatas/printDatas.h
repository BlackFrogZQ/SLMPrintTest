#pragma once
#include <vector>
#include <iostream>
#include <fstream>

class QImage;
namespace TIGER_PrintDatas
{
    struct lineSegment;
    struct Layer;
}
namespace TIGER_PrintDatas
{
    class CPrintDatas
    {
    public:
        CPrintDatas();
        ~CPrintDatas();
        std::vector<std::vector<TIGER_PrintDatas::lineSegment>> setDatas(QImage p_buffer);
        std::vector<Layer> getSLCDatas(const std::string& p_fileName);
    };

    CPrintDatas *printDatas();
}