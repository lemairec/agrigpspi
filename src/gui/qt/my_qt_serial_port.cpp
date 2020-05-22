#include "my_qt_serial_port.hpp"
#include "include_qt.hpp"
#include "../../gps_framework.hpp"
#include <sstream>
#include <QMetaEnum>
MyQTSerialPorts::MyQTSerialPorts(){
    connect(&m_serialPortGps, SIGNAL(readyRead()), this, SLOT(handleReadyReadGps()));
    connect(&m_serialPortGps, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
            this, &MyQTSerialPorts::handleErrorGps);
    connect(&m_serialPortPilot, SIGNAL(readyRead()), this, SLOT(handleReadyReadPilot()));
    connect(&m_serialPortPilot, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
            this, &MyQTSerialPorts::handleErrorPilot);
}
void MyQTSerialPorts::initOrLoad(Config & config){
    INFO(m_gps_serial_input << " " << config.m_input);
    if(m_gps_serial_input != config.m_input && config.m_input != "none"){
        //INFO("ici");
        if(m_serialPortGps.isOpen()){
            m_serialPortGps.close();
        }
        m_gps_serial_input = config.m_input;
        m_serialPortGps.setPortName(QString::fromStdString(m_gps_serial_input));
        m_serialPortGps.setBaudRate(config.m_baudrate);
        if (!m_serialPortGps.open(QIODevice::ReadOnly)) {
            std::ostringstream oss;
            oss << "Failed to open gps port " << m_gps_serial_input << ", error:" << m_serialPortGps.errorString().toUtf8().constData();
            GpsFramework::Instance().addError(oss.str());//standardOutput << QObject::tr("Failed to open port %1, error: %2").arg(serialPortName).arg(serialPort.errorString()) << endl;//
        }
    }
    
    if(m_pilot_serial_input != config.m_inputPilot && config.m_inputPilot != "none"){
        //INFO("ici");
        if(m_serialPortPilot.isOpen()){
            m_serialPortPilot.close();
        }
        m_serialPortPilot.close();
        m_pilot_serial_input = config.m_inputPilot;
        m_serialPortPilot.setPortName(QString::fromStdString(m_pilot_serial_input));
        m_serialPortPilot.setBaudRate(config.m_baudrate);
        if (!m_serialPortPilot.open(QIODevice::ReadWrite)) {
            std::ostringstream oss;
            oss << "Failed to open pilot port " << m_pilot_serial_input << ", error:" << m_serialPortGps.errorString().toUtf8().constData();
            GpsFramework::Instance().addError(oss.str());
            //standardOutput << QObject::tr("Failed to open port %1, error: %2").arg(serialPortName).arg(serialPort.errorString()) << endl;//
        }
    }
    
};

void MyQTSerialPorts::handleReadyReadGps(){
    QByteArray a = m_serialPortGps.readAll();
    for(size_t i = 0; i < a.size(); ++i){
        GpsFramework::Instance().m_gpsModule.readChar(a.data()[i]);
    }
}
void MyQTSerialPorts::handleErrorGps(QSerialPort::SerialPortError error){
    if(error != 0){
        std::ostringstream oss;
        oss << "handleErrorGps " << error << ", error:" << m_serialPortGps.errorString().toUtf8().constData();
        GpsFramework::Instance().addError(oss.str());
        WARN(error);
    }
}

void MyQTSerialPorts::handleReadyReadPilot(){
    QByteArray a = m_serialPortPilot.readAll();
    for(size_t i = 0; i < a.size(); ++i){
        INFO(a.data()[i]);
    }
}
void MyQTSerialPorts::handleErrorPilot(QSerialPort::SerialPortError error){
    if(error != 0){
        std::ostringstream oss;
        auto error_s = std::string(QMetaEnum::fromType<QSerialPort::SerialPortError>().valueToKey(error));
        oss << "handleErrorPilot " << error << " " << error_s << ", error:" << m_serialPortGps.errorString().toUtf8().constData();
        GpsFramework::Instance().addError(oss.str());
        WARN(error);
    }
}

void MyQTSerialPorts::writePilotSerialD(std::vector<unsigned char> & l){
    QByteArray b;
    for(int i = 0; i < l.size(); ++i){
        b.append(l[i]);
    }
    m_serialPortPilot.write(b);
    QString hex(b.toHex());
    INFO("toto");
    INFO(hex.toUtf8().constData());
    INFO("toto");
    
}
void MyQTSerialPorts::writePilotSerialS(const std::string & l){
    QByteArray b;
    b.append(l.c_str());
    m_serialPortPilot.write(b);
}


