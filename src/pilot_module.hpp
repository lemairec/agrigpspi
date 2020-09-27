#ifndef PILOT_H
#define PILOT_H
#include <string>
#include <memory>
#include <list>
#include <vector>
#include "config/config.hpp"
#include <stdint.h>


class PilotModule {
    bool m_inverse = false;
    
    bool m_engage = false;
    int m_pilot_langage = 0;
    int m_motor_vitesse_max = 0;
    int m_motor_vitesse_min = 0;
    double m_motor_vitesse_agressivite = 1;
    
    //algo k
    double m_algo2_goto_pas_by_tour = 100;
    double m_algo2_goto_angle_by_tour = 100;
    int m_volant_derive = 100;
    
    std::string m_inputPilot;
public:
    double m_last_value = 0;
    std::string m_last_order_send;
    
    double m_last_goto_tour = 0;
    double m_last_goto_pas = 0;
    double m_last_leftright = 0;
    std::string m_version_guidage;
    
    PilotModule();
    void run(double value, double time, double vitesse);
    void initOrLoadConfig(Config & config);

    void clear();
    void engage();
    void setVolant0();
    void desengage();
    
    void myGotoVolant2(double i);
    void myLeftRight(double i);
    
    double m_volant = 0.0;
    double m_volant0 = 0.0;
    
    double m_volantMesured = 0.0;
    
    void setPasMotorVolant(int pas);
    void setVolant(double vol);
    void update();
    
    //ARDUINO
    
    
    //HADRIEN
    
    void engageHadrien();
    void desengageHadrien();
    void clearHadrien();
    void runHadrienVolant(std::vector<unsigned char> & c);
    
    double m_hadrien0 = 0.0;
    int m_nbrTourHadrien = 0;
    double m_lastHadrienValue = 0;
    void setHadrienVolant(double val);
    
    
};

#endif //GPS_H
