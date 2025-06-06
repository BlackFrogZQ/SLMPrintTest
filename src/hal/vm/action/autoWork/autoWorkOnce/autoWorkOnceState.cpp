#include "autoWorkOnceState.h"
#include "autoWorkOnceAction.h"
#include "ui/mainWindow.h"
#include "hal/plc/plcSigDef.h"
#include <QTimer>
#include <QCoreApplication>
#include <QTime>

namespace TIGER_VMSLM
{
    CAutoWorkOnceIdleState::CAutoWorkOnceIdleState(CAutoWorkOnceAction* p_action) : m_action(p_action)
    {
        assert(m_action != nullptr);
        m_pVM = m_action->m_pVM;
        assert(m_pVM != nullptr);
    }

    CAutoWorkOnceIdleState::~CAutoWorkOnceIdleState()
    {
    }

    void CAutoWorkOnceIdleState::run()
    {
        emit m_action->sigEnd();
    }

    void CStartAutoSpreadOnce::run()
    {
        m_pVM->startSpread();
        QTimer::singleShot(cSenMessageInterval, this, [this]{ runing(); });
    }

    void CStartAutoSpreadOnce::runing()
    {
        if (m_action->m_bStop)
        {
            changeState(m_action->m_idle);
            return;
        }

        if (plcServerData()->colis(cpcSpread) == false)
        {
            changeState(m_action->m_startMarkOnce);
            return;
        }
        else
        {
            QTimer::singleShot(cSenMessageInterval, this, [this]{ runing(); });
        }
    }

    void CStartAutoMarkOnce::run()
    {
        m_pVM->startMark();
        QTimer::singleShot(cSenMessageInterval, this, [this]{ runing(); });
    }

    void CStartAutoMarkOnce::runing()
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
        else
        {
            QTimer::singleShot(cSenMessageInterval, this, [this]{ runing(); });
        }
    }
}