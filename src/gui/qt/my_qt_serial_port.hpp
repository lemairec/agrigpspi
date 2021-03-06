#ifndef SERIAL_QT_H
#define SERIAL_QT_H

#include <QtSerialPort/QSerialPort>

#include "../../config/config.hpp"
#include <QTextStream>
#include <QByteArray>
#include <QObject>
#include <QTimer>

QT_USE_NAMESPACE

struct SerialSearch {
    QSerialPort m_serial_port;
    std::string m_serial_s;
};

class MyQTSerialPorts : public QObject{
    Q_OBJECT
    
    std::string m_gps_serial_input;
    std::string m_pilot_serial_input;
    std::string m_imu_serial_input;
    
    QSerialPort m_serialPortGps;
    QSerialPort m_serialPortPilot;
    QSerialPort m_serialPortImu;

    std::vector<std::string> m_serials;
    std::map<std::string, SerialSearch *> m_serial_searchs;
    
    int m_pilot_langage = PILOT_LANGAGE_ARDUINO;
public:
    int m_serial_searchs_i = 0;
    
    MyQTSerialPorts();
    
    void initOrLoad(Config & config);
    void closeAll();
    
    void writeGpsSerialS(const std::string & l);
    
    bool pilotIsOpen(){return m_serialPortPilot.isOpen();};
    void writePilotSerialD(std::vector<unsigned char> & l);
    void writePilotSerialDAndWait(std::vector<unsigned char> & l);
    void writePilotSerialS(const std::string & l);
    
    bool imuIsOpen(){return m_serialPortImu.isOpen();};
    
    void rechercheAuto();
    void setRecherche();
    void ecrireRecherche();
    void analyseRecherche();
    
    void addSerialPorts(std::string s);
    std::vector<std::string> & getAvailablePorts();
private slots:
    void handleReadyReadGps();
    void handleErrorGps(QSerialPort::SerialPortError error);
    
    void handleReadyReadPilot();
    void handleErrorPilot(QSerialPort::SerialPortError error);

    void handleReadyReadImu();
    void handleErrorImu(QSerialPort::SerialPortError error);

private:
    QTimer m_timerPilot;
private slots:
    
    void handlePilot();
};


#endif
