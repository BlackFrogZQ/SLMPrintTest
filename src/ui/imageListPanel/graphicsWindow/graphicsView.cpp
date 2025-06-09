#include "graphicsView.h"
#include "tool/toolFun.h"
#include <QGraphicsScene>
#include <QDebug>
#include <QMouseEvent>
namespace TIGER_Graphics
{
    CGraphicsView::CGraphicsView(QGraphicsScene *scene, QWidget *parent)
        : QGraphicsView(scene, parent),
          m_isMoving(false)
    {
        setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
        setDragMode(QGraphicsView::NoDrag);
        setCacheMode(QGraphicsView::CacheBackground);
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setResizeAnchor(QGraphicsView::AnchorUnderMouse);
        setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        setOptimizationFlag(QGraphicsView::DontSavePainterState);
        m_sceneRect = scene->sceneRect();
    }

    CGraphicsView::~CGraphicsView()
    {
    }

    void CGraphicsView::viewScale(const QPoint &p_viewPoint, const double &p_scale)
    {
        QPointF scenePos = this->mapToScene(p_viewPoint);
        QGraphicsView::scale(p_scale, p_scale);
        QPointF scaleScenePos = this->mapToScene(p_viewPoint);
        //调整scene，使得scene和view一直，主要是为了排除掉scroll
        this->setSceneRect(this->mapToScene(this->rect()).boundingRect().toRect());
        //获取缩放前后的坐标差值，即为需要进行move的位移
        viewOffset(scaleScenePos - scenePos);
    }

    void CGraphicsView::viewOffset(const QPointF &p_sceneOffset)
    {
        //调整位置
        this->setSceneRect(this->sceneRect().x() - p_sceneOffset.x(), this->sceneRect().y() - p_sceneOffset.y(),
                           this->sceneRect().width(), this->sceneRect().height());
        this->scene()->update();
    }

    void CGraphicsView::resetView()
    {
        if (m_sceneRect.isEmpty())
        {
            if (scene()->sceneRect().isEmpty())
            {
                return;
            }
            m_sceneRect = scene()->sceneRect();
        }
        QGraphicsView::resetMatrix();
        this->setSceneRect(m_sceneRect);
        auto scale = qMin(this->width() / m_sceneRect.width(), this->height() / m_sceneRect.height())*0.9;
        QGraphicsView::scale(scale, scale);
    }

    void CGraphicsView::wheelEvent(QWheelEvent *p_event)
    {
        if (!m_isMoving)
        {
            constexpr double cScale = 0.9;
            constexpr double cScaleZoom = 2 - cScale;
            double scale = (p_event->delta() > 0 ? cScale : cScaleZoom);
            viewScale(p_event->pos(), scale);
        }
        // QGraphicsView::wheelEvent(p_event);
    }

    void CGraphicsView::mousePressEvent(QMouseEvent *p_event)
    {
        if ((p_event->button() & Qt::MidButton))
        {
            TIGER_GraphicsTool::setOverrideCursor(Qt::ClosedHandCursor);
            m_isMoving = true;
            m_lastPos = p_event->pos();
            return;
        }
        QGraphicsView::mousePressEvent(p_event);
    }

    void CGraphicsView::mouseReleaseEvent(QMouseEvent *p_event)
    {
        if (p_event->button() & Qt::MiddleButton)
        {
            TIGER_GraphicsTool::restoreOverrideCursor();
            m_isMoving = false;
            return;
        }
        QGraphicsView::mouseReleaseEvent(p_event);
    }

    void CGraphicsView::mouseMoveEvent(QMouseEvent *p_event)
    {
        QPoint movPos = p_event->pos();
        if (movPos.x() < 0 || movPos.x() >= this->width() || movPos.y() < 0 || movPos.y() >= this->height())
        {
            return;
        }
        if ((p_event->buttons() & Qt::MiddleButton) && m_isMoving)
        {
            QPointF dis = this->mapToScene(movPos) - this->mapToScene(m_lastPos);
            m_lastPos = movPos;
            viewOffset(dis);
        }
        QGraphicsView::mouseMoveEvent(p_event);
    }

    void CGraphicsView::resizeEvent(QResizeEvent *p_event)
    {
        QGraphicsView::resizeEvent(p_event);
        resetView();
    }

    void CGraphicsView::mouseDoubleClickEvent(QMouseEvent *p_event)
    {
        QGraphicsView::mouseDoubleClickEvent(p_event);
        if ((p_event->buttons() & Qt::MiddleButton) && (!m_isMoving))
        {
            resetView();
        }
    }
};
