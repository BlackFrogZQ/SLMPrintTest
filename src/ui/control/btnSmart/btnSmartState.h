#pragma once
#include <QEvent>

namespace TIGER_UI_BtnSmart
{
    class CBtnSmart;

    class IState
    {
    public:
        IState(CBtnSmart* p_btnSmart);
        virtual ~IState(){};
        virtual void click(){};
        virtual void setCheck(){};
        virtual void setNormal(){};
        virtual void setDisable(){};
        virtual void enterEvent(QEvent *e){};
        virtual void leaveEvent(QEvent *e){};
    protected:
        CBtnSmart* m_pBtnSmart;
    };

    class CNormalState : public IState
    {
    public:
        using IState::IState;
        void click();
        void setCheck();
        void setDisable();
        void enterEvent(QEvent *e);
        void leaveEvent(QEvent *e);
    };

    class CDisableState : public IState
    {
    public:
        using IState::IState;
        void setCheck();
        void setNormal();
    };

    class CCheckState : public IState
    {
    public:
        using IState::IState;
        void click();
        void setNormal();
        void setDisable();
    };
}