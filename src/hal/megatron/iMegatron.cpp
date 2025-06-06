#include "iMegatron.h"

namespace TIGER_Megatron
{
    IMegatron::IMegatron() : m_bConnected(false), m_ip(CMegatronIP()), m_reConnectTimes(0)
    {
    }

    IMegatron::~IMegatron()
    {
    }

    void IMegatron::setMegatronIP(CMegatronIP p_ips)
    {
        m_ip = p_ips;
    }

    void IMegatron::connectMegatron()
    {
        if (m_bConnected)
        {
            return;
        };
        emit sigConnect();
    }
}