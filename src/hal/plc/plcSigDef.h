#pragma once
#include <QLabel>
#include "system/basic.h"
enum CPlcColis : unsigned char
{
    plccReady = 0,
    plccSpreading,
    plccSpreadEnd,
    plccTemp4,
    plccTemp5,
    plccTemp6,
    plccTemp7,
    plccTemp8,
    plccTemp9,
    plccTemp10,
    plccMax
};

enum CPcDiscreteInputs : unsigned char
{
    pcdReady = 0,
    pcdStartSpread,
    pcdStopSpread,
    pcdStartMark,
    pcdTemp5,
    pcdTemp6,
    pcdTemp7,
    pcdTemp8,
    pcdTemp9,
    pcdTemp10,
    pcdMax
};

enum CPcInputRegisters : unsigned char
{
    pciTemp1 = 0,
    pciTemp2,
    pciTemp3,
    pciTemp4,
    pciTemp5,
    pciTemp6,
    pciTemp7,
    pciTemp8,
    pciTemp9,
    pciTemp10,
    pciMax
};

enum CPlcHoldRegisters : unsigned char
{
    plchTemp1 = 0,
    plchTemp2,
    plchTemp3,
    plchTemp4,
    plchTemp5,
    plchTemp6,
    plchTemp7,
    plchTemp8,
    plchTemp9,
    plchTemp10,
    plchMax
};

class CPLCServerData : public QLabel
{
    friend class CModbusServer;
    Q_OBJECT

public:
    bool connected() const;
    void setConnectState(bool p_isConnected);
    bool colis(CPlcColis p_id) const;
    bool disDiscreteInputs(CPcDiscreteInputs p_id) const;
    quint16 inputRegisters(CPcInputRegisters p_id) const;
    quint16 holdRegisters(CPlcHoldRegisters p_id) const;

signals:
    void sigPlcConnect(bool p_isConnected);

protected:
    bool isConnected = false;
    bool plcColis[plccMax] = {0};
    bool pcDiscreteInputs[pcdMax] = {0};
    quint16 pcInputRegisters[pciMax] = {0};
    quint16 plcHoldRegisters[plchMax] = {0};
};

CPLCServerData *plcServerData();
