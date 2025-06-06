#include "autoWorkAction.h"
#include "autoWorkState.h"
#include "hal/vm/vm.h"

namespace TIGER_VMSLM
{
    CAutoWorkAction::CAutoWorkAction(CVM *p_vm) : IAction(p_vm)
    {
        m_idle = new CAutoWorkIdleState(this);
        m_startSpread = new CStartAutoSpread(this);
        m_startMark = new CStartAutoMark(this);
    }

    CAutoWorkAction::~CAutoWorkAction()
    {
        delPtr(m_idle);
        delPtr(m_startSpread);
        delPtr(m_startMark);
    }

    void CAutoWorkAction::start()
    {
        IAction::start();
        m_startSpread->run();
    }

    void CAutoWorkAction::stop()
    {
        IAction::stop();
    }
}