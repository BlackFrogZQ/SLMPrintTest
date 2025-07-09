#include "scanPath.h"
#include "scanPathDef.h"
#include "basicDef.h"
#include "lineFill/lineFill.h"
#include "contourFill/contourFill.h"
#include "blockFill/blockFill.h"
#include "../printDatasDef.h"
#include <vector>
#include <cmath>
#include <cstdint>

using namespace std;
namespace TIGER_PrintDatas
{
    void CScanPath::getScanBlock()
    {
        for (size_t i_layer = 0; i_layer < m_pSLCDatas.pLayerDatas.size(); i_layer++)
        {
            layerDatas& pLayer = m_pSLCDatas.pLayerDatas[i_layer];
            const auto& contours = pLayer.pContours;
            m_used.resize(contours.size(), false);

            // 添加支撑轮廓扫描块
            if(pLayer.existSupport)
            {
                for (size_t i_contour = 0; i_contour < contours.size(); i_contour++)
                {
                    if(contours[i_contour].isModelContour || m_used[i_contour]) continue;
                    getSingleScanBlock(pLayer, i_contour, rtSupportContour);
                }
            }

            // 添加模型轮廓扫描块
            if(pLayer.existModel)
            {
                for (size_t i_contour = 0; i_contour < contours.size(); i_contour++)
                {
                    if(!contours[i_contour].isModelContour || m_used[i_contour]) continue;
                    getSingleScanBlock(pLayer, i_contour, rtModelContour);
                }
            }
        }
    }

    void CScanPath::getSingleScanBlock(layerDatas& p_layer, const int p_contourId, const RegionType p_regionType)
    {
        const auto& contours = p_layer.pContours;
        const auto& outerPoints = contours[p_contourId].points;
        scanBlockDatas pBlock;
        pBlock.pRegionType = p_regionType;

        // 如果轮廓是未闭合的轮廓，直接添加为单独的扫描块
        if(!contours[p_contourId].isClosed)
        {
            m_used[p_contourId] = true;
            pBlock.scanLines.push_back(addLineDatas(outerPoints, ctOuter));
            p_layer.pScanBlocks.push_back(pBlock);
            return;
        }

        if (m_used[p_contourId] || !contours[p_contourId].isOuterContour) return;
        m_used[p_contourId] = true;

        // 寻找外轮廓内部所有的内轮廓
        vector<int> innerIdx;
        for (size_t j = 0; j < contours.size(); ++j)
        {
            if (m_used[j] || contours[p_contourId].isOuterContour || !contours[p_contourId].isClosed) continue;
            if((p_regionType == rtModelContour && !contours[p_contourId].isModelContour) || (p_regionType == rtSupportContour && contours[p_contourId].isModelContour)) continue;
            if (pointInPolygon(contours[j].points[0], outerPoints))
            {
                innerIdx.push_back(j);
                m_used[j] = true;
            }
        }

        // 添加外轮廓
        pBlock.scanLines.push_back(addLineDatas(outerPoints, ctOuter));

        // 添加内轮廓
        for (int idx : innerIdx)
        {
            pBlock.scanLines.push_back(addLineDatas(contours[idx].points, ctInner));
        }

        // 添加扫描路径
        //TODO: 这里需要根据实际的扫描路径算法来生成扫描线数据
        scanLineDatas pScanLine;
        pScanLine.pContourType = ctScan;
        switch (p_regionType)
        {
        case rtModelContour:
            /* code */
            break;
        case rtSupportContour:
            /* code */
            break;
        default:
            break;
        }
        pBlock.scanLines.push_back(pScanLine);

        p_layer.pScanBlocks.push_back(pBlock);
    }

    scanLineDatas CScanPath::addLineDatas(vector<pointDatas> p_points, contourType p_contourType)
    {
        scanLineDatas pScanLine;
        pScanLine.pContourType = p_contourType;
        for (const auto& pt : p_points)
        {
            pScanLine.points.push_back({pt.x, pt.y, 0.0f, 0.0f});
        }
        return pScanLine;
    }
}

namespace TIGER_PrintDatas
{
    CScanPath *pathCreator(TIGER_PrintDatas::ScanPathType p_type)
    {
        TIGER_PrintDatas::CScanPath *path = nullptr;
        switch (p_type)
        {
            case TIGER_PrintDatas::sptLineFill:
                path = new TIGER_PrintDatas::CLineFill();
                break;
            default:
                path = nullptr;
                break;
        }
        return path;
    }
}