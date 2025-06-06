
#pragma once
#include "vm/action/iAction.h"
#include "vm/action/iActionState.h"

namespace TIGER_VMSLM
{
    class CManuOnceAction : public IAction
    {
        friend class CManuOnceIdleState;
        friend class CManuOnceSpread; 
        friend class CManuOnceMark;

        Q_OBJECT
    public:
        CManuOnceAction(CVM *p_vm);
        ~CManuOnceAction();

        virtual void start() override;
        virtual void stop() override;

    protected:
        IActionState* m_idle;
        IActionState* m_startSpreadOnce;
        IActionState* m_startMarkOnce;
    };
}