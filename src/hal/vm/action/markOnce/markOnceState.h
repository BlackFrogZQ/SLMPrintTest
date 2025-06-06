#pragma once
#include "../iActionState.h"
#include "../../vm.h"

namespace TIGER_VMSLM
{
    class CMarkOnceAction;

    class CMarkOnceIdleState : public IActionState
    {
        Q_OBJECT
    public:
        CMarkOnceIdleState(CMarkOnceAction* p_action);
        ~CMarkOnceIdleState();
        virtual void run() override;

    protected:
        CMarkOnceAction* m_action;
        IVM* m_pVM;
    };

    class CDownloadMarkFile : public CMarkOnceIdleState
    {
        Q_OBJECT
    public:
        using CMarkOnceIdleState::CMarkOnceIdleState;
        virtual void run() override;
        virtual void runing() override;
    };

    class CStartMark : public CMarkOnceIdleState
    {
        Q_OBJECT
    public:
        using CMarkOnceIdleState::CMarkOnceIdleState;
        virtual void run() override;
        virtual void runing() override;
    };
}