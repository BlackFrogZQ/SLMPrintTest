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
        m_pVM->startSpread();
        QTimer::singleShot(cSenMessageInterval, this, [this]{ runing(); });
    }

    void CManuOnceSpread::runing()
    {
        if (m_action->m_bStop)
        {
            changeState(m_action->m_idle);
            return;
        }

        if (plcServerData()->colis(cpcSpread) == false && plcServerData()->disColis(cpdcStartSpread) == false)
        {
            changeState(m_action->m_startMarkOnce);
            return;
        }
        QTimer::singleShot(cSenMessageInterval, this, [this]{ runing(); });
    }

    void CManuOnceMark::run()
    {
        m_pVM->startMark();
        QTimer::singleShot(cSenMessageInterval, this, [this]{ runing(); });
    }

    void CManuOnceMark::runing()
    {
        if (m_action->m_bStop)
        {
            changeState(m_action->m_idle);
            return;
        }

        if (GMCState()->getMarkingStatus() == false)
        {
            changeState(m_action->m_idle);
            return;
        }
        QTimer::singleShot(cSenMessageInterval, this, [this]{ runing(); });
    }
}