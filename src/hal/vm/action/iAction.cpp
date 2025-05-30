#include "iAction.h"
#include "./autoWork/autoWorkAction.h"

namespace TIGER_VMSLM
{
    IAction::IAction(CVM *p_vm) : QObject(nullptr), m_pVM(p_vm), m_bStop(false), runCount(0)
    {
        assert(m_pVM != nullptr);
    }

    IAction::~IAction()
    {
    }

    void IAction::start()
    {
        m_bStop = false;
    }

    void IAction::stop()
    {
        m_bStop = true;
    }


    CActionCreater::CActionCreater(CVM *p_vm) : m_pVM(p_vm)
    {
        assert(m_pVM != nullptr);
    }

    IAction *CActionCreater::autoWorkAction()
    {
        return new CAutoWorkAction(m_pVM);
    }
}