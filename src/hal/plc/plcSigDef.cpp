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

bool CPLCServerData::disDiscreteInputs(CPcDiscreteInputs p_id) const
{
    return pcDiscreteInputs[p_id];
}

quint16 CPLCServerData::inputRegisters(CPcInputRegisters p_id) const
{
    return pcInputRegisters[p_id];
}

quint16 CPLCServerData::holdRegisters(CPlcHoldRegisters p_id) const
{
    return plcHoldRegisters[p_id];
}