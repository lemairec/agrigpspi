#include "pilot_module.hpp"
#include <math.h>
#include "logging.hpp"
#include <inttypes.h>
#include <sstream>

#include "gps_framework.hpp"
PilotModule::PilotModule(){
}

void PilotModule::initOrLoadConfig(Config & config){
    m_pilot_motor_inverse = config.m_pilot_motor_inverse;
    m_pilot_encoder_inverse = config.m_pilot_encoder_inverse;
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
    //m_last_value = 0;
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


void PilotModule::setAngleRoueDesired(double value, double time){
    if(m_engaged){
        double res = value/m_algo2_goto_angle_by_tour;
        m_volant = res;
        
        
        m_volant0 += res*m_volant_derive;
        
        m_volantTotal = m_volant0+m_volant;
    }
    
    
}

void PilotModule::myGotoVolant2(double res){
    m_volant = res;
    m_volantTotal = res;
    
    update();
}

void PilotModule::setPasMotorVolant(int pas){
    double res = pas/m_algo2_goto_pas_by_tour;
    setVolant(res);
    update();
}

int temp_i = 0;

void PilotModule::setVolant(double vol){
    GpsFramework & f = GpsFramework::Instance();
    f.m_pilot_time.setNewTime();
    int frequence = f.m_config.m_pilot_virtual_point;
    if(frequence > 0){
        temp_i = (temp_i+1)%frequence;
        if(temp_i == 0){
            GpsFramework::Instance().updateWithoutGps();
        }
    }
    
    if(!m_pilot_encoder_inverse){
        m_volantMesured = vol;
    } else {
        m_volantMesured = -vol;
    }
}

void PilotModule::update(){
    if(m_pilot_langage == PILOT_LANGAGE_ARDUINO){
        arduinoUpdate();
    } else {
        INFO("m_motor_vitesse_agressivite " << m_motor_vitesse_agressivite << " m_volantTotal " << m_volantTotal << " " << m_volantMesured);
        hadrienLeftRight(m_motor_vitesse_agressivite*(m_volantTotal - m_volantMesured));
        //hadrienGoTo(-m_volant);
    }
}

void PilotModule::test(){
    GpsFramework::Instance().m_serialModule.writeGpsSerialS("sbas control enable egnos 0 none\n");
    GpsFramework::Instance().m_serialModule.writeGpsSerialS("saveconfig\n");
    GpsFramework::Instance().m_serialModule.writeGpsSerialS("pdpfilter enable\n");
    GpsFramework::Instance().m_serialModule.writeGpsSerialS("pdpmode relative auto\n");
    GpsFramework::Instance().m_serialModule.writeGpsSerialS("saveconfig\n");
}



/**
    ARDUINO
 */

void ArduinoParser::parseBuffer(){
    if(m_bufferIndLast > 1){
        if(m_buffer[0] == 'P'  && m_buffer[1] == ','){
            //INFO("****new" << m_tempInd << " " << m_bufferIndLast);
            //printBuffer();
            readUntilCommat();
            
            int res = readNegInt();
            GpsFramework::Instance().m_pilotModule.setPasMotorVolant(res);
            
        } else if(m_buffer[0] == 'P'  && m_buffer[1] == 'I'){
            std::string s;
            for(size_t i =0; i < m_bufferIndLast; ++i){
                s += m_buffer[i];
            }
            GpsFramework::Instance().m_pilotModule.m_version_guidage = s;
             //GpsFramework::Instance().m_pilotModule.m_version_guidage = s;
        } else {
            
        }
    }
}

void  PilotModule::arduinoParse(const std::string & s){
    for(auto c : s){
        m_arduino_parser.readChar(c);
    }

}

void PilotModule::arduinoUpdate(){
    int res = m_motor_vitesse_agressivite*(m_volantTotal - m_volantMesured)*m_algo2_goto_pas_by_tour;
    if(m_pilot_motor_inverse){
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
        out << "$L," << -res << ";\n";
    } else {
        out << "$R," << res << ";\n";
    }
    m_last_order_send = out.str();
    //INFO(m_last_order_send);
    GpsFramework::Instance().m_serialModule.writePilotSerialS(out.str());
}

void PilotModule::handleArduino(){
    GpsFramework::Instance().m_serialModule.writePilotSerialS("$P;\n");
}



/**
   HADRIEN VOLANT
 */


void printHexa(std::vector<u_char> & l){
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
    for(size_t i = 0; i < l.size(); i++){
        crc = crc16_update(crc, l[i]);
    }
    return crc;
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

void add2hex( std::vector<unsigned char> & l, int i){
    u_int u_i = i;

    unsigned char i0 = u_i%256;
    u_i = u_i/256;
    unsigned char i1 = u_i%256;
    u_i = u_i/256;

    l.push_back(i1);
    l.push_back(i0);
}

void PilotModule::runHadrienVolant(std::vector<unsigned char> & l){
    uint16_t res = calculcrc16Modbus(l);
    int i = res/256;
    int j = res%256;
    l.push_back(j);
    l.push_back(i);
    printHexa(l);
    GpsFramework::Instance().m_serialModule.writePilotSerialDAndWait(l);
}

void PilotModule::engageHadrien(){
    INFO("engageHadrien");
    {
        //mode vitesse
        std::vector<unsigned char> l = {0x01, 0x06, 0x00, 0x00, 0x00, 0x07};
        runHadrienVolant(l);
    }
    {
        //engage
        std::vector<unsigned char> l = {0x01, 0x10, 0x00, 0x33, 0x00, 0x01, 0x02, 0x00, 0x01};
        runHadrienVolant(l);
    }
}
void PilotModule::desengageHadrien(){
    INFO("desengageHadrien");
    std::vector<unsigned char> l = {0x01, 0x10, 0x00, 0x33, 0x00, 0x01, 0x02, 0x00, 0x00};
    runHadrienVolant(l);
}
void PilotModule::clearHadrien(){
    INFO("clearHadrien");
    std::vector<unsigned char> l = {0x01, 0x10, 0x01, 0x31, 0x00, 0x01, 0x02, 0x00, 0x2E};
    runHadrienVolant(l);
}


/*void PilotModule::hadrienGoTo(double res){
    int res2 = res*32768;
    std::vector<unsigned char> l;
    l = {0x01, 0x10, 0x01, 0x43, 0x00, 0x02, 0x04};
    add4hex(l, res2);
    runHadrienVolant(l);
}*/

void PilotModule::hadrienLeftRight(double res){
    int res2 = -res;
    INFO(res2);
    if(res2 > m_motor_vitesse_max){
        res2 = m_motor_vitesse_max;
    }
    if(res2 < -m_motor_vitesse_max){
        res2 = -m_motor_vitesse_max;
    }
    
    std::vector<unsigned char> l = {0x01, 0x06, 0x00, 0x6A};
    add2hex(l, res2);
    runHadrienVolant(l);
}


/*void PilotModule::hadrienLeftRightPosition(double res){
    int res2 = round(res*32768);
    std::vector<unsigned char> l;
    if(res2>0){
        l = {0x01, 0x10, 0x01, 0x36, 0x00, 0x02, 0x04};
        add4hex(l, -res2);
    } else {
        l = {0x01, 0x10, 0x01, 0x35, 0x00, 0x02, 0x04};
        add4hex(l, -res2);
    }
    runHadrienVolant(l);
}*/

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
}

void PilotModule::handleHadrien(){
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
        //INFO("c'est moin cool" << i0 << " " << i1 << " " << i2);
    }
}


