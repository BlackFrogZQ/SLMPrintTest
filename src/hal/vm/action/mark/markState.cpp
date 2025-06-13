#include "markState.h"
#include "markAction.h"
#include "hal/vm/manuDef.h"
#include "printDatas/printDatasDef.h"
using namespace TIGER_PrintDatas;

namespace TIGER_VMSLM
{
    CMarkIdleState::CMarkIdleState(CMarkAction* p_action) : m_action(p_action)
    {
        assert(m_action != nullptr);
        m_pVM = m_action->m_pVM;
        assert(m_pVM != nullptr);
    }

    CMarkIdleState::~CMarkIdleState()
    {
    }

    void CMarkIdleState::run()
    {
        emit m_action->sigEnd();
    }

    void CDownloadMarkFile::run()
    {
        myInfo << cnStr("开始创建并下载打标文件");
        m_pVM->creatUdmBin(TIGER_SLMManuDef::manuStatus()->layerStatus.allSegments);
        GMCState()->setDownloadStatus(true);
        QTimer::singleShot(cSenMessageInterval, this, [this]{ runing(); });
    }

    void CDownloadMarkFile::runing()
    {
        if (m_action->m_bStop)
        {
            changeState(m_action->m_idle);
            return;
        }

        if (GMCState()->getDownloadStatus() == false)
        {
            myInfo << cnStr("打标文件下载完成");
            changeState(m_action->m_startMark);
            return;
        }
        QTimer::singleShot(cSenMessageInterval, this, [this]{ runing(); });
    }

    void CStartMark::run()
    {
        // assert(plcServerData()->colis(cpcReady) == true);
        // assert(plcServerData()->colis(cpcSpreadEnd) == true);
        myInfo << cnStr("开始打标");
        m_pVM->sendDiscreteInputs(cpdcStartMark, true);
        m_pVM->GMCStarMark();
        GMCState()->setMarkingStatus(true);
        QTimer::singleShot(cSenMessageInterval, this, [this]{ runing(); });
    }

    void CStartMark::runing()
    {
        if (m_action->m_bStop)
        {
            myInfo << cnStr("停止打标");
            changeState(m_action->m_idle);
            return;
        }

        if (GMCState()->getMarkingStatus() == false)
        {
            m_pVM->sendDiscreteInputs(cpdcStartMark, false);
            myInfo << cnStr("打标完成");
            changeState(m_action->m_idle);
            return;
        }
        QTimer::singleShot(cSenMessageInterval, this, [this]{ runing(); });
    }
}