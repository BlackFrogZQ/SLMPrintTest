#include "imageListPanel.h"
#include "imageItem.h"
#include "graphicsWindow/graphicsView.h"
#include "graphicsWindow/graphicsScene.h"
#include "hal/vm/ncDef.h"
#include "hal/vm/vm.h"
#include "hal/vm/manuDef.h"
#include "system/tool/fileTool.h"
#include <QListWidget>
#include <QScrollBar>
#include <QMenu>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QPainterPath>

using namespace TIGER_VMSLM;
using namespace TIGER_PrintDatas;
using namespace std;
namespace TIGER_UI_SLM
{
    CImageListPanel::CImageListPanel(QWidget *parent, CVM *p_pVM)
        : QWidget(parent),
          m_pListWidget(nullptr),
          m_currentLayer(TIGER_SLMManuDef::manuStatus()->currentLayer),
          m_pVM(p_pVM)
    {
        assert(p_pVM != nullptr);
        initListWidget();
        initGraphicsView();
        initLayout();
        // connect(m_pListWidget, &QListWidget::currentRowChanged, this, &CImageListPanel::slotIdChanged);
        connect(m_pListWidget, &QListWidget::currentRowChanged, this, &CImageListPanel::slotIdChangedSLC);
        connect(m_pVM, &CVM::sigSliceUpdate, this, &CImageListPanel::slotSliceUpdate);
        initContextMenu();
    }

    CImageListPanel::~CImageListPanel()
    {
    }

    void CImageListPanel::initListWidget()
    {
        m_pListWidget = new QListWidget;
        m_pListWidget->setMouseTracking(true);
        m_pListWidget->setStyleSheet("background:rgb(240, 240, 240);");
        m_pListWidget->setResizeMode(QListView::Adjust);
        m_pListWidget->setItemAlignment(Qt::AlignCenter);
        m_pListWidget->setViewMode(QListView::IconMode);
        m_pListWidget->setMovement(QListView::Static);
        m_pListWidget->setLayoutMode(QListView::Batched);
        m_pListWidget->setUniformItemSizes(true);
        m_pListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setImageIconSize(listWidgetSize);
    }

    void CImageListPanel::initGraphicsView()
    {
        m_pScene = new TIGER_Graphics::CGraphicsScene(this);
        m_pView = new TIGER_Graphics::CGraphicsView(m_pScene);
        setPlatformSize(QSizeF(960, 540));
    }

    void CImageListPanel::initLayout()
    {
        QHBoxLayout *pLayout = new QHBoxLayout;
        pLayout->addWidget(m_pListWidget);
        pLayout->addWidget(m_pView);
        pLayout->setSpacing(2);
        pLayout->setMargin(0);
        this->setLayout(pLayout);
    }

    void CImageListPanel::setImageIconSize(QSize p_size)
    {
        m_pListWidget->clearPropertyFlags();
        m_pListWidget->setIconSize(p_size);
        m_pListWidget->setGridSize(QSize(p_size.width() + 10, p_size.height() + 30));
        m_pListWidget->setFixedWidth(m_pListWidget->gridSize().width() + m_pListWidget->verticalScrollBar()->sizeHint().width());
    }

    void CImageListPanel::setPlatformSize(QSizeF p_size)
    {
        m_pScene->setBackgroundPlatform(p_size, Qt::cyan, QColor(240, 240, 240));
    }

    void CImageListPanel::clear()
    {
        m_pListWidget->clear();
    }

    void CImageListPanel::scanFolder(QString p_folder)
    {
        QStringList imageNames = TIGER_FlieTool::scanfDir(
            p_folder,
            QStringList() << "*.tif",
            QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot,
            QDir::Name);
        setImages(imageNames);
    }

    void CImageListPanel::setImages(const QStringList &p_imageNames)
    {
        clear();
        for (const auto &file : p_imageNames)
        {
            m_pListWidget->addItem(new CImageListItem(file, m_pListWidget->iconSize()));
        }
        TIGER_SLMManuDef::manuStatus()->printImages = p_imageNames;
        m_currentLayer = 0;
        if (p_imageNames.size() > 0)
        {
            setCurrentImageId(0);
        }
        sigDataChanged(p_imageNames.size());
    }

    void CImageListPanel::setCurrentImageId(int id)
    {
        assert(id >= 0 && id < imageCount());
        m_pListWidget->setCurrentRow(id);
    }

    int CImageListPanel::imageCount() const
    {
        return m_pListWidget->count();
    }

    int CImageListPanel::currentImageId() const
    {
        return m_pListWidget->currentRow();
    }


    void CImageListPanel::setImageItemState(int id, CImageItemState state)
    {
        assert(id >= 0 && id < imageCount());
        (static_cast<CImageListItem *>(m_pListWidget->item(id)))->setState(state);
    }

    QString CImageListPanel::getImageName(int id) const
    {
        assert(id >= 0 && id < imageCount());
        return (static_cast<CImageListItem *>(m_pListWidget->item(id)))->getImageName();
    }

    CImageItemState CImageListPanel::getImageState(int id) const
    {
        assert(id >= 0 && id < imageCount());
        return (static_cast<CImageListItem *>(m_pListWidget->item(id)))->getState();
    }

    void CImageListPanel::slotSliceUpdate()
    {
        if (m_currentLayer < 0 || m_currentLayer >= imageCount())
        {
            return;
        }
        if (m_currentLayer == 0)
        {
            setImageItemState(m_currentLayer, CImageItemState::cisPrinting);
            for (size_t i = 1; i < imageCount(); i++)
            {
                setImageItemState(i, CImageItemState::cisUnPrint);
            }
        }
        else if (getImageState(m_currentLayer - 1) == CImageItemState::cisPrinting && getImageState(m_currentLayer) == CImageItemState::cisUnPrint)
        {
            setImageItemState(m_currentLayer - 1, CImageItemState::cisFinished);
            setImageItemState(m_currentLayer, CImageItemState::cisPrinting);
        }
        else if ((m_currentLayer == imageCount() - 1) && getImageState(m_currentLayer) == CImageItemState::cisPrinting)
        {
            setImageItemState(m_currentLayer, CImageItemState::cisFinished);
        }
        setCurrentImageId(m_currentLayer);
    }

    void CImageListPanel::slotIdChanged(int id)
    {
        if (id >= 0 && id < imageCount())
        {
            m_pScene->showImage(QPixmap((static_cast<CImageListItem *>(m_pListWidget->item(id)))->getImageName()));
            m_pView->resetView();
        }
        else
        {
            m_pScene->clearImage();
        }
        emit currentIdChanged(id);
    }

    void CImageListPanel::initContextMenu()
    {
        m_pContextMenu = new QMenu;
        auto pAction = new QAction(cnStr("打开文件夹"), m_pContextMenu);
        connect(pAction, &QAction::triggered, this, [this]
                {
                    QString folder = QFileDialog::getExistingDirectory(this, cnStr("打开图片文件夹"));
                    if (!folder.isEmpty())
                    {
                        scanFolder(folder);
                    }
                });
        m_pContextMenu->addAction(pAction);

        pAction = new QAction(cnStr("打开测试图片"), m_pContextMenu);
        connect(pAction, &QAction::triggered, this, [this]
                {
                    QString file = QFileDialog::getOpenFileName(this, cnStr("打开测试图片"),QString(),cnStr("测试文件(*.bmp *.jpg *.png *.jpeg *.tif)"));
                    if (!file.isEmpty())
                    {
                        setImages(QStringList() << file);
                        TIGER_SLMManuDef::manuStatus()->updateLayerStatus();
                    }
                });

        m_pContextMenu->addAction(pAction);
        pAction = new QAction(cnStr("清空"), m_pContextMenu);
        connect(pAction, &QAction::triggered, this, [this]{ setImages(QStringList()); });
        m_pContextMenu->addAction(pAction);

        pAction = new QAction(cnStr("转到正在打印层"), m_pContextMenu);
        connect(pAction, &QAction::triggered, this, [this]{ setCurrentImageId(m_currentLayer); });
        m_pContextMenu->addAction(pAction);

        pAction = new QAction(cnStr("打开SLC文件"), m_pContextMenu);
        connect(pAction, &QAction::triggered, this, [this]
            {
                QString file = QFileDialog::getOpenFileName(this, cnStr("打开SLC文件"), QString(), cnStr("SLC文件(*.slc)"));
                if (!file.isEmpty())
                {
                    auto pAllLayers = TIGER_SLMManuDef::manuStatus()->getSLCPrintDatas(file.toStdString());
                    setListWidgetShow(pAllLayers);
                }
            });
        m_pContextMenu->addAction(pAction);
    }

    void CImageListPanel::contextMenuEvent(QContextMenuEvent *event)
    {
        // if (m_pVM->ncStatus().isConnected)
        // {
        //     if (m_pVM->ncStatus().vmStatus == TIGER_NCDef::CVMStatus::vmsIdle)
        //     {
                m_pContextMenu->exec(QCursor::pos());
        //     }
        // }
    }

    void CImageListPanel::setListWidgetShow(const vector<Layer>& layers)
    {
        m_pListWidget->clear();
        for (size_t i = 0; i < layers.size(); ++i)
        {
            const Layer& layer = layers[i];
            QImage img = renderLayer(layer);

            QPixmap pix = QPixmap::fromImage(img);
            QListWidgetItem* item = new QListWidgetItem(QIcon(pix), QString(cnStr("第%1层 (Z=%2)")).arg(i+1).arg(layer.z));
            m_pListWidget->addItem(item);
        }
        m_currentLayer = 0;
        setCurrentImageId(0);
    }

    QImage CImageListPanel::renderLayer(const Layer& layer)
    {
        QImage image(listWidgetSize, QImage::Format_ARGB32_Premultiplied);
        image.fill(Qt::cyan);

        QPainter painter(&image);
        painter.setRenderHint(QPainter::Antialiasing);

        float minX=1e10, minY=1e10, maxX=-1e10, maxY=-1e10;
        for (auto& contour : layer.pContours)
        {
            for (auto& p : contour)
            {
                minX = std::min(minX, p.x);
                minY = std::min(minY, p.y);
                maxX = std::max(maxX, p.x);
                maxY = std::max(maxY, p.y);
            }
        }
        float dx = maxX - minX, dy = maxY - minY;
        float scale = std::min(listWidgetSize.width()/dx, listWidgetSize.height()/dy) * 0.9f;
        float xOff = (listWidgetSize.width()  - dx*scale)/2 - minX*scale;
        float yOff = (listWidgetSize.height() - dy*scale)/2 - minY*scale;

        QPainterPath path;
        for (auto& contour : layer.pContours)
        {
            size_t pointNum = contour.size();
            if(pointNum >= 3)
            {
                QPolygonF poly;
                for (auto& p : contour)
                {
                    QPointF pt(p.x*scale + xOff, p.y*scale + yOff);
                    poly << pt;
                }
                path.addPolygon(poly);
            }
            else if (pointNum == 2)
            {
                QPointF pt1(contour[0].x*scale + xOff, contour[0].y*scale + yOff);
                QPointF pt2(contour[1].x*scale + xOff, contour[1].y*scale + yOff);
                path.moveTo(pt1);
                path.lineTo(pt2);
            }
            else if (pointNum == 1)
            {
                QPointF pt(contour[0].x*scale + xOff, contour[0].y*scale + yOff);
                constexpr qreal pRadius = 2.0;
                path.addEllipse(pt, pRadius, pRadius);
            }
        }
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::black);
        painter.drawPath(path);

        QPen pen(Qt::blue);
        pen.setWidthF(qMax(1.0f, layer.lineWidth * scale * 0.5f));
        painter.setPen(pen);
        painter.setBrush(Qt::NoBrush);
        painter.drawPath(path);

        return image;
    }

    void CImageListPanel::slotIdChangedSLC(int id)
    {
        if (id < 0 || id >= m_pListWidget->count()) return;
        auto *item = static_cast<CImageListItem*>( m_pListWidget->item(id) );
        QIcon icon = item->icon();
        QPixmap pix = icon.pixmap(m_pListWidget->iconSize());
        m_pScene->showImage(pix);
        m_pView->resetView();
    }
}