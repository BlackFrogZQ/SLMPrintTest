#include "manuOnceAction.h"
#include "manuOnceState.h"
#include "hal/vm/vm.h"

namespace TIGER_VMSLM
{
    CManuOnceAction::CManuOnceAction(CVM *p_vm) : IAction(p_vm)
    {
        m_manuOnceIdle = new CManuOnceIdleState(this);
        m_manuOnceSpread = new CManuOnceSpread(this);
        m_manuOnceMark = new CManuOnceMark(this);
    }

    CManuOnceAction::~CManuOnceAction()
    {
        delPtr(m_manuOnceIdle);
        delPtr(m_manuOnceSpread);
        delPtr(m_manuOnceMark);
    }

    void CManuOnceAction::start()
    {
        IAction::start();
        m_manuOnceSpread->run();
    }

    void CManuOnceAction::stop()
    {
        IAction::stop();
    }
}
