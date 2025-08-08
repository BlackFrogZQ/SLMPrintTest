#include "lineFill.h"
#include "../../iFillTypeDef.h"
#include "../../../basicDef.h"

using namespace std;
namespace TIGER_PrintDatas
{
    vector<scanLineDatas> CLineFill::generateRegionFill(const vector<pointDatas>& p_outer, const vector<vector<pointDatas>>& p_holes, float p_lineWidth, float p_angleDegree, ScanDirection p_dirMode)
    {
        vector<scanLineDatas> pScanLines;

        // 1.外轮廓收缩，孔洞膨胀，避免重熔情况。
        int64_t offsetInt = int64_t(p_lineWidth * 0.5f * 1e6);

        Paths64 safeOuter = InflatePaths({toClip(p_outer)}, -offsetInt, JoinType::Square, EndType::Polygon);

        Paths64 bigHoles;
        for (const auto& h : p_holes)
        {
            Paths64 inflated = InflatePaths({toClip(h)}, offsetInt, JoinType::Square, EndType::Polygon);
            bigHoles.insert(bigHoles.end(), inflated.begin(), inflated.end());
        }

        Paths64 fillRegions = BooleanOp(ClipType::Difference, FillRule::NonZero, safeOuter, bigHoles);

        // 2.扫描角度
        float rad = p_angleDegree * float(M_PI) / 180.f;
        float cosA = cos(rad), sinA = sin(rad);

        // 3.生成扫描线
        for (const auto& singleRegion : fillRegions) // 遍历每个填充区域
        {
            auto polyF = fromClip(singleRegion);

            // 旋转
            vector<pointDatas> P2;
            P2.reserve(polyF.size());
            for (auto &p : polyF)
            {
                P2.push_back({ p.x * cosA + p.y * sinA, -p.x * sinA + p.y * cosA });
            }

            // Y 范围
            float minY = P2[0].y, maxY = P2[0].y;
            for (auto &p : P2)
            {
                minY = min(minY, p.y);
                maxY = max(maxY, p.y);
            }

            // 扫描每一行
            int row = 0;
            for (float y = minY; y <= maxY; y += p_lineWidth, ++row)
            {
                vector<float> xs;
                int n = P2.size();

                // 寻找与扫描线相交的 X 点
                for (int i = 0, j = n - 1; i < n; j = i++)
                {
                    const auto &A = P2[j], &B = P2[i];
                    if ((A.y <= y && B.y > y) || (B.y <= y && A.y > y))
                    {
                        float t = (y - A.y) / (B.y - A.y);
                        xs.push_back(A.x + t * (B.x - A.x));
                    }
                }
                if (xs.size() < 2) continue;
                // 排序交点（从左到右）
                sort(xs.begin(), xs.end());

                // 每两个相邻交点生成一条扫描线段
                for (size_t k = 0; k + 1 < xs.size(); k += 2)
                {
                    pointDatas S{xs[k], y}, E{xs[k+1], y};  // 线段起点和终点
                    pointDatas M{(S.x + E.x) * 0.5f, y};    // 线段中点

                    // 重要：验证中点是否在多边形内（避免生成无效线段）
                    if (!pointInPolygon(M, P2)) continue;

                    scanLineDatas ln;
                    ln.pContourType = ctScan;
                    ln.pMarkType = TIGER_VMSLM::cmtScanFill;

                    // 确定扫描方向：
                    // - 单向模式(sdUnidirectional)总是正向
                    // - 双向模式(sdBidirectional)奇数行反向
                    bool forward = (p_dirMode == sdUnidirectional) || ((row & 1) == 0);

                    // 定义逆向旋转函数（将坐标转回原始坐标系）
                    auto rotateBack = [&](const pointDatas& p) -> pointDatas
                    {
                        return
                        {
                            p.x * cosA - p.y * sinA,
                            p.x * sinA + p.y * cosA
                        };
                    };

                    // 根据方向生成线段端点：正向扫描（左->右）；反向扫描（右->左）
                    if (forward)
                    {
                        auto p1 = rotateBack(S);
                        auto p2 = rotateBack(E);
                        ln.points.push_back({ p1.x, p1.y, 0, 0 });
                        ln.points.push_back({ p2.x, p2.y, 0, 0 });
                    }
                    else
                    {
                        auto p1 = rotateBack(E);
                        auto p2 = rotateBack(S);
                        ln.points.push_back({ p1.x, p1.y, 0, 0 });
                        ln.points.push_back({ p2.x, p2.y, 0, 0 });
                    }
                    pScanLines.push_back(move(ln));
                }
            }
        }

        return pScanLines;
    }
}