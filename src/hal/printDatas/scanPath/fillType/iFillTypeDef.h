#pragma once
#include "clipper2/clipper.h"
#include "../scanPathDef.h"
#include "../../printDatasDef.h"
using namespace Clipper2Lib;

namespace TIGER_PrintDatas
{
    constexpr double M_PI = 3.14159265358979323846;
    using Path64 = Path<int64_t>;
    using Paths64 = Paths<int64_t>;

    static Path64 toClip(const std::vector<pointDatas>& P)
    {
        Path64 r;
        r.reserve(P.size());
        for (auto& p : P)
            r.emplace_back(int64_t(p.x * 1e6), int64_t(p.y * 1e6));
        return r;
    };

    static std::vector<pointDatas> fromClip(const Path64& P)
    {
        std::vector<pointDatas> r;
        r.reserve(P.size());
        for (auto& v : P)
            r.push_back({v.x / 1e6f, v.y / 1e6f});
        return r;
    };

    enum ScanDirection
    {
        sdUnidirectional = 0,
        sdBidirectional,
        sdUnknown
    };
}