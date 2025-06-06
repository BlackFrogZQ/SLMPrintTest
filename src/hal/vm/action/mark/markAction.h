#pragma once
#include "../iAction.h"
#include "../iActionState.h"

namespace TIGER_VMSLM
{
    class CMarkAction : public IAction
    {
        friend class CMarkIdleState;
        friend class CDownloadMarkFile;
        friend class CStartMark;

        Q_OBJECT
    public:
        CMarkAction(CVM *p_vm);
        ~CMarkAction();

        virtual void start() override;
        virtual void stop() override;

    protected:
        IActionState* m_idle;
        IActionState* m_downloadMarkFile;
        IActionState* m_startMark;
    };
}