#include "spreadState.h"
#include "spreadAction.h"

namespace TIGER_VMSLM
{
    CSpreadIdleState::CSpreadIdleState(CSpreadAction* p_action) : m_action(p_action)
    {
        assert(m_action != nullptr);
        m_pVM = m_action->m_pVM;
        assert(m_pVM != nullptr);
    }

    CSpreadIdleState::~CSpreadIdleState()
    {
    }

    void CSpreadIdleState::run()
    {
        emit m_action->sigEnd();
    }

    void CStartSpread::run()
    {
        assert(plcServerData()->colis(cpcSpreading) == false);
        assert(!m_isSpreading);
        m_pVM->sendDiscreteInputs(cpdcStartSpread, true);
        myInfo << cnStr("开始铺粉");
        QTimer::singleShot(cSenMessageInterval, this, [this]{ runing(); });
    }

    void CStartSpread::runing()
    {
        if (m_action->m_bStop)
        {
            myInfo << cnStr("停止铺粉");
            changeState(m_action->m_idle);
            return;
        }

        if (plcServerData()->colis(cpcSpreading) == true && !m_isSpreading)
        {
            m_isSpreading = true;
            m_pVM->sendDiscreteInputs(cpdcStartSpread, false);
            myInfo << cnStr("铺粉中......");
        }
        else if (plcServerData()->colis(cpcSpreading) == false && m_isSpreading)
        {
            m_isSpreading = false;
            myInfo << cnStr("铺粉结束");
            return;
        }
        QTimer::singleShot(cSenMessageInterval, this, [this]{ runing(); });
    }
}