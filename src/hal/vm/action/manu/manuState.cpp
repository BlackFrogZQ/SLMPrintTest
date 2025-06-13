#include "manuState.h"
#include "manuAction.h"
#include "ui/mainWindow.h"
#include "hal/plc/plcSigDef.h"
#include "hal/vm/manuDef.h"
#include <QTimer>
#include <QTime>
#include <QtConcurrent>
#include <QCoreApplication>

using namespace TIGER_SLMManuDef;
namespace TIGER_VMSLM
{
    CManuIdleState::CManuIdleState(CManuAction *p_action) : m_action(p_action)
    {
        assert(m_action != nullptr);
        m_pVM = m_action->m_pVM;
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

    void CManuOnceCycle::run()
    {
        assert(plcServerData()->colis(cpcReady) == true);
        m_updateLayerFuture = QtConcurrent::run([this]
        {
            manuStatus()->updateLayerStatus();
            QMetaObject::invokeMethod(m_action, "sigSliceUpdate", Qt::QueuedConnection);
        });
        m_pVM->manuOnce();
        m_connect = connect(m_pVM, &IVM::sigManuOnceEnd, this, [this](){ runing(); });
    }

    void CManuOnceCycle::runing()
    {
        disconnect(m_connect);
        if (m_action->m_bStop)
        {
            changeState(m_action->m_manuIdle);
            return;
        }
        manuStatus()->currentLayer++;
        QTimer::singleShot(cSenMessageInterval, this, [this]{ run(); });
    }
}