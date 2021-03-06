#ifndef PILOT_H
#define PILOT_H
#include <string>
#include <memory>
#include <list>
#include <vector>
#include "config/config.hpp"
#include <stdint.h>
#include "util/parser.hpp"

class ArduinoParser : public Parser {
    void parseBuffer();
};

class PilotModule {
    ArduinoParser m_arduino_parser;
    bool m_pilot_motor_inverse = false;
    bool m_pilot_encoder_inverse = false;
    
    int m_pilot_langage = 0;
    int m_motor_vitesse_max = 0;
    int m_motor_vitesse_min = 0;
    double m_motor_vitesse_agressivite = 1;
    
    //algo k
    double m_algo2_goto_pas_by_tour = 100;
    double m_algo2_goto_angle_by_tour = 100;
    double m_volant_derive = 0.01;
    
    std::string m_inputPilot;
public:
    bool m_engaged = false;
    
    std::string m_last_order_send;
    
    double m_last_goto_tour = 0;
    double m_last_goto_pas = 0;
    double m_last_leftright = 0;
    std::string m_version_guidage;
    
    PilotModule();
    void setAngleRoueDesired(double value, double time);
    void turnRoue(double degree);
    void initOrLoadConfig(Config & config);

    void clear();
    void engage();
    void setVolant0();
    void desengage();
    
    void myGotoVolant2(double i);
    
    double m_volant = 0.0;
    double m_volant0 = 0.0;
    double m_volantTotal = 0.0;
    
    double m_volantMesured = 0.0;
    
    void setPasMotorVolant(int pas);
    void setVolant(double vol);
    void update();
    
    void test();
    
    //ARDUINO
    void arduinoUpdate();
    void arduinoParse(const std::string & s);
    void handleArduino();
    
    //HADRIEN
    int m_nbrTourHadrien = 0;
    double m_lastHadrienValue = 0;
    
    void engageHadrien();
    void desengageHadrien();
    void clearHadrien();
    void runHadrienVolant(std::vector<unsigned char> & c);
    
    void hadrienLeftRight(double i);
    void hadrienGoTo(double i);
    
    void setHadrienVolant(double val);
    void parseHadrienVolant(char * c);
    
    void handleHadrien();
};

#endif //GPS_H
