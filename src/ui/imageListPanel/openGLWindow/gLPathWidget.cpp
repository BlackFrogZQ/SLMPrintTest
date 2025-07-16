#include "glPathWidget.h"

namespace TIGER_OpenGL
{
    GLPathWidget::GLPathWidget(QWidget* parent) : QOpenGLWidget(parent)
    {
    }

    void GLPathWidget::setSLCData(const TIGER_PrintDatas::printSLCDatas &data)
    {
        m_slcData = data;
        currentLayer = 0;
        update();
    }

    void GLPathWidget::setLayerIndex(int index)
    {
        if (index >= 0 && index < m_slcData.pLayerDatas.size())
        {
            currentLayer = index;
            update();
        }
    }

    void GLPathWidget::initializeGL()
    {
        initializeOpenGLFunctions();
        glShadeModel(GL_SMOOTH);
        glClearColor(0.898f, 0.898f, 0.898f, 1.0f);
        glClearDepth(1.0f);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_LINE_SMOOTH);
    }

    void GLPathWidget::resizeGL(int w, int h)
    {
        glViewport(0, 0, w, h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        float halfWidth = 100.0f;
        float aspect = static_cast<float>(h) / w;
        glOrtho(-halfWidth, halfWidth, -halfWidth * aspect, halfWidth * aspect, -1, 1);

        glMatrixMode(GL_MODELVIEW);
    }

    void GLPathWidget::paintGL()
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();
        glScalef(zoom, zoom, 1);
        glTranslatef(offsetX, offsetY, 0);

        float printRegionSize = 180.0f;
        float half = printRegionSize / 2.0f;

        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_QUADS);
        glVertex2f(-half, -half);
        glVertex2f(half, -half);
        glVertex2f(half, half);
        glVertex2f(-half, half);
        glEnd();

        if (currentLayer < m_slcData.pLayerDatas.size())
        {
            ScanPathRenderer renderer;
            renderer.initializeOpenGLFunctions();
            renderer.drawLayer(m_slcData.pLayerDatas[currentLayer]);
        }
    }

    void GLPathWidget::mousePressEvent(QMouseEvent *e)
    {
        lastPos = e->pos();
    }

    void GLPathWidget::mouseMoveEvent(QMouseEvent *e)
    {
        if (e->buttons() & Qt::LeftButton)
        {
            QPoint delta = e->pos() - lastPos;
            offsetX += delta.x() / 100.0f;
            offsetY -= delta.y() / 100.0f;
            lastPos = e->pos();
            update();
        }
    }

    void GLPathWidget::wheelEvent(QWheelEvent *e)
    {
        zoom += e->angleDelta().y() / 1200.0f;
        if (zoom < 0.05f) zoom = 0.05f;
        update();
    }
}