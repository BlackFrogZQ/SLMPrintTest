#include "glPathWidget.h"

namespace TIGER_OpenGL
{
    GLPathWidget::GLPathWidget(QWidget* parent) : QOpenGLWidget(parent), m_currentLayer(0), offsetX(0), offsetY(0), zoom(1.0f)
    {
        setMouseTracking(true);
        setFocusPolicy(Qt::StrongFocus);
        setAttribute(Qt::WA_OpaquePaintEvent);
        setAttribute(Qt::WA_NoSystemBackground);
    }

    void GLPathWidget::setSLCData(const TIGER_PrintDatas::printSLCDatas &data)
    {
        m_slcData = data;
        m_currentLayer = 0;
        update();
    }

    void GLPathWidget::setLayerIndex(int index)
    {
        if (index >= 0 && index < m_slcData.pLayerDatas.size())
        {
            m_currentLayer = index;
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

        if (m_currentLayer < m_slcData.pLayerDatas.size())
        {
            ScanPathRenderer renderer;
            renderer.initializeOpenGLFunctions();
            renderer.drawLayer(m_slcData.pLayerDatas[m_currentLayer]);
        }
    }

    void GLPathWidget::mousePressEvent(QMouseEvent *e)
    {
        if (e->buttons() & Qt::LeftButton)
        {
            m_lastPos = e->pos();
        }
    }

    void GLPathWidget::mouseMoveEvent(QMouseEvent *e)
    {
        if (e->buttons() & Qt::LeftButton)
        {
            QPoint delta = e->pos() - m_lastPos;
            float dx = delta.x() / (width() * 0.5f) * (1.0f / zoom) * 200.0f;
            float dy = -delta.y() / (height() * 0.5f) * (1.0f / zoom) * 200.0f;

            offsetX += dx;
            offsetY += dy;

            m_lastPos = e->pos();
            update();
        }
    }

    void GLPathWidget::wheelEvent(QWheelEvent *e)
    {
        // 获取当前鼠标位置在视口中的坐标
        QPointF pos = e->position();
        float xRatio = (pos.x() / width()) * 2.0f - 1.0f;
        float yRatio = 1.0f - (pos.y() / height()) * 2.0f;

        float viewHalfWidth = 200.0f;
        float aspect = static_cast<float>(height()) / width();
        float viewHalfHeight = viewHalfWidth * aspect;

        float worldX = xRatio * viewHalfWidth / zoom - offsetX;
        float worldY = yRatio * viewHalfHeight / zoom - offsetY;

        // 缩放
        float scaleFactor = 1.0f + e->angleDelta().y() / 1200.0f;
        float oldZoom = zoom;
        zoom *= scaleFactor;
        if (zoom < 0.05f) zoom = 0.05f;

        // 以鼠标为中心调整偏移
        offsetX = offsetX + worldX * (1.0f - oldZoom / zoom);
        offsetY = offsetY + worldY * (1.0f - oldZoom / zoom);

        update();
    }

    void GLPathWidget::mouseDoubleClickEvent(QMouseEvent *event)
    {
        if (event->button() == Qt::LeftButton)
        {
            zoom = 1.0f;
            offsetX = 0.0f;
            offsetY = 0.0f;
            update();
        }
    }
}