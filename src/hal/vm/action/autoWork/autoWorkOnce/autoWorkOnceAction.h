
#pragma once
#include "vm/action/iAction.h"
#include "vm/action/iActionState.h"

namespace TIGER_VMSLM
{
    class CAutoWorkOnceAction : public IAction
    {
        friend class CAutoWorkOnceIdleState;
        friend class CStartAutoSpreadOnce; 
        friend class CStartAutoMarkOnce;

        Q_OBJECT
    public:
        CAutoWorkOnceAction(CVM *p_vm);
        ~CAutoWorkOnceAction();

        virtual void start() override;
        virtual void stop() override;

    protected:
        IActionState* m_idle;
        IActionState* m_startSpreadOnce;
        IActionState* m_startMarkOnce;
    };
}