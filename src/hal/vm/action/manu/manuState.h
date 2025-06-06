#pragma once
#include "../iActionState.h"
#include "../../vm.h"

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
        CManuAction* m_action;
        IVM* m_pVM;
    };

    class CManuStartSpread : public CManuIdleState
    {
        Q_OBJECT
    public:
        using CManuIdleState::CManuIdleState;
        virtual void run() override;
        virtual void runing() override;
    };

    class CManuStartMark : public CManuIdleState
    {
        Q_OBJECT
    public:
        using CManuIdleState::CManuIdleState;
        virtual void run() override;
        virtual void runing() override;
    };
}