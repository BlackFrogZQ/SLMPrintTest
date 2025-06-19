#include "btnSmart.h"
#include "btnSmartState.h"
#include "system/basic.h"
#include <QPainter>

namespace TIGER_UI_BtnSmart
{
    CBtnSmart::CBtnSmart(const QString &p_fileName, QSize p_buttonSize)
    {
        m_pNormalBitmap = new QPixmap(QString(":/res/button/") + p_fileName + "-" + "1");
        assert(!m_pNormalBitmap->isNull());
        m_pFocusBitmap = new QPixmap(QString(":/res/button/") + p_fileName + "-" + "2");
        assert(!m_pFocusBitmap->isNull());
        m_pCheckBitmap = new QPixmap(QString(":/res/button/") + p_fileName + "-" + "3");
        assert(!m_pCheckBitmap->isNull());
        m_pDisableBitmap = new QPixmap(QString(":/res/button/") + p_fileName + "-" + "4");
        assert(!m_pDisableBitmap->isNull());

        m_pNormalState = new CNormalState(this);
        m_pDisableState = new CDisableState(this);
        m_pCheckState = new CCheckState(this);
        m_pState = m_pNormalState;

        m_buttonSize = p_buttonSize;
        setFixedSize(m_buttonSize);
        setBitmap(m_pNormalBitmap);
        setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        setAttribute(Qt::WA_TranslucentBackground);

        connect(this, SIGNAL(clicked()), this, SLOT(click()));
        setFocusPolicy(Qt::NoFocus);
    }

    CBtnSmart::~CBtnSmart()
    {
        delPtr(m_pNormalState);
        delPtr(m_pDisableState);
        delPtr(m_pCheckState);
    }

    void CBtnSmart::setState(CSetState p_state)
    {
        switch (p_state)
        {
        case ssNormal:
            setNormal();
            break;
        case ssDisable:
            setDisable();
            break;
        case ssCheck:
            setCheck();
            break;
        default:
            break;
        }
    }

    void CBtnSmart::setNormal()
    {
        m_pState->setNormal();
    };

    void CBtnSmart::setDisable()
    {
        m_pState->setDisable();
    };

    void CBtnSmart::setCheck()
    {
        m_pState->setCheck();
    };

    bool CBtnSmart::isNormal()
    {
        return m_pState == m_pNormalState;
    }

    bool CBtnSmart::isDisable()
    {
        return m_pState == m_pDisableState;
    }

    bool CBtnSmart::isCheck()
    {
        return m_pState == m_pCheckState;
    }

    void CBtnSmart::click()
    {
        m_pState->click();
    };

    void CBtnSmart::setBitmap(QPixmap *p_pBitmap)
    {
        m_pCurrentBitmap = p_pBitmap;
        setIcon(*p_pBitmap);
        setIconSize(m_buttonSize);
    };

    void CBtnSmart::enterEvent(QEvent *e)
    {
        isEnabled() ? m_pState->enterEvent(e) : m_pDisableState->enterEvent(e);
    };

    void CBtnSmart::leaveEvent(QEvent *e)
    {
        isEnabled() ? m_pState->leaveEvent(e) : m_pDisableState->leaveEvent(e);
    };

    void CBtnSmart::setNormalState()
    {
        m_pState = m_pNormalState;
    };

    void CBtnSmart::setDisableState()
    {
        m_pState = m_pDisableState;
    };

    void CBtnSmart::setCheckState()
    {
        m_pState = m_pCheckState;
    };

    void CBtnSmart::paintEvent(QPaintEvent *event)
    {
        QPainter painter(this);
        painter.drawPixmap(0, 0, m_pCurrentBitmap->scaled(this->size()));
        painter.drawText(rect(), Qt::AlignCenter, text());
    }
}