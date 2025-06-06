#include "manuState.h"
#include "manuAction.h"
#include "ui/mainWindow.h"
#include "hal/plc/plcSigDef.h"
#include <QTimer>
#include <QCoreApplication>
#include <QTime>

namespace TIGER_VMSLM
{
    CManuIdleState::CManuIdleState(CManuAction *p_action) : m_action(p_action)
    {
        assert(m_action != nullptr);
        m_pVM = m_action->m_pVM;
        m_action->runCount = 0;
        assert(m_pVM != nullptr);
    }

    CManuIdleState::~CManuIdleState()
    {
    }

    void CManuIdleState::run()
    {
        myInfo << cnStr("自动工作结束");
        emit m_action->sigEnd();
    }


    void CManuStartSpread::run()
    {
        assert(plcServerData()->colis(cpcReady) == true);
        myInfo << cnStr("开始第%1层打印").arg(++m_action->runCount);
        m_pVM->sendDisColis(cpdcStartSpread, true);
        myInfo << cnStr("开始铺粉");
        QTimer::singleShot(cSenMessageInterval, this, [this]{ runing(); });
    }

    void CManuStartSpread::runing()
    {
        if (m_action->m_bStop)
        {
            changeState(m_action->m_manuIdle);
            return;
        }

        if (plcServerData()->colis(cpcSpreadEnd) == true)
        {
            m_pVM->sendDisColis(cpdcStartSpread, false);
            changeState(m_action->m_manuMark);
            return;
        }
        else
        {
            QTimer::singleShot(cSenMessageInterval, this, [this]{ runing(); });
        }
    }


    void CManuStartMark::run()
    {
        assert(plcServerData()->colis(cpcSpreadEnd) == true);
        m_pVM->sendDisColis(cpdcStartMark, true);
        myInfo << cnStr("开始扫描");
        {
            QTime time;
            time.start();
            while(time.elapsed() < 3000)
                QCoreApplication::processEvents();
        }
        QTimer::singleShot(cSenMessageInterval, this, [this]{ runing(); });
    }

    void CManuStartMark::runing()
    {
        if (m_action->m_bStop)
        {
            changeState(m_action->m_manuIdle);
            return;
        }

        if (plcServerData()->colis(cpcSpreadEnd) == false)
        {
            m_pVM->sendDisColis(cpdcStartMark, false);
            changeState(m_action->m_manuSpread);
            return;
        }
        else
        {
            QTimer::singleShot(cSenMessageInterval, this, [this]{ runing(); });
        }
    }
}