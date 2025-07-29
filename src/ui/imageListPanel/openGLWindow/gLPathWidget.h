#pragma once
#include "scanPathRenderer.h"
#include "hal/printDatas/printDatasDef.h"
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMouseEvent>
#include <QWheelEvent>

namespace TIGER_OpenGL
{
    class GLPathWidget : public QOpenGLWidget, protected QOpenGLFunctions
    {
        Q_OBJECT
    public:
        explicit GLPathWidget(QWidget* parent = nullptr);
        void setSLCData(const TIGER_PrintDatas::printSLCDatas &data);
        void setLayerIndex(int index);

    protected:
        void initializeGL() override;
        void resizeGL(int w, int h) override;
        void paintGL() override;
        void mousePressEvent(QMouseEvent *) override;
        void mouseMoveEvent(QMouseEvent *) override;
        void mouseDoubleClickEvent(QMouseEvent *event) override;
        void wheelEvent(QWheelEvent *) override;

    private:
        TIGER_PrintDatas::printSLCDatas m_slcData;
        int m_currentLayer;
        float offsetX, offsetY;
        float zoom;
        QPoint m_lastPos;
    };
}