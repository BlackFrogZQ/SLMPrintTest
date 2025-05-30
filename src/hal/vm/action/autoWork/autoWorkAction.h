#pragma once
#include "../iAction.h"
#include "../iActionState.h"

namespace TIGER_VMSLM
{
    class CAutoWorkAction : public IAction
    {
        friend class CAutoWorkIdleState;
        friend class CStartSpread;
        friend class CStartMark;

        Q_OBJECT
    public:
        CAutoWorkAction(CVM *p_vm);
        ~CAutoWorkAction();

        virtual void start() override;
        virtual void stop() override;

    protected:
        IActionState* m_idle;
        IActionState* m_startSpread;
        IActionState* m_startMark;
    };
}