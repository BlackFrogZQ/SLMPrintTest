#include "graphicsScene.h"
#include "../imageListDef.h"
#include <QImage>
#include <QGraphicsPixmapItem>
#include <QToolTip>
#include <QPainter>

namespace TIGER_Graphics
{
    CGraphicsScene::CGraphicsScene(QObject *parent)
        : QGraphicsScene(parent),
          m_pImageItem(nullptr),
          m_platformSize(1920, 1080),
          m_platformColor(Qt::cyan),
          m_fillColor(Qt::white)
    {
        m_pImageItem = new QGraphicsPixmapItem;
        m_pImageItem->setFlags(m_pImageItem->flags() & ~QGraphicsItem::ItemIsSelectable);
        this->addItem(m_pImageItem);
    }

    CGraphicsScene::~CGraphicsScene()
    {
        QGraphicsScene::clear();
    }

    void CGraphicsScene::setBackgroundPlatform(QSizeF p_platformSize, QColor p_platformColor, QColor p_fillColor)
    {
        assert(p_platformSize.isValid());
        m_platformSize = p_platformSize;
        m_platformColor = p_platformColor;
        m_fillColor = p_fillColor;
        const double dx = m_platformSize.width() * 0.5;
        const double dy = m_platformSize.height() * 0.5;
        setSceneRect(-dx, -dy, m_platformSize.width(), m_platformSize.height());
    }

    void CGraphicsScene::clearImage()
    {
        m_pImageItem->setPixmap(QPixmap());
        m_pImageItem->hide();
    }

    void CGraphicsScene::showImage(const QPixmap &p_pImage)
    {
        m_pImageItem->setPixmap(showTransform(p_pImage));
        if (p_pImage.isNull())
        {
            m_pImageItem->hide();
        }
        else
        {
            const double dx = -p_pImage.width() * 0.5;
            const double dy = -p_pImage.height() * 0.5;
            m_pImageItem->setPos(dx,dy);
            m_pImageItem->show();
        }
    }

    QPixmap CGraphicsScene::getImage() const
    {
        return m_pImageItem->pixmap();
    }

    void CGraphicsScene::drawBackground(QPainter *painter, const QRectF &rect)
    {
        painter->setRenderHint(QPainter::SmoothPixmapTransform, true);
        painter->fillRect(rect, m_fillColor);
        if (m_platformSize.isValid())
        {
            const double dx = m_platformSize.width() * 0.5;
            const double dy = m_platformSize.height() * 0.5;
            painter->setBrush(m_platformColor);
            painter->setPen(m_platformColor);
            painter->drawRect(-dx, -dy, m_platformSize.width(), m_platformSize.height());
        }
    }
};