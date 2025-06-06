#include "manuOnceAction.h"
#include "manuOnceState.h"
#include "hal/vm/vm.h"

namespace TIGER_VMSLM
{
    CManuOnceAction::CManuOnceAction(CVM *p_vm) : IAction(p_vm)
    {
        m_idle = new CManuOnceIdleState(this);
        m_startSpreadOnce = new CManuOnceSpread(this);
        m_startMarkOnce = new CManuOnceMark(this);
    }

    CManuOnceAction::~CManuOnceAction()
    {
        delPtr(m_idle);
        delPtr(m_startSpreadOnce);
        delPtr(m_startMarkOnce);
    }

    void CManuOnceAction::start()
    {
        IAction::start();
        m_startSpreadOnce->run();
    }

    void CManuOnceAction::stop()
    {
        IAction::stop();
    }
}
