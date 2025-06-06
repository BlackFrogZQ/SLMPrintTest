#include "manuAction.h"
#include "manuState.h"
#include "hal/vm/vm.h"

namespace TIGER_VMSLM
{
    CManuAction::CManuAction(CVM *p_vm) : IAction(p_vm)
    {
        m_manuIdle = new CManuIdleState(this);
        m_manuSpread = new CManuStartSpread(this);
        m_manuMark = new CManuStartMark(this);
    }

    CManuAction::~CManuAction()
    {
        delPtr(m_manuIdle);
        delPtr(m_manuSpread);
        delPtr(m_manuMark);
    }

    void CManuAction::start()
    {
        IAction::start();
        m_manuSpread->run();
    }

    void CManuAction::stop()
    {
        IAction::stop();
    }
}