#pragma once
#include "printDatasDef.h"
#include <vector>
#include <iostream>
#include <fstream>

class QImage;
namespace TIGER_PrintDatas
{
    class CPrintDatas
    {
    public:
        CPrintDatas();
        virtual ~CPrintDatas();
        virtual void getScanBlock() = 0;

        std::vector<std::vector<lineSegment>> getImageDatas(QImage p_buffer);
        printSLCDatas getModelDatas(const std::string& p_fileName, bool p_isModel);
        printSLCDatas getModelAndSupportDatas(const std::string& p_modelFileName, const std::string& p_supportFileName);

    protected:
        printSLCDatas m_pSLCDatas;
    };
}

TIGER_PrintDatas::CPrintDatas *printDatas();