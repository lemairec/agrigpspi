#ifndef CONFIG_H
#define CONFIG_H

#include <string>

#define PILOT_LANGAGE_ARDUINO 0
#define PILOT_LANGAGE_HADRIEN 1

class Config  {
public:
    std::string m_input_gps;
    int m_baudrate_gps = 115200;
    std::string m_file_gps;
    
    std::string m_inputPilot = "none";
    int m_baudratePilot = 115200;
    
    double m_a_lat = 0;
    double m_a_lon = 0;
    double m_b_lat = 0;
    double m_b_lon = 0;

    bool m_gga = false;
   
    bool m_sensDraw = false;
    bool m_debug = false;
    bool m_menu_vertical = false;
    bool m_debug_log = true;
    
    int m_pilot_langage = PILOT_LANGAGE_ARDUINO;
    bool m_pilot_motor_inverse = false;
    bool m_pilot_encoder_inverse = false;
    double m_motor_vitesse_max = 100;
    double m_motor_vitesse_min = 40;
    double m_motor_vitesse_agressivite = 3;
    int m_pilot_time = 20;
    int m_pilot_auto_deactive = -1;
    int m_pilot_auto_active = -1;
    
    int m_pilot_algo = 0;
    double m_pilot_lookahead_d = 5;
    double m_pilot_lookahead_vd = 0.1;
    double m_pilot_rwp_kth = 1;
    double m_pilot_rwp_kte = 0.5;
    
    double m_volant_pas_by_tour = 4500;
    double m_volant_angle_by_tour = 35;
    int m_volant_derive = 100;
    
    double m_distance_cap_vitesse = 10.0;

    bool m_logNMEA = true;
    bool m_api = true;
    
    double m_tracteur_antenne_pont_arriere = 1;
    double m_tracteur_empatement = 2.75;
    double m_tracteur_hauteur = 2.75;
    double m_outil_distance = 1.5;
    double m_outil_largeur = 3.0;
    
    
    
    Config();
    
    void save();
    void load();
};

//-500 =>

#endif // CONFIG_H
