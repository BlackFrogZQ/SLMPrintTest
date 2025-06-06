#pragma once
#include "../iActionState.h"
#include "../../vm.h"

namespace TIGER_VMSLM
{
    class CSpreadAction;

    class CSpreadIdleState : public IActionState
    {
        Q_OBJECT
    public:
        CSpreadIdleState(CSpreadAction* p_action);
        ~CSpreadIdleState();
        virtual void run() override;

    protected:
        CSpreadAction* m_action;
        IVM* m_pVM;
    };

    class CStartSpread : public CSpreadIdleState
    {
        Q_OBJECT
    public:
        using CSpreadIdleState::CSpreadIdleState;
        virtual void run() override;
        virtual void runing() override;
    private:
        bool m_isSpreading = false; // 是否正在铺粉
    };
}