#pragma once
#include "../iActionState.h"
#include "../../vm.h"

namespace TIGER_VMSLM
{
    class CAutoWorkAction;

    class CAutoWorkIdleState : public IActionState
    {
        Q_OBJECT
    public:
        CAutoWorkIdleState(CAutoWorkAction* p_action);
        ~CAutoWorkIdleState();
        virtual void run() override;

    protected:
        CAutoWorkAction* m_action;
        IVM* m_pVM;
    };

    class CStartSpread : public CAutoWorkIdleState
    {
        Q_OBJECT
    public:
        using CAutoWorkIdleState::CAutoWorkIdleState;
        virtual void run() override;
        virtual void runing() override;
    };

    class CStartMark : public CAutoWorkIdleState
    {
        Q_OBJECT
    public:
        using CAutoWorkIdleState::CAutoWorkIdleState;
        virtual void run() override;
        virtual void runing() override;
    };
}