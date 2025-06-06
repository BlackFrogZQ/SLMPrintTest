#pragma once
#include "../iAction.h"
#include "../iActionState.h"

namespace TIGER_VMSLM
{
    class CSpreadAction : public IAction
    {
        friend class CSpreadIdleState;
        friend class CStartSpread;

        Q_OBJECT
    public:
        CSpreadAction(CVM *p_vm);
        ~CSpreadAction();

        virtual void start() override;
        virtual void stop() override;

    protected:
        IActionState* m_idle;
        IActionState* m_startSpread;
        // int runCount; // 记录铺粉层数
    };
}