#pragma once
#include <QLabel>
#include "system/basic.h"
enum CPlcColis : unsigned char
{
    cpcReady = 0,
    cpcSpreadEnd,
    cpcSpreading,
    cpcTemp4,
    cpcTemp5,
    cpcTemp6,
    cpcTemp7,
    cpcTemp8,
    cpcTemp9,
    cpcTemp10,
    cpcMax
};

enum CPcDiscreteInputs : unsigned char
{
    cpdcStartSpread = 0,
    cpdcStartMark,
    cpdcTemp3,
    cpdcTemp4,
    cpdcTemp5,
    cpdcTemp6,
    cpdcTemp7,
    cpdcTemp8,
    cpdcTemp9,
    cpdcTemp10,
    cpdcMax
};

enum CPcInputRegisters : unsigned char
{
    cpdTemp1 = 0,
    cpdTemp2,
    cpdTemp3,
    cpdTemp4,
    cpdTemp5,
    cpdTemp6,
    cpdTemp7,
    cpdTemp8,
    cpdTemp9,
    cpdTemp10,
    cpdMax
};

enum CPlcHoldRegisters : unsigned char
{
    cphTemp1 = 0,
    cphTemp2,
    cphTemp3,
    cphTemp4,
    cphTemp5,
    cphTemp6,
    cphTemp7,
    cphTemp8,
    cphTemp9,
    cphTemp10,
    cphMax
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
    bool plcColis[cpcMax] = {0};
    bool pcDiscreteInputs[cpdcMax] = {0};
    quint16 pcInputRegisters[cpdMax] = {0};
    quint16 plcHoldRegisters[cphMax] = {0};
};

CPLCServerData *plcServerData();
