#include "my_qt_serial_port.hpp"
#include "include_qt.hpp"
#include "../../gps_framework.hpp"
#include <sstream>
#include <QMetaEnum>
#include <QSerialPortInfo>

#include "environnement.hpp"

#define HADRIEN_TIME_VOLANT 20
#define FILE_TIME 200

MyQTSerialPorts::MyQTSerialPorts(){
    connect(&m_serialPortGps, SIGNAL(readyRead()), this, SLOT(handleReadyReadGps()));
    connect(&m_serialPortGps, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
            this, &MyQTSerialPorts::handleErrorGps);
    connect(&m_serialPortPilot, SIGNAL(readyRead()), this, SLOT(handleReadyReadPilot()));
    connect(&m_serialPortPilot, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
            this, &MyQTSerialPorts::handleErrorPilot);
    
    connect(&m_timerHadrien, SIGNAL(timeout()), this, SLOT(handleHadrien()));
      
}
void MyQTSerialPorts::initOrLoad(Config & config){
    DEBUG("begin");
    INFO(m_gps_serial_input << " " << config.m_input_gps);
    m_pilot_langage = config.m_pilot_langage;
    
    if(config.m_input_gps != "none" && config.m_input_gps != "file"){
        if(m_gps_serial_input == config.m_input_gps && m_serialPortGps.isOpen()){
            INFO("gps port already open");
        } else {
            if(m_serialPortGps.isOpen()){
                m_serialPortGps.close();
            }
            m_gps_serial_input = config.m_input_gps;
            m_serialPortGps.setPortName(QString::fromStdString(m_gps_serial_input));
            m_serialPortGps.setBaudRate(config.m_baudrate_gps);
            if (!m_serialPortGps.open(QIODevice::ReadOnly)) {
                std::ostringstream oss;
                oss << "Failed to open gps port " << m_gps_serial_input << ", error:" << m_serialPortGps.errorString().toUtf8().constData();
                GpsFramework::Instance().addError(oss.str());//standardOutput << QObject::tr("Failed to open port %1, error: %2").arg(serialPortName).arg(serialPort.errorString()) << endl;//
            }
        }
    }
    
    if(config.m_inputPilot != "none"){
        if(m_pilot_serial_input == config.m_inputPilot && m_serialPortPilot.isOpen()){
            INFO("pilot port already open");
        } else {
            //INFO("ici");
            if(m_serialPortPilot.isOpen()){
                m_serialPortPilot.close();
            }
            m_serialPortPilot.close();
            m_pilot_serial_input = config.m_inputPilot;
            m_serialPortPilot.setPortName(QString::fromStdString(m_pilot_serial_input));
            m_serialPortPilot.setBaudRate(config.m_baudratePilot);
            if (!m_serialPortPilot.open(QIODevice::ReadWrite)) {
                std::ostringstream oss;
                oss << "Failed to open pilot port " << m_pilot_serial_input << ", error:" << m_serialPortPilot.errorString().toUtf8().constData();
                GpsFramework::Instance().addError(oss.str());
                //standardOutput << QObject::tr("Failed to open port %1, error: %2").arg(serialPortName).arg(serialPort.errorString()) << endl;//
            }
        }
    }
    m_timerHadrien.stop();
    m_timerHadrien.start(HADRIEN_TIME_VOLANT);
    DEBUG("end");
};

void MyQTSerialPorts::handleReadyReadGps(){
    DEBUG("begin");
    std::string s = "";
    QByteArray a = m_serialPortGps.readAll();
    for(size_t i = 0; i < a.size(); ++i){
        
        GpsFramework::Instance().m_gpsModule.readChar(a.data()[i]);
        s += ((char)(a.data()[i]));
    }
    DEBUG(s);
    DEBUG("end");
}
void MyQTSerialPorts::handleErrorGps(QSerialPort::SerialPortError error){
    DEBUG("begin");
    if(error != 0){
        std::ostringstream oss;
        oss << "handleErrorGps " << error << ", error:" << m_serialPortGps.errorString().toUtf8().constData();
        GpsFramework::Instance().addError(oss.str());
        WARN(error);
    }
    DEBUG("end");
}

void MyQTSerialPorts::handleReadyReadPilot(){
    DEBUG("begin");
    QByteArray b = m_serialPortPilot.readAll();
    if(m_pilot_langage == PILOT_LANGAGE_HADRIEN)
    {
        QString hex(b.toHex());
        std::string s = hex.toUtf8().constData();
        
        GpsFramework::Instance().addLog(s);
        
        char * data = b.data();
        GpsFramework::Instance().m_pilotModule.parseHadrienVolant(data);
        
        
    } else {
        QString hex(b);
        std::string s = (hex.toUtf8().constData());
        
        GpsFramework::Instance().m_pilotModule.arduinoParse(s);
    }
    DEBUG("end");
}
void MyQTSerialPorts::handleErrorPilot(QSerialPort::SerialPortError error){
    DEBUG("begin");
    if(error != 0){
        std::ostringstream oss;
        //auto error_s = std::string(QMetaEnum::fromType<QSerialPort::SerialPortError>().valueToKey(error));
        auto error_s = "fix";
        oss << "handleErrorPilot " << error << " " << error_s << ", error:" << m_serialPortPilot.errorString().toUtf8().constData();
        GpsFramework::Instance().addError(oss.str());
        WARN(error);
    }
    DEBUG("end");
}

void MyQTSerialPorts::writePilotSerialD(std::vector<unsigned char> & l){
    DEBUG("begin");
    QByteArray b;
    for(int i = 0; i < l.size(); ++i){
        b.append(l[i]);
    }
    m_serialPortPilot.write(b);
    QString hex(b.toHex());
    GpsFramework::Instance().addLog(hex.toUtf8().constData());
    //INFO("toto");
    //INFO(hex.toUtf8().constData());
    //INFO("toto");
    
    DEBUG("end");
}

void MyQTSerialPorts::writePilotSerialDAndWait(std::vector<unsigned char> & l){
    DEBUG("begin");
    writePilotSerialD(l);
    
    DEBUG("end");
}

void MyQTSerialPorts::writePilotSerialS(const std::string & l){
    DEBUG("begin");
    QByteArray b;
    b.append(l.c_str());
    m_serialPortPilot.write(b);
    DEBUG("end");
}


#include <boost/algorithm/string.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

std::string execute3(std::string cmd){
    std::string file = ProjectSourceBin + "/tmp_cmd";
    std::string cmd2 = cmd + " > " + file;
    system(cmd2.c_str());
    std::ifstream infile(file);
    std::stringstream strStream;
    strStream << infile.rdbuf();//read the file
    std::string res = strStream.str();
    return res;
}

void MyQTSerialPorts::addSerialPorts(std::string s){
    std::string res = execute3(s);
    std::vector<std::string> strs;
    boost::split(strs, res, boost::is_any_of("\n"));
    for(auto s : strs){
        if(!s.empty()){
            INFO(s);
            m_serials.push_back(s);
        }
    }
}

std::vector<std::string> & MyQTSerialPorts::getAvailablePorts(){
    m_serials.clear();
    addSerialPorts("ls /dev/cu.*");
    addSerialPorts("ls /dev/ttyACM*");
    addSerialPorts("ls /dev/ttymxc*");
    addSerialPorts("ls /dev/ttyUSB*");
    return m_serials;
}


void MyQTSerialPorts::handleHadrien(){
    DEBUG("begin");
    //INFO("coucou je suis ici");
    
    if(m_pilot_langage == PILOT_LANGAGE_HADRIEN){
        GpsFramework::Instance().m_pilotModule.handleHadrien();
        //GpsFramework::Instance().m_pilotModule.update();
    } else {
        //GpsFramework::Instance().m_pilotModule.update();
    }
    DEBUG("end");
    
}
