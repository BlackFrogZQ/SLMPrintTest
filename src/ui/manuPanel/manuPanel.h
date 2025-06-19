#pragma once
#include "ui/control/ledLab.h"

namespace TIGER_VMSLM
{
    class CVM;
}

namespace TIGER_UI_SLM
{
    class CManuPanel: public TIGER_UI_CONTROL::CLedLab
    {
        Q_OBJECT
    public:
        CManuPanel(TIGER_VMSLM::CVM *p_pVM = nullptr);
        ~CManuPanel();

    protected:
        void init();
        void initLayout();

    protected:
        enum CButtonID
        {
            cbiStartManu = 0,
            cbiPauseManu,
            cbiManuOnce,
            cbiSpread,
            cbiAutoMark,
            cbiClearAlarm,
            cbiMax
        };

    private:
        TIGER_VMSLM::CVM *m_pVM;
    };
}