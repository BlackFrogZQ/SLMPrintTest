#pragma once
#include "../iActionState.h"
#include "../../vm.h"
#include <QFuture>

namespace TIGER_VMSLM
{
    class CManuAction;

    class CManuIdleState : public IActionState
    {
        Q_OBJECT
    public:
        CManuIdleState(CManuAction* p_action);
        ~CManuIdleState();
        virtual void run() override;

    protected:
        QMetaObject::Connection m_connect;
        CManuAction* m_action;
        IVM* m_pVM;
    };

    class CManuOnceCycle : public CManuIdleState
    {
        Q_OBJECT
    public:
        using CManuIdleState::CManuIdleState;
        virtual void run() override;
        virtual void runing() override;

    protected:
        QFuture<void> m_updateLayerFuture;
    };
}