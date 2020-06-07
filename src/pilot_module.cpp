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
    m_algo2_pid_i = config.m_algo2_pid_i;
    m_algo2_pid_d = config.m_algo2_pid_d;
    
    m_algo2_my_k = config.m_algo2_my_k;
    m_algo2_my_p = config.m_algo2_my_p;
    
    m_pilot_langage = config.m_pilot_langage;
    //m_version_guidage = m_serial->readString(12);
    
    clear();
    
}

void PilotModule::clear(){
    m_0 = 0;
    m_last_value = 0;
    m_integral = 0;
    m_sum_value = 0;
    
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


void PilotModule::run(double value, double time){
    std::ostringstream out;
    
    if(m_algo2 == ALGO2_GOTO){
        int res = value*m_algo2_goto_k;
        myGoto(res);
    } else if(m_algo2 == ALGO2_GOTO_REL){
        //int res = (value-m_0)*m_algo2_goto_k; test1
        double correction = value+m_0;
        int res = correction*m_algo2_goto_k;
        myGoto(res);
        m_lastValues.push_back(correction);
        while(m_lastValues.size() > m_algo2_goto_rel_s){
            m_lastValues.pop_front();
        }
        double moy = 0;
        for(auto i : m_lastValues){
            moy += i;
        }
        m_0 = moy/m_algo2_goto_rel_s;
    } else if(m_algo2 == ALGO2_PID){
        double _dt = 0.1;
        
        // Proportional term
        double Pout = m_algo2_pid_p * value;

        // Integral term
        m_integral += value * _dt;
        double Iout = m_algo2_pid_i * m_integral;

        // Derivative term
        double derivative = (value - m_last_value) / _dt;
        double Dout = m_algo2_pid_d * derivative;

        // Calculate total output
        int res = Pout + Iout + Dout;
        
        myLeftRight(res);
    } else if(m_algo2 == ALGO2_MY){
        
        int res = m_algo2_my_k * (value - m_last_value);
        
        myLeftRight(res);
    }
    m_last_value = value;
    
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
            std::vector<unsigned char> l;
            l = {0x01, 0x03, 0x40, 0x08, 0x00, 0x02};
            runHadrienVolant(l);
            l.clear();
            l = {0x01, 0x03, 0x40, 0x09, 0x00, 0x02};
            runHadrienVolant(l);
            //engageHadrien();
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
            myLeftRight(m_algo2_pid_p);
        } else {
            myLeftRight(-m_algo2_pid_p);
        }
    } else {
        if(i == 0){
            myGoto(0);
        }else if(i > 0){
            myGoto(m_algo2_goto_k);
        } else {
            myGoto(-m_algo2_goto_k);
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


void add4hex( std::vector<unsigned char> & l, int i){
    u_int u_i = i;
    
    unsigned char i0 = u_i%256;
    u_i = u_i/256;
    unsigned char i1 = u_i%256;
    u_i = u_i/256;
    unsigned char i2 = u_i%256;
    u_i = u_i/256;
    unsigned char i3 = u_i%256;
    u_i = u_i/256;
    
    l.push_back(i1);
    l.push_back(i0);
    l.push_back(i3);
    l.push_back(i2);
    
}

void PilotModule::myGoto(int res){
    if(m_inverse){
        res = -res;
    }
    std::ostringstream out;
    if(res<0){
        out << "$G;-" << (-res) << "\n";
    } else {
        out << "$G; " << res << "\n";
    }
    m_last_order_send = out.str();
    if(m_pilot_langage == PILOT_LANGAGE_ARDUINO){
        
        GpsFramework::Instance().m_serialModule.writePilotSerialS(out.str());
    } else {
        std::vector<unsigned char> l;
        l = {0x01, 0x10, 0x01, 0x43, 0x00, 0x02, 0x04};
        add4hex(l, res);
        runHadrienVolant(l);
    }
}



void PilotModule::myLeftRight(int res){
    if(m_inverse){
        res = -res;
    }
    std::ostringstream out;
    if(res<0){
        out << "$L;" << (-res) << "\n";
    } else {
        out << "$R; " << res << "\n";
    }
    m_last_order_send = out.str();
    if(m_pilot_langage == PILOT_LANGAGE_ARDUINO){
        GpsFramework::Instance().m_serialModule.writePilotSerialS(out.str());
    } else {
        int res_a = std::abs(res);
        unsigned char res1 = (res_a/256)%256;
        unsigned char res2 = res_a%256;
        std::vector<unsigned char> l;
        if(res>0){
            l = {0x01, 0x10, 0x01, 0x35, 0x00, 0x02, 0x04};
            add4hex(l, res);
        } else {
            l = {0x01, 0x10, 0x01, 0x36, 0x00, 0x02, 0x04};
            add4hex(l, res);
        }
        runHadrienVolant(l);
    }
}

void PilotModule::setHadrienVolant(double val){
    double temp = m_lastHadrienValue - val;
     
    if(temp > 0.5){
        m_nbrTourHadrien += 1;
    }
    
    if(temp < -0.5){
        m_nbrTourHadrien -= 1;
    }
    
    m_lastHadrienValue = val;
    
    m_hadrienVolant = m_nbrTourHadrien + val;
}


