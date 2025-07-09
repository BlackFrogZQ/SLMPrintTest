#pragma once
#include "../scanPath.h"

namespace TIGER_PrintDatas
{
    class CLineFill : public CScanPath
    {
    public:
        using CScanPath::CScanPath;
        virtual void generatePath() override;
    };
}