#include "my_qt_serial_port.hpp"
#include "include_qt.hpp"
#include "../../gps_framework.hpp"

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
    if(m_gps_serial_input != config.m_input){
        //INFO("ici");
        m_serialPortGps.close();
        m_gps_serial_input = config.m_input;
        m_serialPortGps.setPortName(QString::fromStdString(m_gps_serial_input));
        m_serialPortGps.setBaudRate(config.m_baudrate);
        if (!m_serialPortGps.open(QIODevice::ReadOnly)) {
            WARN("Failed to open port" << "p" << ", error:" << m_serialPortGps.errorString().toUtf8().constData());
            //standardOutput << QObject::tr("Failed to open port %1, error: %2").arg(serialPortName).arg(serialPort.errorString()) << endl;//
        }
    }
    
    if(m_pilot_serial_input != config.m_inputPilot){
        //INFO("ici");
        m_serialPortPilot.close();
        m_gps_serial_input = config.m_input;
        m_serialPortPilot.setPortName(QString::fromStdString(m_gps_serial_input));
        m_serialPortPilot.setBaudRate(config.m_baudrate);
        if (!m_serialPortPilot.open(QIODevice::ReadOnly)) {
            WARN("Failed to open port" << "p" << ", error:" << m_serialPortGps.errorString().toUtf8().constData());
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
    WARN(error);
}

void MyQTSerialPorts::handleReadyReadPilot(){
    QByteArray a = m_serialPortPilot.readAll();
    for(size_t i = 0; i < a.size(); ++i){
        INFO(a.data()[i]);
    }
}
void MyQTSerialPorts::handleErrorPilot(QSerialPort::SerialPortError error){
    WARN(error);
}

void MyQTSerialPorts::writePilotSerialD(std::vector<unsigned char> & l){
    
}
void MyQTSerialPorts::writePilotSerialS(const std::string & l){
}


