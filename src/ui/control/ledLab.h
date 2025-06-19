#pragma once
#include <QLabel>

namespace TIGER_UI_BtnSmart
{
    class CBtnSmart;
};
namespace TIGER_UI_CONTROL
{
    class CLedLab : public QLabel
    {
        Q_OBJECT
    public:
        CLedLab(QWidget *parent = nullptr);
        virtual ~CLedLab();

    protected:
        void paintEvent(QPaintEvent *) override;

    protected:
        void setBtns(bool p_isNormal);
        void setOnlyCheckBtn(int id);
        void setCheckBtns(QList<int> ids);

    protected:
        QVector<TIGER_UI_BtnSmart::CBtnSmart *> m_pButtons;
    };
};