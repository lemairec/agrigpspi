#include "pilot_module.hpp"
#include <math.h>
#include "logging.hpp"
#include <inttypes.h>
#include <sstream>

#include "gps_framework.hpp"
PilotModule::PilotModule(){
}

void PilotModule::initOrLoadConfig(Config & config){
    m_inverse = config.m_pilot_inverse;
    m_motor_vitesse_max = config.m_motor_vitesse_max*255.0/100.0;
    m_motor_vitesse_min = config.m_motor_vitesse_min*255.0/100.0;
    m_motor_vitesse_agressivite = config.m_motor_vitesse_agressivite;
    
    m_algo2_goto_pas_by_tour = config.m_volant_pas_by_tour;
    m_algo2_goto_angle_by_tour = config.m_volant_angle_by_tour/180.0*3.14;
    m_volant_derive = config.m_volant_derive;
   
    m_pilot_langage = config.m_pilot_langage;

    clear();
}

void PilotModule::clear(){
    m_last_value = 0;
}

void PilotModule::engage(){
    m_engaged = true;
    if(m_pilot_langage == PILOT_LANGAGE_HADRIEN){
        clearHadrien();
        engageHadrien();
    }
}
void PilotModule::desengage(){
    m_engaged = false;
    if(m_pilot_langage == PILOT_LANGAGE_HADRIEN){
        clearHadrien();
        desengageHadrien();
    } else {
        GpsFramework::Instance().m_serialModule.writePilotSerialS("$D;");
    }
}


void PilotModule::run(double value, double time, double vitesse){
    if(m_engaged){
        if(vitesse < 1.0){
            GpsFramework::Instance().addError("desengagement, vitesse trop faible");
            desengage();
        }
        
        double res = value/m_algo2_goto_angle_by_tour;
        m_volant = res;
        
        
        m_volant0 += res/m_volant_derive;
        
        m_volant = m_volant0+m_volant;
        
        m_last_value = value;
    }
    
    
}

void PilotModule::myGotoVolant2(double res){
    m_volant = res;
    
    update();
}

void PilotModule::setPasMotorVolant(int pas){
    double res = pas/m_algo2_goto_pas_by_tour;
    m_volantMesured = res;
    update();
}

void PilotModule::setVolant(double vol){
    if(m_inverse){
        m_volantMesured = -vol;
    } else {
        m_volantMesured = vol;
    }
}

void PilotModule::update(){
    if(m_pilot_langage == PILOT_LANGAGE_ARDUINO){
        arduinoUpdate();
    } else {
        hadrienGoTo(m_volant);
    }
}



/**
    ARDUINO
 */

int getIntWithChar(char c){
    if(c =='0'){
        return 0;
    } else if(c =='1'){
        return 1;
    } else if(c =='2'){
        return 2;
    } else if(c =='3'){
        return 3;
    } else if(c =='4'){
        return 4;
    } else if(c =='5'){
        return 5;
    } else if(c =='6'){
        return 6;
    } else if(c =='7'){
        return 7;
    } else if(c =='8'){
        return 8;
    } else if(c =='9'){
        return 9;
    } else {
        return 0;
    }
}

void  PilotModule::arduinoParse(const std::string & s){
    int i =0;
    if(s[i+0] == '$' && s[i+1] == 'P' && s[i+2] == ';'){
        int res = 0;
        int j = i+3;
        if(s[i+3] == '-'){
            j = i+4;
        }
        while(j < s.size()){
            char c = s[j];
            if(c == '\r'){
                break;
            } else {
                res = res*10 + getIntWithChar(c);
                ++j;
            }
        }
        if(s[i+3] == '-'){
            res = -res;
        }
        //INFO(s << " => " << res);
        GpsFramework::Instance().m_pilotModule.setPasMotorVolant(res);
        //GpsFramework::Instance().m_pilotModule.setPasVolant(res);
        
    }
    if(s[i+0] == '$' && s[i+1] == 'P' && s[i+2] == 'I'){
        GpsFramework::Instance().m_pilotModule.m_version_guidage = s;
    }
}

void PilotModule::arduinoUpdate(){
    int res = m_motor_vitesse_agressivite*(m_volant - m_volantMesured)*m_algo2_goto_pas_by_tour;
    if(m_inverse){
        res = -res;
    }
    
    if(!m_engaged){
        res = 0;
    }
    
    std::ostringstream out;
    
    if(res > m_motor_vitesse_max){
        res = m_motor_vitesse_max;
    }
    if(res < -m_motor_vitesse_max){
        res = -m_motor_vitesse_max;
    }
    
    if(res > 0 && res < m_motor_vitesse_min){
        res = 0;
    }
    if(res < 0 && res > -m_motor_vitesse_min){
        res = -0;
    }
    
    if(res<0){
        out << "$L;" << -res << ";\n";
    } else {
        out << "$R;" << res << ";\n";
    }
    m_last_order_send = out.str();
    //INFO(m_last_order_send);
    GpsFramework::Instance().m_serialModule.writePilotSerialS(out.str());
}


/**
   HADRIEN VOLANT
 */

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

void PilotModule::engageHadrien(){
    INFO("engageHadrien");
    std::vector<unsigned char> l;
    l = {0x01, 0x10, 0x00, 0x33, 0x00, 0x01, 0x02, 0x00, 0x01};
    runHadrienVolant(l);
    l.clear();
    l = {0x01, 0x06, 0x00, 0x6A, 0x00};
    uint16_t i = m_motor_vitesse_max;
    l.push_back(i);
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



/*void PilotModule::myGotoVolant(double res){
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
        
        
    std::vector<unsigned char> l;
    l = {0x01, 0x10, 0x01, 0x43, 0x00, 0x02, 0x04};
    add4hex(l, res);
    runHadrienVolant(l);
    //}
}*/

void PilotModule::hadrienGoTo(double res){
    int res2 = res*32768;
    std::vector<unsigned char> l;
    l = {0x01, 0x10, 0x01, 0x43, 0x00, 0x02, 0x04};
    add4hex(l, res2);
    runHadrienVolant(l);
}

void PilotModule::hadrienLeftRight(double res){
    int res2 = res*32768;
    std::vector<unsigned char> l;
    if(res2>0){
        l = {0x01, 0x10, 0x01, 0x36, 0x00, 0x02, 0x04};
        add4hex(l, -res2);
    } else {
        l = {0x01, 0x10, 0x01, 0x35, 0x00, 0x02, 0x04};
        add4hex(l, -res2);
    }
    runHadrienVolant(l);
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
    
    setVolant(m_nbrTourHadrien + val);
    
    /*std::ostringstream out;
    out << "set H " << m_volant;
    GpsFramework::Instance().addLog(out.str(), false);*/
}

void PilotModule::handleHadrien(){
    //GpsFramework::Instance().addLog("demand angle", false);
    std::vector<unsigned char> l = {0x01, 0x03, 0x40, 0x08, 0x00, 0x02, 0x50, 0x09};
    GpsFramework::Instance().m_serialModule.writePilotSerialD(l);
}

void PilotModule::parseHadrienVolant(char * data){
    int i0 = data[0];
    int i1 = data[1];
    int i2 = data[2];
    
    
    if (i0 == 1 && i1 ==3 && i2 == 4) {
        
        int r1 = data[3];
        int r2 = data[4];
        int r3 = data[5];
        int r4 = data[6];
        
        int temp1 = r3*256+r4;
        int temp2 = r1*256+r2;
        uint16_t r = temp1*65536 + temp2;
        int16_t res = r;
        
        GpsFramework::Instance().m_pilotModule.setHadrienVolant(res/4000.0);
        update();
        
        //INFO("toto" << r1 << " " << r2 << " " << r3 << " " << r4 << " ==> " << temp1 << " " << temp2 << " => " << r << " " << res);
        //u_int16_t res =
        //INFO("c'est cool");
        //01 10 01 36 00 02 A0 3A //acquittement sur le registre 36 peut etre 35
    } else {
        GpsFramework::Instance().addLog("autre", false);
        //INFO("c'est moin cool" << i0 << " " << i1 << " " << i2);
    }
}


