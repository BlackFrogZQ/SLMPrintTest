#include "plcSigDef.h"

CPLCServerData* plcServerData()
{
    static CPLCServerData gMasterData;
    return &gMasterData;
}

bool CPLCServerData::connected() const
{
    return isConnected;
}

void CPLCServerData::setConnectState(bool p_isConnected)
{
    emit sigPlcConnect(isConnected = p_isConnected);
}

bool CPLCServerData::colis(CPlcColis p_id) const
{
    return plcColis[p_id];
}

bool CPLCServerData::disColis(CPcDisColis p_id) const
{
    return pcDisColis[p_id];
}

quint16 CPLCServerData::hold(CPcHold p_id) const
{
    return pcHold[p_id];
}