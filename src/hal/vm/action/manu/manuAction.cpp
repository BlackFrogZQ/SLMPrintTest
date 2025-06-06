#include "manuAction.h"
#include "manuState.h"
#include "hal/vm/vm.h"

namespace TIGER_VMSLM
{
    CManuAction::CManuAction(CVM *p_vm) : IAction(p_vm)
    {
        m_idle = new CManuIdleState(this);
        m_startSpread = new CManuStartSpread(this);
        m_startMark = new CManuStartMark(this);
    }

    CManuAction::~CManuAction()
    {
        delPtr(m_idle);
        delPtr(m_startSpread);
        delPtr(m_startMark);
    }

    void CManuAction::start()
    {
        IAction::start();
        m_startSpread->run();
    }

    void CManuAction::stop()
    {
        IAction::stop();
    }
}