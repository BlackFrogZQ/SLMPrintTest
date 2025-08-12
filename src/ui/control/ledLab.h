#pragma once
#include <QLabel>

namespace TIGER_UI_BtnSmart
{
    class CBtnSmart;
};
class CalibrateDialog;
namespace TIGER_UI_CONTROL
{
    class CLedLab : public QLabel
    {
        friend class CalibrateDialog;
        Q_OBJECT
    public:
        CLedLab(QWidget *parent = nullptr);
        virtual ~CLedLab();
        void setShowBackground(bool show) { m_showBackground = show; }
        void setBtns(bool p_isNormal);

    protected:
        void paintEvent(QPaintEvent *) override;

    protected:
        void setOnlyCheckBtn(int id);
        void setCheckBtns(QList<int> ids);

    protected:
        QVector<TIGER_UI_BtnSmart::CBtnSmart *> m_pButtons;
        bool m_showBackground;
    };
};