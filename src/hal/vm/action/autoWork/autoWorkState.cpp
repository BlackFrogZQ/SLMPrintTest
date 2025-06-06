#include "autoWorkState.h"
#include "autoWorkAction.h"
#include "ui/mainWindow.h"
#include "hal/plc/plcSigDef.h"
#include <QTimer>
#include <QCoreApplication>
#include <QTime>

namespace TIGER_VMSLM
{
    CAutoWorkIdleState::CAutoWorkIdleState(CAutoWorkAction *p_action) : m_action(p_action)
    {
        assert(m_action != nullptr);
        m_pVM = m_action->m_pVM;
        m_action->runCount = 0;
        assert(m_pVM != nullptr);
    }

    CAutoWorkIdleState::~CAutoWorkIdleState()
    {
    }

    void CAutoWorkIdleState::run()
    {
        myInfo << cnStr("自动工作结束");
        emit m_action->sigEnd();
    }


    void CStartAutoSpread::run()
    {
        // assert(plcServerData()->colis(cpcReady) == true);
        myInfo << cnStr("开始第%1层打印").arg(++m_action->runCount);
        m_pVM->sendDisColis(cpdcStartSpread, true);
        myInfo << cnStr("开始铺粉");
        QTimer::singleShot(cSenMessageInterval, this, [this]{ runing(); });
    }

    void CStartAutoSpread::runing()
    {
        if (m_action->m_bStop)
        {
            changeState(m_action->m_idle);
            return;
        }

        if (plcServerData()->colis(cpcSpreadEnd) == true)
        {
            m_pVM->sendDisColis(cpdcStartSpread, false);
            changeState(m_action->m_startMark);
            return;
        }
        else
        {
            QTimer::singleShot(cSenMessageInterval, this, [this]{ runing(); });
        }
    }


    void CStartAutoMark::run()
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

    void CStartAutoMark::runing()
    {
        if (m_action->m_bStop)
        {
            changeState(m_action->m_idle);
            return;
        }

        if (plcServerData()->colis(cpcSpreadEnd) == false)
        {
            m_pVM->sendDisColis(cpdcStartMark, false);
            changeState(m_action->m_startSpread);
            return;
        }
        else
        {
            QTimer::singleShot(cSenMessageInterval, this, [this]{ runing(); });
        }
    }
}