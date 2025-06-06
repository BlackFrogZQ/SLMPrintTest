#pragma once
#include "vm/vm.h"
#include "vm/action/iActionState.h"

namespace TIGER_VMSLM
{
    class CAutoWorkOnceAction;

    class CAutoWorkOnceIdleState : public IActionState
    {
        Q_OBJECT
    public:
        CAutoWorkOnceIdleState(CAutoWorkOnceAction* p_action);
        ~CAutoWorkOnceIdleState();
        virtual void run() override;

    protected:
        CAutoWorkOnceAction* m_action;
        IVM* m_pVM;
    };

    class CStartAutoSpreadOnce : public CAutoWorkOnceIdleState
    {
        Q_OBJECT
    public:
        using CAutoWorkOnceIdleState::CAutoWorkOnceIdleState;
        virtual void run() override;
        virtual void runing() override;
    };

    class CStartAutoMarkOnce : public CAutoWorkOnceIdleState
    {
        Q_OBJECT
    public:
        using CAutoWorkOnceIdleState::CAutoWorkOnceIdleState;
        virtual void run() override;
        virtual void runing() override;
    };
}