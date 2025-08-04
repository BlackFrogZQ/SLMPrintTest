#pragma once
#include "userDef.h"
#include <QLabel>
class QToolButton;

namespace TIGER_VMSLM
{
    class CVM;
}
class CMainWindowMenuBar : public QLabel
{
    Q_OBJECT
public:
    CMainWindowMenuBar(QWidget *p_pParent = nullptr, TIGER_VMSLM::CVM *p_pVM = nullptr);
    ~CMainWindowMenuBar();
    void initLayout();
    CUserType currentUser() const;
    bool checkPassword();
    void setUsedSetParaPB(bool p_used = true);

public slots:
    void setUserType(CUserType type);
    void slotSetPara();
    void slotSignInClicked();
    void slotSetGlavo();

private:
    CUserType m_userType;
    QToolButton *m_pUserPB;
    QToolButton *m_pCalibratePB;
    QToolButton *m_pSetParaPB;
    TIGER_VMSLM::CVM *m_pVM;
};

CMainWindowMenuBar *mainWindowMenuBar();
