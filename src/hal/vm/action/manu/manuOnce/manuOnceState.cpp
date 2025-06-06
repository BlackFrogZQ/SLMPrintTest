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
        m_isSpreading = false;
        QTimer::singleShot(cSenMessageInterval, this, [this]{ runing(); });
    }

    void CManuOnceSpread::runing()
    {
        if (m_action->m_bStop)
        {
            changeState(m_action->m_manuOnceIdle);
            return;
        }

        if (plcServerData()->colis(cpcSpreading) == true && !m_isSpreading)
        {
            m_isSpreading = true;
        }
        else if (plcServerData()->colis(cpcSpreading) == false && plcServerData()->disColis(cpdcStartSpread) == false && m_isSpreading)
        {
            m_isSpreading = false;
            changeState(m_action->m_manuOnceMark);
            return;
        }
        QTimer::singleShot(cSenMessageInterval, this, [this]{ runing(); });
    }

    void CManuOnceMark::run()
    {
        m_pVM->startMark();
        m_isMarking = false;
        QTimer::singleShot(cSenMessageInterval, this, [this]{ runing(); });
    }

    void CManuOnceMark::runing()
    {
        if (m_action->m_bStop)
        {
            changeState(m_action->m_manuOnceIdle);
            return;
        }

        if(GMCState()->getMarkingStatus() == true && !m_isMarking)
        {
            m_isMarking = true;
        }
        else if (GMCState()->getMarkingStatus() == false && m_isMarking)
        {
            m_isMarking = false;
            changeState(m_action->m_manuOnceIdle);
            return;
        }
        QTimer::singleShot(cSenMessageInterval, this, [this]{ runing(); });
    }
}