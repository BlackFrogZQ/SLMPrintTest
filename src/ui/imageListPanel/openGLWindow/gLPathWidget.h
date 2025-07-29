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
		QPointF getPosition(QPoint p_point);
		void setMove(QPoint p_point);

    protected:
        void initializeGL() override;
        void resizeGL(int w, int h) override;
        void paintGL() override;
        void wheelEvent(QWheelEvent *) override;
        void mousePressEvent(QMouseEvent *) override;
        void mouseMoveEvent(QMouseEvent *) override;
        void mouseDoubleClickEvent(QMouseEvent *event) override;

    private:
        TIGER_PrintDatas::printSLCDatas m_slcData;
        int m_currentLayer;

		QPointF m_lastMove;
		QPointF m_viewMove;
		double m_times;
    };
}