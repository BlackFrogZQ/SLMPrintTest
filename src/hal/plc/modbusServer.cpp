#include "modbusServer.h"
#include "plcSigDef.h"
#include "system/systemService.h"
#include "system/basic.h"
#include <QModbusTcpServer>
#include <QTimer>

CModbusServer::CModbusServer(QObject *p) : QObject(p), m_pServer(nullptr), m_isConnected(plcServerData()->isConnected), m_reConnectTimes(0)
{
    assert(m_isConnected == false);
    m_pServer = new QModbusTcpServer(this);
    QModbusDataUnitMap reg;
    reg.insert(QModbusDataUnit::Coils, {QModbusDataUnit::Coils, 0, 10});
    reg.insert(QModbusDataUnit::DiscreteInputs, {QModbusDataUnit::DiscreteInputs, 0, 10});
    reg.insert(QModbusDataUnit::InputRegisters, {QModbusDataUnit::InputRegisters, 0, 10});
    reg.insert(QModbusDataUnit::HoldingRegisters, {QModbusDataUnit::HoldingRegisters, 0, 10});
    m_pServer->setMap(reg);

    connect(m_pServer, &QModbusTcpServer::dataWritten, this, &CModbusServer::dataChanged);
    connect(m_pServer, &QModbusTcpServer::stateChanged, this, [this](QModbusDevice::State state)
            { m_isConnected = (m_pServer->state() != QModbusDevice::UnconnectedState); });
    connect(m_pServer, &QModbusTcpServer::errorOccurred, this, [this]
            { myDebug << "modbusMaster error:" << m_pServer->errorString(); });
}

CModbusServer::~CModbusServer()
{
    if (m_pServer)
        m_pServer->disconnectDevice();
    delPtr(m_pServer);
}

bool CModbusServer::sendDiscreteInputs(int p_addr, bool p_value)
{
    plcServerData()->pcDiscreteInputs[p_addr] = (p_value != 0);
    return writeData(QModbusDataUnit::DiscreteInputs, p_addr, QList<quint16>() << p_value);
}

bool CModbusServer::sendInputRegisters(int p_addr, quint16 p_value)
{
    plcServerData()->plcHoldRegisters[p_addr] = p_value;
    return writeData(QModbusDataUnit::InputRegisters, p_addr, QList<quint16>() << p_value);
}

bool CModbusServer::sendHoldRegisters(int p_addr, quint16 p_value)
{
    plcServerData()->plcHoldRegisters[p_addr] = p_value;
    return writeData(QModbusDataUnit::HoldingRegisters, p_addr, QList<quint16>() << p_value);
}

bool CModbusServer::readData(QModbusDataUnit::RegisterType p_type, int p_startAddr, QList<quint16> &p_readDatas)
{
    if (!m_isConnected)
    {
        return false;
    }

    for (size_t i = 0; i < p_readDatas.size(); i++)
    {
        if (!m_pServer->data(p_type, p_startAddr + i, &p_readDatas[i]))
        {
            return false;
        }
    }
    return true;
}

bool CModbusServer::writeData(QModbusDataUnit::RegisterType p_type, int p_startAddr, QList<quint16> p_readDatas)
{
    if (!m_isConnected)
    {
        return false;
    }

    for (size_t i = 0; i < p_readDatas.size(); i++)
    {
        if (!m_pServer->setData(p_type, p_startAddr + i, p_readDatas[i]))
        {
            return false;
        }
    }
    return true;
}

bool CModbusServer::listen(QString p_ip, quint16 p_port, quint8 p_masterAddr)
{
    if (m_isConnected)
    {
        m_pServer->disconnectDevice();
        m_isConnected = false;
    }
    m_pServer->setConnectionParameter(QModbusDevice::NetworkAddressParameter, p_ip);
    m_pServer->setConnectionParameter(QModbusDevice::NetworkPortParameter, p_port);
    m_pServer->setServerAddress(p_masterAddr);
    return m_pServer->connectDevice();
}

void CModbusServer::dataChanged(QModbusDataUnit::RegisterType table, int address, int size)
{
    for (size_t i = 0; i < size; i++)
    {
        const int addr = address + i;
        quint16 vaule = 0;
        m_pServer->data(table, addr, &vaule);
        switch (table)
        {
        case QModbusDataUnit::Coils:
            assert(addr >= 0 && addr < cpcMax);
            plcServerData()->plcColis[addr] = (vaule != 0);
            break;
        case QModbusDataUnit::DiscreteInputs:
            assert(addr >= 0 && addr < cpdcMax);
            plcServerData()->pcDiscreteInputs[addr] = (vaule != 0);
            break;
        case QModbusDataUnit::HoldingRegisters:
            assert(addr >= 0 && addr < cphMax);
            plcServerData()->plcHoldRegisters[addr] = vaule;
            break;
        default:
            break;
        }
    }
    emit sigDataUpdate();
}

void CModbusServer::setPlcIP(CPlcIP p_ips)
{
    m_ip = p_ips;
}

void CModbusServer::connectPlc()
{
    if (m_isConnected)
    {
        return;
    };
    emit sigConnect();
}

void CModbusServer::slotConnectDevice()
{
    if (m_isConnected)
    {
        return;
    };

    m_pServer->setConnectionParameter(QModbusDevice::NetworkAddressParameter, m_ip.ip);
    m_pServer->setConnectionParameter(QModbusDevice::NetworkPortParameter, m_ip.ipPort);
    m_pServer->setServerAddress(m_ip.ipMAC);
    if(m_pServer->connectDevice())
    {
        emit sigConnected();
        return;
    }
    emit sigDisconnected();
}

void CModbusServer::slotConnected()
{
    m_isConnected = true;
    plcServerData()->setConnectState(m_isConnected);
    myInfo << cnStr("%1(%2:%3:%4)已连接").arg(m_ip.name).arg(m_ip.ip).arg(m_ip.ipPort).arg(m_ip.ipMAC);
    m_reConnectTimes = 0;
}

void CModbusServer::slotDisconnected()
{
    m_isConnected = false;
    plcServerData()->setConnectState(m_isConnected);
    if (m_reConnectTimes % 10 == 0)
    {
        myInfo << cnStr("%1(%2:%3:%4)已断开连接").arg(m_ip.name).arg(m_ip.ip).arg(m_ip.ipPort).arg(m_ip.ipMAC) + cnStr(".尝试第%1次重连").arg((m_reConnectTimes == 0) ? 1 : m_reConnectTimes);
    }
    m_reConnectTimes++;
    QTimer::singleShot(cReconnectInterval, this, &CModbusServer::slotConnectDevice);
}