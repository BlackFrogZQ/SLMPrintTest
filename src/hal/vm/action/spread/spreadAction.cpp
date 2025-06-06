#include "spreadAction.h"
#include "spreadState.h"

namespace TIGER_VMSLM
{
    CSpreadAction::CSpreadAction(CVM *p_vm) : IAction(p_vm)
    {
        m_idle = new CSpreadIdleState(this);
        m_startSpread = new CStartSpread(this);
    }

    CSpreadAction::~CSpreadAction()
    {
        delPtr(m_idle);
        delPtr(m_startSpread);
    }

    void CSpreadAction::start()
    {
        IAction::start();
        m_startSpread->run();
    }

    void CSpreadAction::stop()
    {
        IAction::stop();
    }
}