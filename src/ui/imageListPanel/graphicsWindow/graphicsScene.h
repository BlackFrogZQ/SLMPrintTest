#pragma once
#include <QGraphicsScene>
#include <QGraphicsScene>
class QPixmap;
class QGraphicsPixmapItem;
namespace TIGER_Graphics
{
    class CGraphicsScene : public QGraphicsScene
    {
        Q_OBJECT

    public:
        CGraphicsScene(QObject *parent = nullptr);
        ~CGraphicsScene();
        void setBackgroundPlatform(QSizeF p_platformSize, QColor p_platformColor = Qt::cyan, QColor p_fillColor = Qt::white);
        void clearImage();
        void showImage(const QPixmap &p_pImage);
        QPixmap getImage() const;

    protected:
        virtual void drawBackground(QPainter *painter, const QRectF &rect) override;

    protected:
        QGraphicsPixmapItem *m_pImageItem;
        QColor m_fillColor;
        QColor m_platformColor;
        QSizeF m_platformSize;
    };
};