#pragma once
#include "../iActionState.h"
#include "../../vm.h"

namespace TIGER_VMSLM
{
    class CMarkAction;

    class CMarkIdleState : public IActionState
    {
        Q_OBJECT
    public:
        CMarkIdleState(CMarkAction* p_action);
        ~CMarkIdleState();
        virtual void run() override;

    protected:
        CMarkAction* m_action;
        IVM* m_pVM;
    };

    class CDownloadMarkFile : public CMarkIdleState
    {
        Q_OBJECT
    public:
        using CMarkIdleState::CMarkIdleState;
        virtual void run() override;
        virtual void runing() override;
    };

    class CStartMark : public CMarkIdleState
    {
        Q_OBJECT
    public:
        using CMarkIdleState::CMarkIdleState;
        virtual void run() override;
        virtual void runing() override;
    };
}