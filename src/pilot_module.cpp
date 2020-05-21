#include "pilot_module.hpp"
#include "serial.hpp"
#include <math.h>
#include "logging.hpp"
#include <inttypes.h>


PilotModule::PilotModule(){
    //setRef(LAT_REF, LON_REF);
}

void PilotModule::initOrLoadConfig(Config & config){
    m_inverse = config.m_pilot_inverse;
    if(config.m_inputPilot != "none"){
        if(config.m_inputPilot != m_inputPilot){
            try {
                m_serial = NULL;
                if(m_serial){
                    delete(m_serial);
                }
                m_serial = new Serial(config.m_inputPilot,config.m_baudratePilot);
                m_inputPilot = config.m_inputPilot;
            } catch(boost::system::system_error& e)
            {
                WARN(config.m_input << " " << config.m_baudrate);
                m_serial = NULL;
            }
        }
       
    }
    m_algo2 = config.m_algo2;
    m_algo2_goto_k = config.m_algo2_goto_k;
    m_algo2_goto_rel_s = config.m_algo2_goto_rel_s;
    m_algo2_pid_p = config.m_algo2_pid_p;
    m_algo2_pid_d = config.m_algo2_pid_d;
    

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
        m_lastValues.push_back(value);
        while(m_lastValues.size() > m_algo2_goto_rel_s){
            m_lastValues.pop_front();
        }
        double moy = 0;
        for(auto i : m_lastValues){
            moy += i;
        }
        m_0 = moy/m_algo2_goto_rel_s;
    } else if(m_algo2 == ALGO2_PID){
        int res = value*m_algo2_pid_p + (m_lastValue-value)*m_algo2_pid_d;
        if(!m_inverse){
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

void print(std::vector<u_char> & l){
    for(auto i : l){
        printf("%02" PRIx16 " ", i);
        
    }
    printf("\n");
}

uint16_t crc16_update(uint16_t crc, uint8_t a) {
    int i;

    crc ^= (uint16_t)a;
    for (i = 0; i < 8; ++i) {
        if (crc & 1)
            crc = (crc >> 1) ^ 0xA001;
        else
            crc = (crc >> 1);
    }

    return crc;
}

uint16_t calculcrc16Modbus(std::vector<unsigned char> & l){
    uint16_t crc;
    crc = 0xFFFF;
    for(int i = 0; i < l.size(); i++){
        crc = crc16_update(crc, l[i]);
    }
    return crc;
}

void PilotModule::runAdrienVolant(std::vector<unsigned char> & l){
    uint16_t res = calculcrc16Modbus(l);
    int i = res/256;
    int j = res%256;
    l.push_back(j);
    l.push_back(i);
    print(l);
    if(m_serial){
        m_serial->writeData(l);
    }else {
        WARN("oh c'est nul!");
    }
}

void PilotModule::run(int i){
    std::vector<unsigned char> l;
    if(i == 0){
        INFO("connect");
        l = {0x01, 0x10, 0x00, 0x33, 0x00, 0x01, 0x02, 0x00, 0x01};
        runAdrienVolant(l);
        l.clear();
        l = {0x01, 0x06, 0x00, 0x6A, 0x00, 0x64};
        runAdrienVolant(l);
        if(m_serial) m_serial->writeData(l);
        l.clear();
    } else if(i ==1){
        INFO("disable");
        l = {0x01, 0x10, 0x00, 0x33, 0x00, 0x01, 0x02, 0x00, 0x00};
        runAdrienVolant(l);
        l.clear();
        
    } else if(i ==2){
        INFO("clean");
        l = {0x01, 0x10, 0x01, 0x31, 0x00, 0x01, 0x02, 0x00, 0x2E};
        runAdrienVolant(l);
        l.clear();
   }
}

void PilotModule::test(int i){
    if(m_serial == NULL){
        WARN("oh c'est nul!");
    }
    
    std::vector<unsigned char> l;
    if(i == 0){
        //run(0);
    }else if(i > 0){
        l = {0x01, 0x10, 0x01, 0x35, 0x00, 0x02, 0x04, 0x80, 0x00, 0x00, 0x00};
        runAdrienVolant(l);
    } else {
        l = {0x01, 0x10, 0x01, 0x36, 0x00, 0x02, 0x04, 0x80, 0x00, 0xFF, 0xFF};
        runAdrienVolant(l);
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
