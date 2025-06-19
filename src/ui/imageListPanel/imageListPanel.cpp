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

using namespace TIGER_VMSLM;

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
        connect(m_pListWidget, &QListWidget::currentRowChanged, this, &CImageListPanel::slotIdChanged);
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
        setImageIconSize(QSize(100, 100));
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
}