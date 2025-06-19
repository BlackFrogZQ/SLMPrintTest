#pragma once
#include <QPushButton>
#include <QSize>
#include <QPixmap>
#include <QEvent>

namespace TIGER_UI_BtnSmart
{
    const QSize cButtonSize(57, 57);

    class IState;
    class CNormalState;
    class CDisableState;
    class CCheckState;

    class CBtnSmart : public QPushButton
    {
        Q_OBJECT
        friend class IState;
        friend class CNormalState;
        friend class CDisableState;
        friend class CCheckState;

    public:
        CBtnSmart(const QString &p_fileName, QSize p_buttonSize = cButtonSize);
        ~CBtnSmart();

        enum CSetState
        {
            ssNormal,
            ssDisable,
            ssCheck,
            ssUnknown
        };
        void setState(CSetState p_state);
        void setNormal();
        void setDisable();
        void setCheck();
        bool isNormal();
        bool isDisable();
        bool isCheck();

    public slots:
        void click();

    signals:
        void sigClick();
        void sigCheckClick();

    protected:
        void setNormalState();
        void setDisableState();
        void setCheckState();
        void setBitmap(QPixmap *p_pBitmap);
        void enterEvent(QEvent *e); // 鼠标进入事件
        void leaveEvent(QEvent *e); // 鼠标离开事件
        virtual void paintEvent(QPaintEvent *) override;

    protected:
        QSize m_buttonSize;
        QPixmap *m_pNormalBitmap;
        QPixmap *m_pFocusBitmap;
        QPixmap *m_pCheckBitmap;
        QPixmap *m_pDisableBitmap;
        QPixmap *m_pCurrentBitmap;

        IState *m_pState;
        IState *m_pNormalState;
        IState *m_pDisableState;
        IState *m_pCheckState;
    };
}