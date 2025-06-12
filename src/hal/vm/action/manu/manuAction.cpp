#include "manuAction.h"
#include "manuState.h"
#include "hal/vm/vm.h"
#include "hal/vm/manuDef.h"

using namespace TIGER_SLMManuDef;
namespace TIGER_VMSLM
{
    CManuAction::CManuAction(CVM *p_vm) : IAction(p_vm)
    {
        m_manuIdle = new CManuIdleState(this);
        m_manuOnceCycle = new CManuOnceCycle(this);
    }

    CManuAction::~CManuAction()
    {
        delPtr(m_manuIdle);
        delPtr(m_manuOnceCycle);
    }

    void CManuAction::start()
    {
        IAction::start();
        myInfo << cnStr("开始加工");
        manuStatus()->currentLayer = 0;
        m_manuOnceCycle->run();
    }

    void CManuAction::stop()
    {
        IAction::stop();
    }
}