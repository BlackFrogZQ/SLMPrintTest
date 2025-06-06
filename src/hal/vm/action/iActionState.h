#pragma once
#include <QObject>

namespace TIGER_VMSLM
{
    constexpr int cSenMessageInterval = 50; // 硬件状态刷新间隔
    class IActionState : public QObject
    {
        Q_OBJECT
    public:
        virtual ~IActionState();
        virtual void run() = 0;
        virtual void runing();

    protected:
        void changeState(IActionState* p_state);
    };
}