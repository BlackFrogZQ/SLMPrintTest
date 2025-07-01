#pragma once
#include <vector>
#include <iostream>
#include <fstream>

class QImage;
namespace TIGER_PrintDatas
{
    struct lineSegment;
    struct printSLCDatas;
}
namespace TIGER_PrintDatas
{
    class CPrintDatas
    {
    public:
        CPrintDatas();
        ~CPrintDatas();
        std::vector<std::vector<TIGER_PrintDatas::lineSegment>> getImageDatas(QImage p_buffer);
        TIGER_PrintDatas::printSLCDatas getModelDatas(const std::string& p_fileName, bool p_isModel);
        TIGER_PrintDatas::printSLCDatas getModelAndSupportDatas(const std::string& p_modelFileName, const std::string& p_supportFileName);
    };

    CPrintDatas *printDatas();
}