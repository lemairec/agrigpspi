#include "my_qt_serial_port.hpp"
#include "include_qt.hpp"
#include "../../gps_framework.hpp"
#include <sstream>
#include <QMetaEnum>
#include <QSerialPortInfo>

#include "../gps_widget.hpp"

#include "environnement.hpp"
#include "../../util/directory_manager.hpp"

MyQTSerialPorts::MyQTSerialPorts(){
    connect(&m_serialPortGps, SIGNAL(readyRead()), this, SLOT(handleReadyReadGps()));
    connect(&m_serialPortGps, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
            this, &MyQTSerialPorts::handleErrorGps);
    connect(&m_serialPortPilot, SIGNAL(readyRead()), this, SLOT(handleReadyReadPilot()));
    connect(&m_serialPortPilot, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
            this, &MyQTSerialPorts::handleErrorPilot);
    connect(&m_serialPortImu, SIGNAL(readyRead()), this, SLOT(handleReadyReadImu()));
    connect(&m_serialPortImu, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
            this, &MyQTSerialPorts::handleErrorImu);
    
    connect(&m_timerPilot, SIGNAL(timeout()), this, SLOT(handlePilot()));
      
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
            if (!m_serialPortGps.open(QIODevice::ReadWrite)) {
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
                oss << "Failed to open pilot \nport " << m_pilot_serial_input << "\nerror:\n" << m_serialPortPilot.errorString().toUtf8().constData();
                GpsFramework::Instance().m_pilot_last_error = oss.str();
                GpsFramework::Instance().addError(oss.str());
                //standardOutput << QObject::tr("Failed to open port %1, error: %2").arg(serialPortName).arg(serialPort.errorString()) << endl;//
            }
        }
    }
    
    if(config.m_imu_input != "none"){
        if(m_imu_serial_input == config.m_imu_input && m_serialPortImu.isOpen()){
            INFO("imu port already open");
        } else {
            //INFO("ici");
            if(m_serialPortImu.isOpen()){
                m_serialPortImu.close();
            }
            m_serialPortImu.close();
            m_imu_serial_input = config.m_imu_input;
            m_serialPortImu.setPortName(QString::fromStdString(m_imu_serial_input));
            m_serialPortImu.setBaudRate(config.m_imu_baudrate);
            if (!m_serialPortImu.open(QIODevice::ReadWrite)) {
                std::ostringstream oss;
                oss << "Failed to open imu port " << m_imu_serial_input << ", error:" << m_serialPortImu.errorString().toUtf8().constData();
                GpsFramework::Instance().addError(oss.str());
                //standardOutput << QObject::tr("Failed to open port %1, error: %2").arg(serialPortName).arg(serialPort.errorString()) << endl;//
            }
        }
    }
    m_timerPilot.stop();
    m_timerPilot.start(1000/config.m_pilot_frequence);
    DEBUG("end");
};

void MyQTSerialPorts::closeAll(){
    INFO("###close all");
    if(m_serialPortGps.isOpen()){
        INFO("close gps");
        m_serialPortGps.close();
    }
    if(m_serialPortImu.isOpen()){
        INFO("close imu");
        m_serialPortImu.close();
    }
    if(m_serialPortPilot.isOpen()){
        INFO("close pilot");
        m_serialPortPilot.close();
    }
}

void MyQTSerialPorts::handleReadyReadGps(){
    DEBUG("begin");
    std::string s = "";
    QByteArray a = m_serialPortGps.readAll();
    for(int i = 0; i < (int)a.size(); ++i){
        
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
        GpsFramework::Instance().m_pilot_last_error = oss.str();
        //GpsFramework::Instance().addError(oss.str());
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
        //GpsFramework::Instance().addError(oss.str());
        WARN(error);
    }
    DEBUG("end");
}

/**
 IMU
 */

void MyQTSerialPorts::handleReadyReadImu(){
    DEBUG("begin");
    QByteArray b = m_serialPortImu.readAll();
    QString hex(b.toHex());
    std::string s = hex.toUtf8().constData();
    
    char * data = b.data();
    INFO(" " << b.size());
    GpsFramework & f = GpsFramework::Instance();
    for(int i = 0; i < b.size(); ++i){
        int j = data[i];
        f.m_imuModule.addIChar(j);
        //std::cout << std::hex << j << " ";
    }
    std::cout << std::endl;
    DEBUG("end");
}
void MyQTSerialPorts::handleErrorImu(QSerialPort::SerialPortError error){
    DEBUG("begin");
    if(error != 0){
        std::ostringstream oss;
        //auto error_s = std::string(QMetaEnum::fromType<QSerialPort::SerialPortError>().valueToKey(error));
        auto error_s = "fix";
        oss << "handleErrorPilot " << error << " " << error_s << ", error:" << m_serialPortPilot.errorString().toUtf8().constData();
        //GpsFramework::Instance().addError(oss.str());
        WARN(error);
    }
    DEBUG("end");
}



void MyQTSerialPorts::writeGpsSerialS(const std::string & l){
    if(m_serialPortGps.isOpen()){
        QByteArray b;
        b.append(l.c_str());
        m_serialPortGps.write(b);
    }
}


void MyQTSerialPorts::writePilotSerialD(std::vector<unsigned char> & l){
    DEBUG("begin");
    if(m_serialPortPilot.isOpen()){
        QByteArray b;
        for(long unsigned int i = 0; i < l.size(); ++i){
            b.append(l[i]);
        }
        m_serialPortPilot.write(b);
        QString hex(b.toHex());
    }
    
    DEBUG("end");
}

void MyQTSerialPorts::writePilotSerialDAndWait(std::vector<unsigned char> & l){
    DEBUG("begin");
    writePilotSerialD(l);
    
    DEBUG("end");
}

void MyQTSerialPorts::writePilotSerialS(const std::string & l){
    DEBUG("begin");
    if(m_serialPortPilot.isOpen()){
        QByteArray b;
        b.append(l.c_str());
        m_serialPortPilot.write(b);
    }
    DEBUG("end");
}
#include <QThread>
int i = 0;
void MyQTSerialPorts::rechercheAuto(){
    setRecherche();
//    GCVBHÙDÈ
    
}

void MyQTSerialPorts::setRecherche(){
    INFO("##0 open");
    closeAll();
    GpsFramework & f = GpsFramework::Instance();
    f.m_config.m_input_gps = "none";
    f.m_config.m_inputPilot = "none";
    f.initOrLoadConfig();
    auto serials = getAvailablePorts();
    m_serial_searchs.clear();
    for(auto s : serials){
        INFO("##0 " << s);
        auto serial_search = new SerialSearch();
        serial_search->m_serial_s = s;
        serial_search->m_serial_port.setPortName(QString::fromStdString(s));
        serial_search->m_serial_port.setBaudRate(115200);
        if (!serial_search->m_serial_port.open(QIODevice::ReadWrite)) {
            INFO("##0 " << "failed");
             std::ostringstream oss;
            oss << "Failed to open imu port " << s << ", error:" << serial_search->m_serial_port.errorString().toUtf8().constData();
            GpsFramework::Instance().addError(oss.str());
            //standardOutput << QObject::tr("Failed to open port %1, error: %2").arg(serialPortName).arg(serialPort.errorString()) << endl;//
        }
        m_serial_searchs[s] = serial_search;
        
    }
    m_serial_searchs_i = 1;
}

void MyQTSerialPorts::ecrireRecherche(){
    INFO("##1 ecrire");
    for(auto s : m_serial_searchs){
        auto serial_search = s.second;
        
        std::string s2 = "$P;\n$P;\n";
        QByteArray b;
        b.append(s2.c_str());
        serial_search->m_serial_port.write(b);
    }
    
}
void MyQTSerialPorts::analyseRecherche(){
    INFO("##2 analyse");
    std::string gps_port = "none";
    std::string pilot_port = "none";
    for(auto s : m_serial_searchs){
        auto serial_search = s.second;
        INFO(s.first << " " << serial_search->m_serial_port.isOpen());
        QByteArray b = serial_search->m_serial_port.readAll();
        QString hex(b);
        std::string data_s = hex.toUtf8().constData();
        if(data_s.size()>2){
            if(data_s.find("$GNGGA") !=std::string::npos){
                gps_port = serial_search->m_serial_s;
            } else if(data_s.find("$GPGGA") !=std::string::npos){
                gps_port = serial_search->m_serial_s;
            } else if(data_s.find("$P,") !=std::string::npos){
                pilot_port = serial_search->m_serial_s;
            } else {
                INFO(data_s);
            }
        }
        QByteArray b2 = serial_search->m_serial_port.readAll();
        
        INFO(data_s);
        INFO("toto " << b2.constData());
        s.second->m_serial_port.close();
    }
    
    
    INFO("##2 fin recherche");
    INFO("## gps_port " << gps_port);
    INFO("## pilot_port " << pilot_port);
    GpsFramework & f = GpsFramework::Instance();
    f.m_config.m_input_gps = gps_port;
    f.m_config.m_inputPilot = pilot_port;
    f.initOrLoadConfig();
    
}


#include <boost/algorithm/string.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

std::string execute3(std::string cmd){
    std::string file = DirectoryManager::Instance().getBinDirectory() + "/tmp_cmd";
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
    addSerialPorts("ls /dev/ttyS*");

    return m_serials;
}


void MyQTSerialPorts::handlePilot(){
    DEBUG("begin");
    //INFO("coucou je suis ici");
    
    if(m_pilot_langage == PILOT_LANGAGE_HADRIEN){
        GpsFramework::Instance().m_pilotModule.handleHadrien();
        //GpsFramework::Instance().m_pilotModule.update();
    } else {
        GpsFramework::Instance().m_pilotModule.handleArduino();
    }
    DEBUG("end");
    
    if(m_serial_searchs_i > 0){
        m_serial_searchs_i++;
        INFO(m_serial_searchs_i);
        if(m_serial_searchs_i == 500){
            ecrireRecherche();
        }
        if(m_serial_searchs_i > 1000){
            analyseRecherche();
            GpsWidget::Instance()->m_menuWidget.m_close = true;
            m_serial_searchs_i = 0;
            
        }
    }
}
