#include "menuBar.h"
#include "../basic.h"
#include "inputDialog/inputDialog.h"
#include "messageBox/messageBox.h"
#include "setParaWindow/setParaWindow.h"
#include "system/systemService.h"
#include "scanSystemWindow/calibrateDialog.h"
#include "hal/vm/vm.h"
#include "hal/vm/manuDef.h"
#include <QToolButton>
#include <QHBoxLayout>

using namespace TIGER_VMSLM;
const int cSize = 30;
CMainWindowMenuBar *g_pMainWindowMenuBar = nullptr;
CMainWindowMenuBar *mainWindowMenuBar()
{
    return g_pMainWindowMenuBar;
}

CMainWindowMenuBar::CMainWindowMenuBar(QWidget *p_pParent, CVM *p_pVM) : QLabel(p_pParent), m_pVM(p_pVM), m_userType(cutUndefine)
{
    g_pMainWindowMenuBar = this;
    setFocusPolicy(Qt::ClickFocus);
    setFixedHeight(cSize / 0.8);
    initLayout();
    slotSignInClicked();
}

CMainWindowMenuBar::~CMainWindowMenuBar()
{
}

void CMainWindowMenuBar::initLayout()
{
    QHBoxLayout *pLayout = new QHBoxLayout;
    auto getToolPb = []
    {
        auto p = new QToolButton;
        p->setStyleSheet("QToolButton{background:transparent;border:none;}"
                         "QToolButton:hover:!checked{background-color:rgb(218,219,220);}"
                         "QToolButton { padding: 0px; margin: 0px; spacing: 0px; }");
        p->setIconSize(QSize(cSize, cSize));
        return p;
    };

    {
        m_pUserPB = getToolPb();
        m_pUserPB->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        connect(m_pUserPB, &QToolButton::clicked, this, &CMainWindowMenuBar::slotSignInClicked);
        pLayout->addWidget(m_pUserPB);
    }

    {
        m_pCalibratePB = getToolPb();
        m_pCalibratePB->setToolButtonStyle(Qt::ToolButtonIconOnly);
        m_pCalibratePB->setIcon(QIcon(":res/menu/glavo.png"));
        m_pCalibratePB->setToolTip(cnStr("振镜调试"));
        connect(m_pCalibratePB, &QToolButton::clicked, this, &CMainWindowMenuBar::slotSetGlavo);
        pLayout->addWidget(m_pCalibratePB);
    }

    {
        m_pSetParaPB = getToolPb();
        m_pSetParaPB->setToolButtonStyle(Qt::ToolButtonIconOnly);
        m_pSetParaPB->setIcon(QIcon(":res/menu/setting.png"));
        m_pSetParaPB->setToolTip(cnStr("设置"));
        connect(m_pSetParaPB, &QToolButton::clicked, this, &CMainWindowMenuBar::slotSetPara);
        pLayout->addWidget(m_pSetParaPB);
    }

    pLayout->addStretch();
    pLayout->setMargin(0);
    pLayout->setSpacing(5);
    this->setLayout(pLayout);
}

CUserType CMainWindowMenuBar::currentUser() const
{
    return m_userType;
}

void CMainWindowMenuBar::setUsedSetParaPB(bool p_used)
{
    m_pCalibratePB->setVisible(p_used);
    m_pSetParaPB->setVisible(p_used);
}

bool CMainWindowMenuBar::checkPassword()
{
    QString pwStr = CInputDialog::getText(cnStr("权限切换"), cnStr("请输入管理员密码"), QLineEdit::Password, QString(), this).trimmed();
    if (pwStr.size() > 0)
    {
        if (pwStr != QString("123"))
        {
            TIGER_UIBasic::showToolTip(this, cnStr("密码错误"));
            myDebug << cnStr("密码错误");
            return false;
        }
        return true;
    }
    return false;
}

void CMainWindowMenuBar::setUserType(CUserType type)
{
    m_userType = type;
    QMetaObject::invokeMethod(this, [type, this]
        {
            bool isAdministrators = (type == cutAdministrators);
            m_pUserPB->setIcon(QIcon(isAdministrators ? ":res/menu/administrator.png" : ":res/menu/user.png"));
            setUsedSetParaPB(isAdministrators);
            m_pUserPB->setText(cUserTypeStr[currentUser()]);
            myInfo << cnStr("当前用户类型：%1").arg(m_pUserPB->text());
        }, Qt::QueuedConnection);
}

void CMainWindowMenuBar::slotSignInClicked()
{
    switch (currentUser())
    {
    case cutUndefine:
        setUserType(cutOperator);
        break;
    case cutOperator:
        if (checkPassword())
        {
            setUserType(cutAdministrators);
        }
        break;
    case cutAdministrators:
        if (CMessageBox::Yes == CMessageBox::question(this, cnStr("权限切换"), cnStr("是否退出%1,切换为%2?").arg(cUserTypeStr[cutAdministrators]).arg(cUserTypeStr[cutOperator])))
        {
            setUserType(cutOperator);
        }
        break;
    default:
        break;
    }
}

void CMainWindowMenuBar::slotSetPara()
{
    if (currentUser() == CUserType::cutAdministrators || checkPassword())
    {
        CSetParaWindow setParaWindow(this);
        if (setParaWindow.showRootNode())
        {
            if (setParaWindow.isSave())
            {
                sys()->save();
                sys()->updateParas();
            }
            else
            {
                sys()->load();
            }
        }
        else
        {
            TIGER_UIBasic::warningBox(this, cnStr("参数错误"), cnStr("无有效参数，无法设置"));
        }
    }
}

void CMainWindowMenuBar::slotSetGlavo()
{
    if (currentUser() == CUserType::cutAdministrators || checkPassword())
    {
        CalibrateDialog dialog(this, m_pVM);
        dialog.exec();
        if (auto s = TIGER_SLMManuDef::manuStatus(); s->existSLCDatas)
            s->updataSLCLayerStatus();
    }
}