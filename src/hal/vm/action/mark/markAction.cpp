#include "markAction.h"
#include "markState.h"

namespace TIGER_VMSLM
{
    CMarkAction::CMarkAction(CVM *p_vm) : IAction(p_vm)
    {
        m_idle = new CMarkIdleState(this);
        m_downloadMarkFile = new CDownloadMarkFile(this);
        m_startMark = new CStartMark(this);
    }

    CMarkAction::~CMarkAction()
    {
        delPtr(m_idle);
        delPtr(m_downloadMarkFile);
        delPtr(m_startMark);
    }

    void CMarkAction::start()
    {
        IAction::start();
        m_downloadMarkFile->run();
    }

    void CMarkAction::stop()
    {
        IAction::stop();
    }
}