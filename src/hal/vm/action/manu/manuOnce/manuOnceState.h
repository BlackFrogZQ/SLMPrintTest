#pragma once
#include "vm/vm.h"
#include "vm/action/iActionState.h"

namespace TIGER_VMSLM
{
    class CManuOnceAction;

    class CManuOnceIdleState : public IActionState
    {
        Q_OBJECT
    public:
        CManuOnceIdleState(CManuOnceAction* p_action);
        ~CManuOnceIdleState();
        virtual void run() override;

    protected:
        QMetaObject::Connection m_connect;
        CManuOnceAction* m_action;
        IVM* m_pVM;
    };

    class CManuOnceSpread : public CManuOnceIdleState
    {
        Q_OBJECT
    public:
        using CManuOnceIdleState::CManuOnceIdleState;
        virtual void run() override;
        virtual void runing() override;
    };

    class CManuOnceMark : public CManuOnceIdleState
    {
        Q_OBJECT
    public:
        using CManuOnceIdleState::CManuOnceIdleState;
        virtual void run() override;
        virtual void runing() override;
    };
}