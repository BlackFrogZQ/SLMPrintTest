#include "markOnceAction.h"
#include "markOnceState.h"

namespace TIGER_VMSLM
{
    CMarkOnceAction::CMarkOnceAction(CVM *p_vm) : IAction(p_vm)
    {
        m_idle = new CMarkOnceIdleState(this);
        m_downloadMarkFile = new CDownloadMarkFile(this);
        m_startMark = new CStartMark(this);
    }

    CMarkOnceAction::~CMarkOnceAction()
    {
        delPtr(m_idle);
        delPtr(m_downloadMarkFile);
        delPtr(m_startMark);
    }

    void CMarkOnceAction::start()
    {
        IAction::start();
        m_downloadMarkFile->run();
    }

    void CMarkOnceAction::stop()
    {
        IAction::stop();
    }
}