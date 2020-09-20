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
    m_vitesse_motor = config.m_vitesse_motor;
    
    m_algo2 = config.m_algo2;
    m_algo2_goto_pas_by_tour = config.m_algo2_goto_pas_by_tour;
    m_algo2_goto_angle_by_tour = config.m_algo2_goto_angle_by_tour/180.0*3.14;
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

void PilotModule::setVitesse(){
    int vitesse = m_vitesse_motor*250/100;
    std::ostringstream out;
    out << "$C;" << vitesse << "\n";
    GpsFramework::Instance().m_serialModule.writePilotSerialS(out.str());
}

void PilotModule::engage(){
    if(m_pilot_langage == PILOT_LANGAGE_HADRIEN){
        clearHadrien();
        engageHadrien();
    } else {
        setVitesse();
    }
}
void PilotModule::desengage(){
    if(m_pilot_langage == PILOT_LANGAGE_HADRIEN){
        clearHadrien();
        desengageHadrien();
    } else {
        GpsFramework::Instance().m_serialModule.writePilotSerialS("$D;");
    }
}


void PilotModule::run(double value, double time){
    std::ostringstream out;
    
    if(m_algo2 == ALGO2_GOTO){
        double res = value/m_algo2_goto_angle_by_tour;
        m_volant = res;
        
        myGotoVolant(res);
    } else if(m_algo2 == ALGO2_GOTO_REL){
        double res = value/m_algo2_goto_angle_by_tour;
        m_volant = res;
        
        m_lastValues.push_back(res);
        while(m_lastValues.size() > m_algo2_goto_rel_s){
            m_lastValues.pop_front();
        }
        double moy = 0;
        for(auto i : m_lastValues){
            moy += i;
        }
        m_volant0 = moy/m_algo2_goto_rel_s;
        
        myGotoVolant(m_volant0+m_volant);
        
        
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
    GpsFramework::Instance().m_serialModule.writePilotSerialDAndWait(l);
}

void PilotModule::run_test(int i){
    if(m_pilot_langage == PILOT_LANGAGE_HADRIEN){
        if(i == 0){
            std::vector<unsigned char> l;
            /*l = {0x01, 0x03, 0x40, 0x08, 0x00, 0x02};
            runHadrienVolant(l);
            l.clear();
            l = {0x01, 0x03, 0x40, 0x09, 0x00, 0x02};
            runHadrienVolant(l);*/
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
            myLeftRight(1);
        } else {
            myLeftRight(-1);
        }
    } else {
        if(i == 0){
            myGotoVolant(0);
        }else if(i > 0){
            run(20.0/180.0*3.14, 0);
        } else {
            run(-20.0/180.0*3.14, 0);
        }
    }
}

void PilotModule::engageHadrien(){
    m_hadrien0 = m_volant;
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

void PilotModule::myGotoVolant2(double res){
    m_volant = res;
    myGotoVolant(res);
}

void PilotModule::myGotoVolant(double res){
    if(m_inverse){
        res = -res;
    }
    
    m_last_goto_pas = res*m_algo2_goto_pas_by_tour;
    
    int res2 = m_last_goto_pas;
    std::ostringstream out;
    if(res<0){
        out << "$G;-" << (-res2) << "\n";
    } else {
        out << "$G;" << res2 << "\n";
    }
    m_last_order_send = out.str();
    INFO(m_last_order_send);
    if(m_pilot_langage == PILOT_LANGAGE_ARDUINO){
        
        GpsFramework::Instance().m_serialModule.writePilotSerialS(out.str());
    } else {
        INFO("goto " << res);
        INFO("m_hadrien0 " << m_hadrien0);
        
        double gotoRel = res-m_hadrien0;
        
        INFO("gotoRel " << m_hadrien0);
        INFO("hadrienVolant " << m_volant);
        
        double leftRight = m_volant-gotoRel;
        INFO("leftRight " << leftRight);
        myLeftRight(leftRight);
        //m_tour_volant = res/4000.0
        
        
        /*std::vector<unsigned char> l;
        l = {0x01, 0x10, 0x01, 0x43, 0x00, 0x02, 0x04};
        add4hex(l, res);
        runHadrienVolant(l);*/
    }
}



void PilotModule::myLeftRight(double res){
    if(m_inverse){
        //res = -res;
    }
    m_last_leftright = res;
    
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
        int res2 = res*32000;
        INFO(out.str() << " " << res2);
        std::vector<unsigned char> l;
        if(res2>0){
            l = {0x01, 0x10, 0x01, 0x35, 0x00, 0x02, 0x04};
            add4hex(l, res2);
        } else {
            l = {0x01, 0x10, 0x01, 0x36, 0x00, 0x02, 0x04};
            add4hex(l, res2);
        }
        runHadrienVolant(l);
        GpsFramework::Instance().addLog(out.str(), false);
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
    
    m_volantMesured = m_nbrTourHadrien + val;
    
    std::ostringstream out;
    out << "set H " << m_volant;
    GpsFramework::Instance().addLog(out.str(), false);
}

void PilotModule::setPasMotorVolant(int pas){
    m_volantMesured = pas/m_algo2_goto_pas_by_tour;
}

void PilotModule::setVolant(double vol){
    m_volantMesured = vol;
}


