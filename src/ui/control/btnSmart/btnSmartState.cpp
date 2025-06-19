#include "btnSmartState.h"
#include "btnSmart.h"

namespace TIGER_UI_BtnSmart
{
    IState::IState(CBtnSmart* p_btnSmart)
    {
        m_pBtnSmart = p_btnSmart;
    };

    void CNormalState::click()
    {
        emit m_pBtnSmart->sigClick();
    };

    void CNormalState::setCheck()
    {
        // m_pBtnSmart->setBitmap(m_pBtnSmart->m_pCheckBitmap);
        m_pBtnSmart->setCheckState();
    };

    void CNormalState::setDisable()
    {
        // m_pBtnSmart->setBitmap(m_pBtnSmart->m_pDisableBitmap);
        m_pBtnSmart->setDisableState();
    };

    void CNormalState::enterEvent(QEvent *e)
    {
        // m_pBtnSmart->setBitmap(m_pBtnSmart->m_pFocusBitmap);
    };

    void CNormalState::leaveEvent(QEvent *e)
    {
        // m_pBtnSmart->setBitmap(m_pBtnSmart->m_pNormalBitmap);
    };

    void CDisableState::setCheck()
    {
        // m_pBtnSmart->setBitmap(m_pBtnSmart->m_pCheckBitmap);
        m_pBtnSmart->setCheckState();
    };

    void CDisableState::setNormal()
    {
        // m_pBtnSmart->setBitmap(m_pBtnSmart->m_pNormalBitmap);
        m_pBtnSmart->setNormalState();
    };

    void CCheckState::click()
    {
        emit m_pBtnSmart->sigCheckClick();
    };

    void CCheckState::setNormal()
    {
        // m_pBtnSmart->setBitmap(m_pBtnSmart->m_pNormalBitmap);
        m_pBtnSmart->setNormalState();
    };

    void CCheckState::setDisable()
    {
        // m_pBtnSmart->setBitmap(m_pBtnSmart->m_pDisableBitmap);
        m_pBtnSmart->setDisableState();
    };
}

