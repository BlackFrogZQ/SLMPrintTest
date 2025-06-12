#pragma once
#include <QObject>
#include <QModbusDataUnit>
#include <QThread>

class QModbusTcpServer;
class IPlcController;
constexpr int cReconnectInterval = 1000;   // 硬件掉线后重连时间

class CModbusServer:public QObject
{
    friend class IPlcController;
    Q_OBJECT
public:
    CModbusServer(QObject* p = nullptr);
    ~CModbusServer();
    bool sendDiscreteInputs(int p_addr,bool p_value);
    bool sendInputRegisters(int p_addr,quint16 p_value);
    bool sendHoldRegisters(int p_addr,quint16 p_value);
    bool readData(QModbusDataUnit::RegisterType p_type,int p_startAddr,QList<quint16>& p_readDatas);
    bool listen(QString p_ip = "127.0.0.1",quint16 p_port = 502,quint8 p_masterAddr = 0xff);

    struct CPlcIP
    {
        QString name;
        QString ip;
        int ipPort;
        int ipMAC;
        CPlcIP(QString p_name = "", QString p_ip = "", int p_ipPort = 502 ,int p_mac = 100)
            : name(p_name),
              ip(p_ip),
              ipPort(p_ipPort),
              ipMAC(p_mac) {}
    };
    void setPlcIP(CPlcIP p_ips);
    void connectPlc();

signals:
    void sigDataUpdate();
    void sigConnect();
    void sigConnected();
    void sigDisconnected();

protected slots:
    void dataChanged(QModbusDataUnit::RegisterType table, int address, int size);
    void slotConnectDevice();
    void slotConnected();
    void slotDisconnected();

protected:
    bool writeData(QModbusDataUnit::RegisterType p_type,int p_startAddr,QList<quint16> p_readDatas);

protected:
    QModbusTcpServer *m_pServer;
    bool& m_isConnected;
    CPlcIP m_ip;
    uint32_t m_reConnectTimes;
};

class IPlcController : public QObject
{
    Q_OBJECT
    QThread workerThread;

public:
    IPlcController(CModbusServer *p_pMegatron)
    {
        p_pMegatron->moveToThread(&workerThread);
        connect(p_pMegatron, &CModbusServer::sigConnect, p_pMegatron, &CModbusServer::slotConnectDevice);
        connect(p_pMegatron, &CModbusServer::sigConnected, p_pMegatron, &CModbusServer::slotConnected);
        connect(p_pMegatron, &CModbusServer::sigDisconnected, p_pMegatron, &CModbusServer::slotDisconnected);
        workerThread.start();
    };
    ~IPlcController()
    {
        workerThread.quit();
        workerThread.wait();
    };
};