#pragma once
#include "../iAction.h"
#include "../iActionState.h"

namespace TIGER_VMSLM
{
    class CMarkOnceAction : public IAction
    {
        friend class CMarkOnceIdleState;
        friend class CDownloadMarkFile;
        friend class CStartMark;

        Q_OBJECT
    public:
        CMarkOnceAction(CVM *p_vm);
        ~CMarkOnceAction();

        virtual void start() override;
        virtual void stop() override;

    protected:
        IActionState* m_idle;
        IActionState* m_downloadMarkFile;
        IActionState* m_startMark;
    };
}