#include "printDatas.h"
#include "printDatasDef.h"
#include <QImage>
#include <cstdint>
#include <array>
#include <string>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>

namespace TIGER_PrintDatas
{
    CPrintDatas::CPrintDatas()
    {
    }

    CPrintDatas::~CPrintDatas()
    {
    }

    vector<vector<lineSegment>> CPrintDatas::getImageDatas(QImage p_buffer)
    {
        assert(!p_buffer.isNull());
        if (p_buffer.format() != QImage::Format_Grayscale8)
        {
            p_buffer = p_buffer.convertToFormat(QImage::Format_Grayscale8);
        }

        const float width = p_buffer.width();
        const float height = p_buffer.height();
        const float cx = width / 2;
        const float cy = height / 2;
        CLaserPara *laserParas = new CLaserPara(0.0765, 300);

        vector<vector<lineSegment>> allSegments;
        allSegments.reserve(height);

        for (int row_y = 0; row_y < height; row_y++)
        {
            vector<lineSegment> rowSegments;
            rowSegments.reserve(width);
            bool isSegment = false;
            float xStart = 0;
            uchar *p = (uchar *)p_buffer.constScanLine(row_y);

            for (int col_x = 0; col_x < width; col_x++, p++)
            {
                if ((*p) < 128 && !isSegment)
                {
                    xStart = col_x;
                    isSegment = true;
                }
                else if (((*p) >= 128 || col_x == width - 1) && isSegment)
                {
                    float xEnd = ((*p) >= 128) ? col_x - 1 : col_x;

                    // 坐标变换：图像中心为原点，右为x正，上为y正。并转换为单位为mm
                    float transformedXStart = (xStart - cx) * laserParas->laserLineWidth;
                    float transformedXEnd = (xEnd - cx) * laserParas->laserLineWidth;
                    float transformedY = (cy - row_y) * laserParas->laserLineWidth;

                    rowSegments.push_back({transformedXStart, transformedXEnd, transformedY});
                    isSegment = false;
                }
            }

            if (!rowSegments.empty())
            {
                allSegments.push_back(rowSegments);
            }
        }
        delete laserParas;
        return allSegments;
    }

    printSLCDatas CPrintDatas::getModelDatas(const string& p_fileName, bool p_isModel)
    {
        size_t pBbyteIndex = 0;
        ifstream file(p_fileName, ios::binary);
        ostringstream pStringstream;
        string pSLCByteContent;
        pStringstream << file.rdbuf();
        pSLCByteContent = pStringstream.str();

        pBbyteIndex = pSLCByteContent.find("\x0d\x0a\x1a") + 3;  // 跳过头部直到遇到 0x0D 0x0A 0x1A 序列,跳过这三个字节，把pBbyteIndex定位到头部标志之后的第一字节位置
        pBbyteIndex = pBbyteIndex + 256;                         // 跳过256字节保留区
        uint8_t size = read<uint8_t>(pSLCByteContent, pBbyteIndex);

        printSLCDatas pSLCDatas;
        pSLCDatas.isModelSlice = p_isModel;
        for (size_t i = 0; i < static_cast<int>(size); i++)
        {
            pSLCDatas.initialHeight = read<float>(pSLCByteContent, pBbyteIndex);
            pSLCDatas.layerThickness = read<float>(pSLCByteContent, pBbyteIndex);
            pSLCDatas.lineWidth = read<float>(pSLCByteContent, pBbyteIndex);
            pSLCDatas.reservedSize = read<float>(pSLCByteContent, pBbyteIndex);
        }

        while (true)
        {
            float currentLayerHeight = read<float>(pSLCByteContent, pBbyteIndex);
            uint32_t boundariesNum = read<uint32_t>(pSLCByteContent, pBbyteIndex);
            if (boundariesNum == 0xffffffff)                     //读取到尾部退出
            {
                break;
            }

            layerDatas pLayer;
            pLayer.z = currentLayerHeight;
            pLayer.pContours.reserve(boundariesNum);
            for (size_t i = 0; i < boundariesNum; i++)
            {
                uint32_t verticesNum = read<uint32_t>(pSLCByteContent, pBbyteIndex);
                uint32_t gapsNum = read<uint32_t>(pSLCByteContent, pBbyteIndex);
                countourDatas contour;
                contour.points.reserve(verticesNum);
                contour.isModelContour = p_isModel;
                for (size_t j = 0; j < verticesNum; j++)
                {
                    pointDatas pPoint;
                    float x = read<float>(pSLCByteContent, pBbyteIndex);
                    float y = read<float>(pSLCByteContent, pBbyteIndex);
                    pPoint.x = x;
                    pPoint.y = y;
                    contour.points.push_back(pPoint);
                }
                pLayer.pContours.push_back(move(contour));
            }
            pSLCDatas.allSLCLayers.push_back(move(pLayer));
        }
        return pSLCDatas;
    }

    printSLCDatas CPrintDatas::getModelAndSupportDatas(const string& p_modelFileName, const string& p_supportFileName)
    {
        printSLCDatas modelDatas = getModelDatas(p_modelFileName, true);
        printSLCDatas sliceDatas = getModelDatas(p_supportFileName, false);

        printSLCDatas mergedDatas;
        mergedDatas.initialHeight  = sliceDatas.initialHeight;
        mergedDatas.layerThickness = sliceDatas.layerThickness;
        mergedDatas.lineWidth = modelDatas.lineWidth;
        mergedDatas.reservedSize = modelDatas.reservedSize;

        const auto& pModelLayers = modelDatas.allSLCLayers;
        const auto& pSliceLayers = sliceDatas.allSLCLayers;
        size_t modelCount   = pModelLayers.size();
        size_t supportCount = pSliceLayers.size();

        constexpr float eps = 1e-6f;    // 用于浮点数比较的容差
        for (size_t i = 0; i < supportCount; ++i)
        {
            float z = pSliceLayers[i].z;
            layerDatas layer;
            layer.z = z;

            float idxF = (z - modelDatas.initialHeight) / modelDatas.layerThickness;
            int idx  = int(idxF + 0.5f);
            if (idx >= 0 && idx < (int)modelCount && fabs(pModelLayers[idx].z - z) < eps)
            {
                for (auto& c : pModelLayers[idx].pContours)
                {
                    layer.pContours.push_back(c);
                }
            }

            for (auto& c : pSliceLayers[i].pContours)
            {
                layer.pContours.push_back(c);
            }

            mergedDatas.allSLCLayers.push_back(std::move(layer));
        }

        for (size_t j = supportCount; j < modelCount; ++j)
        {
            layerDatas layer;
            layer.z = pModelLayers[j].z;
            for (auto& c : pModelLayers[j].pContours)
            {
                layer.pContours.push_back(c);
            }
            mergedDatas.allSLCLayers.push_back(std::move(layer));
        }

        return mergedDatas;
    }

    CPrintDatas *printDatas()
    {
        static CPrintDatas p_printDatas;
        return &p_printDatas;
    }
}