#pragma once
#include "../printDatasDef.h"
#include <vector>

namespace TIGER_PrintDatas
{
    // 判断点是否在多边形内（射线法）
    // 从待测点向右（或向左）发出一条水平射线，统计该射线与多边形边的交叉次数。奇数次交叉：点在多边形内部；偶数次交叉：点在多边形外部
    static inline bool pointInPolygon(const pointDatas& pt, const std::vector<pointDatas>& poly)
    {
        bool inside = false;
        for (size_t i = 0, j = poly.size() - 1; i < poly.size(); j = i++)
        {
            if (((poly[i].y > pt.y) != (poly[j].y > pt.y)) && (pt.x < (poly[j].x - poly[i].x) * (pt.y - poly[i].y) / (poly[j].y - poly[i].y) + poly[i].x))
            {
                inside = !inside;
            }
        }
        return inside;
    }
}