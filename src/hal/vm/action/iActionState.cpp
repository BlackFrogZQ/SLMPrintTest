#include "iActionState.h"

namespace TIGER_VMSLM
{
    IActionState::~IActionState()
    {
    }

    void IActionState::runing()
    {
    }

    void IActionState::changeState(IActionState *p_state)
    {
        p_state->run();
    }
}