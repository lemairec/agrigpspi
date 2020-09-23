#ifndef CONFIG_H
#define CONFIG_H

#include <string>

#define ALGO_NAIF 0
#define ALGO_FOLLOW_CARROT 1

#define ALGO2_GOTO 0
#define ALGO2_GOTO_REL 1
#define ALGO2_MY 3

#define PILOT_LANGAGE_ARDUINO 0
#define PILOT_LANGAGE_HADRIEN 1

class Config  {
public:
    double m_largeur = 27;
    
    std::string m_input_gps;
    int m_baudrate_gps = 115200;
    std::string m_file_gps;
    
    std::string m_inputPilot = "none";
    int m_baudratePilot = 115200;
    
    double m_a_lat = 0;
    double m_a_lon = 0;
    double m_b_lat = 0;
    double m_b_lon = 0;

    
    bool m_sensDraw = false;
    bool m_debug = false;
    
    
    int m_pilot_langage = PILOT_LANGAGE_ARDUINO;
    bool m_pilot_inverse = false;
    double m_motor_vitesse_max = 100;
    double m_motor_vitesse_min = 40;
    double m_motor_vitesse_agressivite = 3;
    
    int m_algo = ALGO_FOLLOW_CARROT;
    double m_algo_lookahead_d = 10;
    
    int m_algo2 = ALGO2_GOTO_REL;
    double m_algo2_goto_pas_by_tour = 4500;
    double m_algo2_goto_angle_by_tour = 35;
    int m_algo2_goto_rel_s = 100;
    
    bool m_logNMEA = true;
    bool m_api = true;
    
    
    Config();
    
    void save();
    void load();
};

//-500 =>

#endif // CONFIG_H
