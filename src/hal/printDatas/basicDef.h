#pragma once
#include <memory.h>
#include <assert.h>
#include "system/basic.h"

namespace TIGER_PrintDatas
{
    constexpr double cInch = 25.4;
#define MINVALUE 0.0001

    // 将点数转换为DPI: 根据字体大小（pointSize）和目标长度（len），计算所对应的 DPI（每英寸点数）值; (pointSize / len)表示pointSize有多长
    constexpr int toDpi(int pointSize, double len)
    {
        return int(cInch * pointSize / len);
    }

    // 将DPI转换为长度: 根据字体大小（pointSize）和 DPI 值，计算字体在实际显示/打印时所占的物理长度; (pointSize / dpi)表示pointSize有多少英寸
    constexpr double toDpiLen(int pointSize, int dpi)
    {
        return cInch * pointSize / dpi;
    }

    constexpr int doubleToInt(double p_double)
    {
        return int(p_double + 0.5);
    }

    template <typename T>
    struct CRect
    {
        T left, top, w, h;
        CRect(const T &p_left = T(0.0), const T &p_top = T(0.0), const T &p_w = T(0.0), const T &p_h = T(0.0)) : left(p_left), top(p_top), w(p_w), h(p_h)
        {
            assert(w >= 0);
            assert(h >= 0);
        };
        T right() const
        {
            return left + w;
        };
        T bottom() const
        {
            return top + h;
        };
        bool operator==(const CRect<T> &p_rect) const
        {
            return p_rect.left == this->left && p_rect.top == this->top && p_rect.w == this->w && p_rect.h == this->h;
        }
        friend QDebug operator<<(QDebug out, const CRect<T> &p_rect)
        {
            out.noquote() << cnStr("CRect(left:%1,top:%2,w:%3,h:%4)").arg(p_rect.left).arg(p_rect.top).arg(p_rect.w).arg(p_rect.h);
            return out;
        }
    };

    template <typename T>
    struct CPoint
    {
        T x, y;
        CPoint(const T &p_x = T(0.0), const T &p_y = T(0.0)) : x(p_x), y(p_y){};
    };

    using CPointi = CPoint<int>;
    using CPointd = CPoint<double>;
    using CRecti = CRect<int>;
    using CRectd = CRect<double>;

    struct CPrintRegion
    {
        bool positiveMove;
        CRecti picRect;
        double horizontalPos;
        double printHeadStartPos;
        double printHeadEndPos;
        friend QDebug operator<<(QDebug out, const CPrintRegion &p_region)
        {
            out.noquote() << cnStr("CPrintRegion(") << Qt::endl
                          << cnStr("\tpositiveMove:%1,").arg(p_region.positiveMove) << Qt::endl
                          << cnStr("\thorizontalPos:%1,").arg(p_region.horizontalPos) << Qt::endl
                          << cnStr("\tprintHeadStartPos:%1,").arg(p_region.printHeadStartPos) << Qt::endl
                          << cnStr("\tprintHeadEndPos:%1,").arg(p_region.printHeadEndPos) << Qt::endl
                          << cnStr("\tpicRect:") << p_region.picRect << Qt::endl
                          << cnStr(")") << Qt::endl;
            return out;
        }
    };
}