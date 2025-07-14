#include "printDatas.h"
#include "scanPath/scanPath.h"
#include <QImage>
#include <cstdint>
#include <array>
#include <string>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
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

        m_pSLCDatas = printSLCDatas();
        for (size_t i = 0; i < static_cast<int>(size); i++)
        {
            m_pSLCDatas.initialHeight = read<float>(pSLCByteContent, pBbyteIndex);
            m_pSLCDatas.layerThickness = read<float>(pSLCByteContent, pBbyteIndex);
            m_pSLCDatas.lineWidth = read<float>(pSLCByteContent, pBbyteIndex);
            m_pSLCDatas.reservedSize = read<float>(pSLCByteContent, pBbyteIndex);
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
                for (size_t j = 0; j < verticesNum; j++)
                {
                    pointDatas pPoint;
                    float x = read<float>(pSLCByteContent, pBbyteIndex);
                    float y = read<float>(pSLCByteContent, pBbyteIndex);
                    pPoint.x = x;
                    pPoint.y = y;
                    contour.points.push_back(pPoint);
                }
                contour.points[0] == contour.points[contour.points.size() - 1] ? contour.isClosed = true : contour.isClosed = false;
                contour.isOuterContour = !contour.isClockwise(contour.points);
                contour.isModelContour = p_isModel;
                pLayer.pContours.push_back(move(contour));
                pLayer.existModel = p_isModel;
                pLayer.existSupport = !p_isModel;
            }
            m_pSLCDatas.pLayerDatas.push_back(move(pLayer));
        }
        return m_pSLCDatas;
    }

    printSLCDatas CPrintDatas::getModelAndSupportDatas(const string& p_modelFileName, const string& p_supportFileName)
    {
        printSLCDatas modelDatas = getModelDatas(p_modelFileName, true);
        printSLCDatas supportDatas = getModelDatas(p_supportFileName, false);

        m_pSLCDatas = printSLCDatas();
        m_pSLCDatas.initialHeight  = supportDatas.initialHeight;
        m_pSLCDatas.layerThickness = supportDatas.layerThickness;
        m_pSLCDatas.lineWidth = modelDatas.lineWidth;
        m_pSLCDatas.reservedSize = modelDatas.reservedSize;

        const auto& pModelLayers = modelDatas.pLayerDatas;
        const auto& pSupportLayers = supportDatas.pLayerDatas;
        size_t modelCount   = pModelLayers.size();
        size_t supportCount = pSupportLayers.size();

        constexpr float eps = 1e-6f;    // 用于浮点数比较的容差
        for (size_t i = 0; i < supportCount; ++i)
        {
            float z = pSupportLayers[i].z;
            layerDatas layer;
            layer.existSupport = true;
            layer.existModel = false;
            layer.z = z;

            for (auto& c : pSupportLayers[i].pContours)
            {
                layer.pContours.push_back(c);
            }

            float idxF = (z - modelDatas.initialHeight) / modelDatas.layerThickness;
            int idx  = int(idxF + 0.5f);
            if (idx >= 0 && idx < (int)modelCount && fabs(pModelLayers[idx].z - z) < eps)
            {
                layer.existModel = true;
                for (auto& c : pModelLayers[idx].pContours)
                {
                    layer.pContours.push_back(c);
                }
            }

            m_pSLCDatas.pLayerDatas.push_back(move(layer));
        }

        for (size_t j = supportCount; j < modelCount; ++j)
        {
            layerDatas layer;
            layer.existSupport = false;
            layer.existModel = true;
            layer.z = pModelLayers[j].z;
            for (auto& c : pModelLayers[j].pContours)
            {
                layer.pContours.push_back(c);
            }
            m_pSLCDatas.pLayerDatas.push_back(move(layer));
        }

        return m_pSLCDatas;
    }

    CPrintDatas *_instance = nullptr;
    class CGarboIScanPath
    {
    public:
        ~CGarboIScanPath()
        {
            if (_instance != nullptr)
            {
                delete _instance;
                _instance = nullptr;
            }
        };
    };
    static CGarboIScanPath _garbo;
}

TIGER_PrintDatas::CPrintDatas *printDatas()
{
    if (TIGER_PrintDatas::_instance == nullptr)
    {
        TIGER_PrintDatas::_instance = new TIGER_PrintDatas::CScanPath();
    }
    return TIGER_PrintDatas::_instance;
}