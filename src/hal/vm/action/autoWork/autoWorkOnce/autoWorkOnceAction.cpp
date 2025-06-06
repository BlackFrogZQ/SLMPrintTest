#include "autoWorkOnceAction.h"
#include "autoWorkOnceState.h"
#include "hal/vm/vm.h"

namespace TIGER_VMSLM
{
    CAutoWorkOnceAction::CAutoWorkOnceAction(CVM *p_vm) : IAction(p_vm)
    {
        m_idle = new CAutoWorkOnceIdleState(this);
        m_startSpreadOnce = new CStartAutoSpreadOnce(this);
        m_startMarkOnce = new CStartAutoMarkOnce(this);
    }

    CAutoWorkOnceAction::~CAutoWorkOnceAction()
    {
        delPtr(m_idle);
        delPtr(m_startSpreadOnce);
        delPtr(m_startMarkOnce);
    }

    void CAutoWorkOnceAction::start()
    {
        IAction::start();
        m_startSpreadOnce->run();
    }

    void CAutoWorkOnceAction::stop()
    {
        IAction::stop();
    }
}
