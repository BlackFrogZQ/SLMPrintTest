#pragma once
#include <QWidget>
#include "imageListDef.h"
namespace TIGER_Graphics
{
    class CGraphicsScene;
    class CGraphicsView;
};
namespace TIGER_VMSLM
{
    class CVM;
};
class QListWidget;
class QMenu;
namespace TIGER_UI_SLM
{
    class CImageListPanel : public QWidget
    {
        Q_OBJECT
    public:
        CImageListPanel(QWidget *parent = nullptr, TIGER_VMSLM::CVM *p_pVM = nullptr);
        ~CImageListPanel();

        void setImageIconSize(QSize p_size);
        void setPlatformSize(QSizeF p_size);
        void clear();
        void scanFolder(QString p_folder);
        void setImages(const QStringList &p_imageNames);
        void setCurrentImageId(int id);
        int imageCount() const;
        int currentImageId() const;

        void setImageItemState(int id, CImageItemState state);
        QString getImageName(int id) const;
        CImageItemState getImageState(int id) const;

    signals:
        void currentIdChanged(int id);
        void sigDataChanged(int dataCount);

    public slots:
        void slotSliceUpdate();

    protected slots:
        void slotIdChanged(int id);

    protected:
        void initListWidget();
        void initGraphicsView();
        void initLayout();
        void initContextMenu();
        virtual void contextMenuEvent(QContextMenuEvent *event) override;

    protected:
        TIGER_VMSLM::CVM *m_pVM;
        QListWidget *m_pListWidget;
        TIGER_Graphics::CGraphicsScene *m_pScene;
        TIGER_Graphics::CGraphicsView *m_pView;
        int& m_currentLayer;
        QMenu* m_pContextMenu;
    };
}