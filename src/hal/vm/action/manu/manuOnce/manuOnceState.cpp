#include "manuOnceState.h"
#include "manuOnceAction.h"
#include "ui/mainWindow.h"
#include "hal/plc/plcSigDef.h"
#include <QTimer>
#include <QCoreApplication>
#include <QTime>

namespace TIGER_VMSLM
{
    CManuOnceIdleState::CManuOnceIdleState(CManuOnceAction* p_action) : m_action(p_action)
    {
        assert(m_action != nullptr);
        m_pVM = m_action->m_pVM;
        assert(m_pVM != nullptr);
    }

    CManuOnceIdleState::~CManuOnceIdleState()
    {
    }

    void CManuOnceIdleState::run()
    {
        emit m_action->sigEnd();
    }

    void CManuOnceSpread::run()
    {
        m_pVM->spread();
        m_connect = connect(m_pVM, &IVM::sigSpreadEnd, this, [this](){ runing(); });
    }

    void CManuOnceSpread::runing()
    {
        disconnect(m_connect);
        if (m_action->m_bStop)
        {
            changeState(m_action->m_manuOnceIdle);
        }
        else
        {
            changeState(m_action->m_manuOnceMark);
        }
    }

    void CManuOnceMark::run()
    {
        m_pVM->startMark();
        m_connect = connect(m_pVM, &IVM::sigMarkEnd, this, [this](){ runing(); });
    }

    void CManuOnceMark::runing()
    {
        disconnect(m_connect);
        if (m_action->m_bStop)
        {
            changeState(m_action->m_manuOnceIdle);
        }
        else
        {
            changeState(m_action->m_manuOnceIdle);
        }
    }
}