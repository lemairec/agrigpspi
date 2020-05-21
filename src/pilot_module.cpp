#include "pilot_module.hpp"
#include <math.h>
#include "logging.hpp"
#include <inttypes.h>
#include <sstream>

#include "gps_framework.hpp"
PilotModule::PilotModule(){
    //setRef(LAT_REF, LON_REF);
}

void PilotModule::initOrLoadConfig(Config & config){
    m_inverse = config.m_pilot_inverse;
    
    m_algo2 = config.m_algo2;
    m_algo2_goto_k = config.m_algo2_goto_k;
    m_algo2_goto_rel_s = config.m_algo2_goto_rel_s;
    m_algo2_pid_p = config.m_algo2_pid_p;
    m_algo2_pid_d = config.m_algo2_pid_d;
    
    m_pilot_langage = config.m_pilot_langage;
    //m_version_guidage = m_serial->readString(12);
    
    clear();
    
}

void PilotModule::clear(){
    m_0 = 0;
    m_lastValue = 0;
    
    //todo GpsFramework::Instance().m_serialModule.writePilotSerialS("$C;\n");
}

void PilotModule::engage(){
    if(m_pilot_langage == PILOT_LANGAGE_HADRIEN){
        clearHadrien();
        engageHadrien();
    }
}
void PilotModule::desengage(){
    if(m_pilot_langage == PILOT_LANGAGE_HADRIEN){
        clearHadrien();
        desengageHadrien();
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
    GpsFramework::Instance().m_serialModule.writePilotSerialS(out.str());
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

void PilotModule::runHadrienVolant(std::vector<unsigned char> & l){
    uint16_t res = calculcrc16Modbus(l);
    int i = res/256;
    int j = res%256;
    l.push_back(j);
    l.push_back(i);
    print(l);
    GpsFramework::Instance().m_serialModule.writePilotSerialD(l);
}

void PilotModule::run(int i){
    if(m_pilot_langage == PILOT_LANGAGE_HADRIEN){
        if(i == 0){
            engageHadrien();
        } else if(i ==1){
            desengageHadrien();
        } else if(i ==2){
            clearHadrien();
        }
    }
}

void PilotModule::test(int i){
    if(m_algo2 == ALGO2_PID){
        if(i > 0){
            myLeftRight(32768);
        } else {
            myLeftRight(-32768);
        }
    } else {
        if(i == 0){
            myGoto(0);
        }else if(i > 0){
            myGoto(32768);
        } else {
            myGoto(-32768);
        }
    }
}

void PilotModule::engageHadrien(){
    INFO("engageHadrien");
    std::vector<unsigned char> l;
    l = {0x01, 0x10, 0x00, 0x33, 0x00, 0x01, 0x02, 0x00, 0x01};
    runHadrienVolant(l);
    l.clear();
    l = {0x01, 0x06, 0x00, 0x6A, 0x00, 0x64};
    runHadrienVolant(l);
}
void PilotModule::desengageHadrien(){
    INFO("desengageHadrien");
    std::vector<unsigned char> l;
    l = {0x01, 0x10, 0x00, 0x33, 0x00, 0x01, 0x02, 0x00, 0x00};
    runHadrienVolant(l);
    l.clear();
}
void PilotModule::clearHadrien(){
    INFO("clearHadrien");
    std::vector<unsigned char> l;
    l = {0x01, 0x10, 0x01, 0x31, 0x00, 0x01, 0x02, 0x00, 0x2E};
    runHadrienVolant(l);
    l.clear();
}


void PilotModule::myGoto(int res){
    if(m_pilot_langage == PILOT_LANGAGE_ARDUINO){
        std::ostringstream out;
        if(res<0){
            out << "$G;-" << (-res) << "\n";
        } else {
            out << "$G; " << res << "\n";
        }
        GpsFramework::Instance().m_serialModule.writePilotSerialS(out.str());
    } else {
        int res_a = std::abs(res);
        unsigned char res1 = (res_a/256)%256;
        unsigned char res2 = res_a%256;
        std::vector<unsigned char> l;
        if(res>0){
            l = {0x01, 0x10, 0x01, 0x43, 0x00, 0x02, 0x04};
            l.push_back(res2);
            l.push_back(res1);
            l.push_back(0x00);
            l.push_back(0x00);
        } else {
            l = {0x01, 0x10, 0x01, 0x43, 0x00, 0x02, 0x04};
            l.push_back(res2);
            l.push_back(res1);
            l.push_back(0xFF);
            l.push_back(0xFF);
            
        }
        runHadrienVolant(l);
    }
}

void PilotModule::myLeftRight(int res){
    if(m_pilot_langage == PILOT_LANGAGE_ARDUINO){
        std::ostringstream out;
        if(res<0){
            out << "$L;" << (-res) << "\n";
        } else {
            out << "$R; " << res << "\n";
        }
        GpsFramework::Instance().m_serialModule.writePilotSerialS(out.str());
    } else {
        int res_a = std::abs(res);
        unsigned char res1 = (res_a/256)%256;
        unsigned char res2 = res_a%256;
        std::vector<unsigned char> l;
        if(res>0){
            l = {0x01, 0x10, 0x01, 0x35, 0x00, 0x02, 0x04};
            l.push_back(res2);
            l.push_back(res1);
            l.push_back(0x00);
            l.push_back(0x00);
        } else {
            l = {0x01, 0x10, 0x01, 0x36, 0x00, 0x02, 0x04};
            l.push_back(res2);
            l.push_back(res1);
            l.push_back(0xFF);
            l.push_back(0xFF);
            
        }
        runHadrienVolant(l);
    }
}


