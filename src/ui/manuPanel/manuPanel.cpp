#include "manuPanel.h"
#include "system/basic.h"
#include "hal/vm/ncDef.h"
#include "hal/vm/vm.h"
#include "ui/control/ledLab.h"
#include "ui/control/btnSmart/btnSmart.h"
#include <QPushButton>
#include <QLabel>
#include <QTextBrowser>
#include <QVBoxLayout>

using namespace TIGER_VMSLM;
using namespace TIGER_UI_CONTROL;
using namespace TIGER_UI_BtnSmart;

namespace TIGER_UI_SLM
{
    CManuPanel::CManuPanel(CVM *p_pVM) : CLedLab(nullptr), m_pVM(p_pVM)
    {
        init();
        initLayout();
    }
    CManuPanel::~CManuPanel()
    {
    }

    void CManuPanel::init()
    {
        const QHash<int, QString> cBtnNames = {
            {cbiStartManu, cnStr("开始加工")},
            {cbiPauseManu, cnStr("暂停")},
            {cbiManuOnce, cnStr("加工一层")},
            {cbiSpread, cnStr("铺粉")},
            {cbiAutoMark, cnStr("打标")},
            {cbiClearAlarm, cnStr("清除报警")}};
        assert(cBtnNames.size() == cbiMax);
        m_pButtons.resize(cbiMax);
        for (int i = 0; i < cbiMax; i++)
        {
            m_pButtons[i] = new CBtnSmart("top");
            m_pButtons[i]->setText(cBtnNames[i]);
        }

        connect(m_pButtons[cbiStartManu], &CBtnSmart::sigClick, this, [this]{ m_pVM->manu(); });
        connect(m_pButtons[cbiStartManu], &CBtnSmart::sigCheckClick, this, [this]{ m_pVM->stopManu(); });
        connect(m_pButtons[cbiManuOnce], &CBtnSmart::sigClick, this, [this]{ m_pVM->manuOnce(); });
        connect(m_pButtons[cbiManuOnce], &CBtnSmart::sigCheckClick, this, [this]{ m_pVM->stopManuOnce(); });
        connect(m_pButtons[cbiSpread], &CBtnSmart::sigClick, this, [this]{ m_pVM->spread(); });
        connect(m_pButtons[cbiSpread], &CBtnSmart::sigCheckClick, this, [this]{ m_pVM->stopSpread(); });
        connect(m_pButtons[cbiAutoMark], &CBtnSmart::sigClick, this, [this]{ m_pVM->startMark(); });
        connect(m_pButtons[cbiAutoMark], &CBtnSmart::sigCheckClick, this, [this]{ m_pVM->stopMark(); });
    }

    void CManuPanel::initLayout()
    {
        auto getRow = [this](QWidget *p_left, QWidget *p_mid, QWidget *p_right) -> QHBoxLayout *
        {
            QHBoxLayout *pLayout = new QHBoxLayout;
            pLayout->addStretch();
            pLayout->addWidget(p_left);
            pLayout->addStretch();
            pLayout->addWidget(p_mid);
            pLayout->addStretch();
            pLayout->addWidget(p_right);
            pLayout->addStretch();
            pLayout->setSpacing(0);
            pLayout->setMargin(0);
            return pLayout;
        };

        QVBoxLayout *pLayout = new QVBoxLayout;
        pLayout->addStretch();
        pLayout->addLayout(getRow(m_pButtons[cbiStartManu], m_pButtons[cbiPauseManu], m_pButtons[cbiManuOnce]));
        pLayout->addStretch();
        pLayout->addLayout(getRow(m_pButtons[cbiSpread], m_pButtons[cbiAutoMark], m_pButtons[cbiClearAlarm]));
        pLayout->addStretch();
        pLayout->setSpacing(2);
        pLayout->setMargin(10);
        pLayout->setSizeConstraint(QLayout::SetMinimumSize);
        this->setMinimumSize(pLayout->sizeHint());
        this->setLayout(pLayout);
    }
}