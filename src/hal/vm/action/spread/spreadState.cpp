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
        assert(plcServerData()->colis(cpcSpread) == false);
        assert(!m_isSpread);
        m_pVM->sendDisColis(cpdcStartSpread, true);
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

        if (plcServerData()->colis(cpcSpread) == true && !m_isSpread)
        {
            m_isSpread = true;
            myInfo << cnStr("铺粉中......");
        }
        else if (plcServerData()->colis(cpcSpread) == false && m_isSpread)
        {
            m_isSpread = false;
            m_pVM->sendDisColis(cpdcStartSpread, false);
            myInfo << cnStr("铺粉结束");
            return;
        }
        QTimer::singleShot(cSenMessageInterval, this, [this]{ runing(); });
    }
}