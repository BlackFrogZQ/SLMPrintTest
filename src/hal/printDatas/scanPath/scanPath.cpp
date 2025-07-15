#include "scanPath.h"
#include "scanPathDef.h"
#include "basicDef.h"
#include "system/basic.h"
#include "fillType/iFillType.h"
#include "fillType/modelFill/lineFill/lineFill.h"
#include "../printDatasDef.h"
#include <vector>
#include <cmath>
#include <cstdint>

using namespace std;
namespace TIGER_PrintDatas
{
    CScanPath::CScanPath()
    {
        m_pModelFillType = new CLineFill();
    }

    CScanPath::~CScanPath()
    {
        delPtr(m_pModelFillType);
    }

    void CScanPath::getScanBlock()
    {
        for (size_t layerId = 0; layerId < m_pSLCDatas.pLayerDatas.size(); layerId++)
        {
            layerDatas& pLayer = m_pSLCDatas.pLayerDatas[layerId];
            const auto& contours = pLayer.pContours;
            m_used.resize(contours.size(), false);

            // 添加支撑轮廓扫描块
            if(pLayer.existSupport)
            {
                for (size_t i_contour = 0; i_contour < contours.size(); i_contour++)
                {
                    if(contours[i_contour].isModelContour || m_used[i_contour]) continue;
                    getSingleScanBlock(pLayer, layerId, i_contour, rtSupportContour);
                }
            }

            // 添加模型轮廓扫描块
            if(pLayer.existModel)
            {
                for (size_t i_contour = 0; i_contour < contours.size(); i_contour++)
                {
                    if(!contours[i_contour].isModelContour || m_used[i_contour]) continue;
                    getSingleScanBlock(pLayer, layerId, i_contour, rtModelContour);
                }
            }
        }
    }

    void CScanPath::getSingleScanBlock(layerDatas& p_layer, const int p_layerId, const int p_contourId, const RegionType p_regionType)
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
        for (size_t contourId = 0; contourId < contours.size(); ++contourId)
        {
            if (m_used[contourId] || contours[p_contourId].isOuterContour || !contours[p_contourId].isClosed) continue;
            if((p_regionType == rtModelContour && !contours[p_contourId].isModelContour) || (p_regionType == rtSupportContour && contours[p_contourId].isModelContour)) continue;
            if (pointInPolygon(contours[contourId].points[0], outerPoints))
            {
                innerIdx.push_back(contourId);
                m_used[contourId] = true;
            }
        }

        // 添加外轮廓
        pBlock.scanLines.push_back(addLineDatas(outerPoints, ctOuter));

        // 添加内轮廓
        vector<vector<pointDatas>> holesPoints;
        for (int idx : innerIdx)
        {
            pBlock.scanLines.push_back(addLineDatas(contours[idx].points, ctInner));
            holesPoints.push_back(contours[idx].points);
        }

        // 添加扫描路径
        //TODO: 这里需要根据实际的扫描路径算法来生成扫描线数据
        vector<scanLineDatas> pScanLine;
        switch (p_regionType)
        {
        case rtModelContour:
            {
                float lineWidth = m_pLaserParas->laserWidth;
                float rotateAngle = (p_layerId % 3) * 45.0f;
                ScanDirection scanDir = sdBidirectional;
                pScanLine = m_pModelFillType->generateRegionFill(outerPoints, holesPoints, lineWidth, rotateAngle, scanDir);
            }
            break;
        case rtSupportContour:
            // generateRegionFill(const vector<pointDatas>& outer,  const vector<vector<pointDatas>>& holes);
            break;
        default:
            break;
        }
        for(auto &ln: pScanLine)
        {
            pBlock.scanLines.push_back(ln);
        }

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