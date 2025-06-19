#include "ledLab.h"
#include "btnSmart/btnSmart.h"
#include "controlDef.h"
#include <QPainter>
#include <QPainterPath>
namespace TIGER_UI_CONTROL
{
    CLedLab::CLedLab(QWidget *parent) : QLabel(parent)
    {
        setFont(cFont);
        setAttribute(Qt::WA_TranslucentBackground);
        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    }

    CLedLab::~CLedLab()
    {
        qDeleteAll(m_pButtons);
    }

    void CLedLab::paintEvent(QPaintEvent *event)
    {
        QPainter paint;
        paint.begin(this);
        paint.setRenderHint(QPainter::Antialiasing);
        QPainterPath path;
        path.addRoundedRect(this->rect(), 5, 5);
        // Qt默认颜色
        paint.fillPath(path, QColor(240, 240, 240));
        paint.setPen(QPen(QColor(128, 128, 128), 2));
        paint.drawPath(path);
        QLabel::paintEvent(event);
    }

    void CLedLab::setBtns(bool p_isNormal)
    {
        for (auto pButton : m_pButtons)
        {
            p_isNormal ? pButton->setNormal() : pButton->setDisable();
        }
    }

    void CLedLab::setOnlyCheckBtn(int id)
    {
        setCheckBtns(QList<int>() << id);
    }

    void CLedLab::setCheckBtns(QList<int> ids)
    {
        setBtns(false);
        for (auto id : ids)
        {
            m_pButtons[id]->setCheck();
        }
    }
};
