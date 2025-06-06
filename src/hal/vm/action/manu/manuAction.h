#pragma once
#include "../iAction.h"
#include "../iActionState.h"

namespace TIGER_VMSLM
{
    class CManuAction : public IAction
    {
        friend class CManuIdleState;
        friend class CManuStartSpread;
        friend class CManuStartMark;

        Q_OBJECT
    public:
        CManuAction(CVM *p_vm);
        ~CManuAction();

        virtual void start() override;
        virtual void stop() override;

    protected:
        IActionState* m_idle;
        IActionState* m_startSpread;
        IActionState* m_startMark;
    };
}