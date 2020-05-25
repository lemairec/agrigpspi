#ifndef SERIAL_QT_H
#define SERIAL_QT_H

#include <QtSerialPort/QSerialPort>

#include "../../config/config.hpp"
#include <QTextStream>
#include <QByteArray>
#include <QObject>

QT_USE_NAMESPACE

class MyQTSerialPorts : public QObject{
    Q_OBJECT
    
    std::string m_gps_serial_input;
    std::string m_pilot_serial_input;
    
    QSerialPort m_serialPortGps;
    QSerialPort m_serialPortPilot;

    std::vector<std::string> m_serials;
    
public:
    MyQTSerialPorts();
    
    void initOrLoad(Config & config);
    void open();
    
    void writePilotSerialD(std::vector<unsigned char> & l);
    void writePilotSerialS(const std::string & l);
    
    void addSerialPorts(std::string s);
    std::vector<std::string> & getAvailablePorts();
private slots:
    void handleReadyReadGps();
    void handleErrorGps(QSerialPort::SerialPortError error);
    
    void handleReadyReadPilot();
    void handleErrorPilot(QSerialPort::SerialPortError error);

    
};


#endif
