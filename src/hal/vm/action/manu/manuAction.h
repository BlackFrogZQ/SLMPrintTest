#pragma once
#include "../iAction.h"
#include "../iActionState.h"

namespace TIGER_VMSLM
{
    class CManuAction : public IAction
    {
        friend class CManuIdleState;
        friend class CManuOnceCycle;

        Q_OBJECT
    public:
        CManuAction(CVM *p_vm);
        ~CManuAction();

        virtual void start() override;
        virtual void stop() override;

    protected:
        IActionState* m_manuIdle;
        IActionState* m_manuOnceCycle;
    };
}