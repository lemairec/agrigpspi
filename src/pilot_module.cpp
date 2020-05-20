#include "pilot_module.hpp"
#include "serial.hpp"
#include <math.h>
#include "logging.hpp"


PilotModule::PilotModule(){
    //setRef(LAT_REF, LON_REF);
}

void PilotModule::initOrLoadConfig(Config & config){
    if(m_serial){
        delete(m_serial);
    }
    //m_algo_k = config.m_algo_k;
    
    m_serial = NULL;
    m_inverse = config.m_pilot_inverse;
    if(config.m_inputPilot != "none"){
        try {
            m_serial = new Serial(config.m_inputPilot,config.m_baudratePilot);
        } catch(boost::system::system_error& e)
        {
            WARN(config.m_input << " " << config.m_baudrate);
            m_serial = NULL;
        }
       
    }

    //m_version_guidage = m_serial->readString(12);
    
    clear();
    
}

void PilotModule::clear(){
    m_0 = 0;
    m_lastValue = 0;
    if(m_serial){
        m_serial->writeString("$C;\n");
    }
}

void PilotModule::run(double value){
    std::ostringstream out;
    
    if(m_algo2 == ALGO2_GOTO){
        int res = value*m_algo2_goto_k;
        if(m_inverse){
            res = -res;
        }
        if(res<0){
            out << "$G;-" << (-res) << "\n";
            
            
        } else {
            out << "$G; " << res << "\n";
        }
    } else if(m_algo2 == ALGO2_GOTO_REL){
        int res = (value-m_0)*m_algo2_goto_k;
        if(m_inverse){
            res = -res;
        }
        if(res<0){
            out << "$G;-" << (-res) << "\n";
            
            
        } else {
            out << "$G; " << res << "\n";
        }
        m_0 = (1-m_algo2_goto_rel)*m_0+m_algo2_goto_rel*value;
    } else if(m_algo2 == ALGO2_PID){
        int res = value*m_algo2_pid_p + (m_lastValue-value)*m_algo2_pid_d;
        if(m_inverse){
            res = -res;
        }
        if(res<0){
            out << "$L;" << (-res) << "\n";
        } else {
            out << "$R;" << res << "\n";
        }
        m_lastValue = value;
        
    }
    
    
    INFO(out.str());
    if(m_serial){
        m_serial->writeString(out.str());
    } else {
        WARN("###################serial");
    }
}

void PilotModule::test(int i){
    if(m_serial == NULL){
        return;
    }
    INFO("$R;1000\n");
    if(i<0){
        m_serial->writeString("$G; 1000\n");
    } else {
        m_serial->writeString("$G;-1000\n");
    }
}


/*
 qt
 m_serial = new QSerialPort();
 m_serial->setPortName("/dev/cu.usbserial-141240");
 m_serial->setBaudRate(QSerialPort::Baud115200);
 m_serial->setDataBits(QSerialPort::Data8);
 m_serial->setParity(QSerialPort::NoParity);
 m_serial->setStopBits(QSerialPort::OneStop);
 m_serial->setFlowControl(QSerialPort::NoFlowControl);
 
 if (m_serial->open(QIODevice::ReadWrite)) {

     INFO("Connected");
 //    QThread::msleep(1000);
 } else {

     INFO("Open error");
 }
 }
 
 if(value>0){
 QString str("#G;-100000\n");
 QByteArray br = str.toUtf8();
 m_serial->write(br);
 } else {
     QString str("#G; 100000\n");
     QByteArray br = str.toUtf8();
     m_serial->write(br);
     
 }
 m_serial->flush();
 
  m_serial->waitForBytesWritten(100);
  m_serial->waitForReadyRead(100);
 auto b = m_serial->readAll();
 
 
 INFO(m_serial->isOpen());
 INFO(m_serial->isWritable());
 
 INFO(b.size());
 QString myString(b);
 
 
 INFO("toto" << myString.toUtf8().constData());
 */
