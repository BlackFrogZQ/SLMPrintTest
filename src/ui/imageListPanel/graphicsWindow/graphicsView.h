#pragma once
#include <QGraphicsView>
// 中键按住视图移动、双击复原
namespace TIGER_Graphics
{
    class CGraphicsView : public QGraphicsView
    {
        Q_OBJECT
    public:
        CGraphicsView(QGraphicsScene *scene, QWidget *parent = nullptr);
        ~CGraphicsView();
        void resetView();
        void viewScale(const QPoint &p_viewPoint, const double &p_scale);

    protected:
        void viewOffset(const QPointF &p_sceneOffset);

    protected:
        // button返回"发生此事件的那个按钮"，buttons返回"发生此事件时处于按下状态的那些按钮"
        virtual void wheelEvent(QWheelEvent *p_event) override;
        virtual void mousePressEvent(QMouseEvent *p_event) override;
        virtual void mouseReleaseEvent(QMouseEvent *p_event) override;
        virtual void mouseMoveEvent(QMouseEvent *p_event) override;
        virtual void resizeEvent(QResizeEvent *p_event) override;
        virtual void mouseDoubleClickEvent(QMouseEvent *p_event) override;

    protected:
        QPoint m_lastPos;
        bool m_isMoving;
        //场景最初大小
        QRectF m_sceneRect;
    };
};
