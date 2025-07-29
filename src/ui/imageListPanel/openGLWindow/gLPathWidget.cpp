#include "glPathWidget.h"

namespace TIGER_OpenGL
{
    GLPathWidget::GLPathWidget(QWidget* parent) : QOpenGLWidget(parent), m_currentLayer(0), m_times(1.0)
    {
        setMouseTracking(true);
        setFocusPolicy(Qt::StrongFocus);
        setAttribute(Qt::WA_OpaquePaintEvent);
        setAttribute(Qt::WA_NoSystemBackground);
        setFixedSize(960, 540);
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

    void GLPathWidget::resizeGL(int width, int height)
    {
		if (0 == height)
		{
			height = 1;
		}
		glViewport(0, 0, width, height);
    }

    void GLPathWidget::paintGL()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		int max = 5 * qMax(width(), height());
		float w = this->width() / m_times / 2;
		float h = this->height() / m_times / 2;
		glOrtho(-w - m_viewMove.x(), w - m_viewMove.x(), -h - m_viewMove.y(), h - m_viewMove.y(), -max, max);

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

	void GLPathWidget::setMove(QPoint p_point)
	{
		QPointF point = getPosition(p_point);
		m_viewMove += point - m_lastMove;
		m_lastMove = point;
	}

	QPointF GLPathWidget::getPosition(QPoint p_point)
	{
		float x = (p_point.x() - this->width() * 0.5) / m_times;
		float y = -(p_point.y() - this->height() * 0.5) / m_times;
		return QPointF(x, y);
	}

	void GLPathWidget::wheelEvent(QWheelEvent *event)
	{
		if (event->delta() > 0)
		{
			m_times *= 1.1;
		}
		else
		{
			m_times /= 1.1;
		}
		update();
	}

	void GLPathWidget::mousePressEvent(QMouseEvent *event)
	{
		if (event->button() == Qt::LeftButton)
		{
			QPoint pos = event->pos();
			m_lastMove = getPosition(pos);
		}
	}

	void GLPathWidget::mouseMoveEvent(QMouseEvent *event)
	{
		if (event->x() >= 0.99*width() || event->x() <= 0.01*width() || event->y() <= 0.01*height() || event->y() >= 0.99*height())
		{
			return;
		}
		if (event->buttons() == Qt::LeftButton)
		{
			setMove(event->pos());
			update();
		}
	}

    void GLPathWidget::mouseDoubleClickEvent(QMouseEvent *event)
    {
        if (event->button() == Qt::LeftButton)
        {
            m_times = 1.0;
            m_viewMove = QPointF(0.0, 0.0);
            update();
        }
    }
}