#ifndef CONFIG_H
#define CONFIG_H

#include <string>

#define PILOT_LANGAGE_ARDUINO 0
#define PILOT_LANGAGE_HADRIEN 1

enum LissageGpsMode{
    LissageGpsMode_None = 0,
    LissageGpsMode_Ekf = 1
};

enum CapMode{
    CapMode_Custom = 0,
    CapMode_Ekf = 1,
    CapMode_Rmc = 2
};

enum LissageImuMode{
    LissageImuMode_None = 0,
    LissageImuMode_Ekf = 1
};

enum AlgoVolantMode{
    AlgoVolantMode_Encodeur = 0,
    AlgoVolantMode_CapteurVitesse = 1
};

class Config  {
public:
    std::string m_input_gps;
    int m_baudrate_gps = 115200;
    std::string m_file_gps;
    
    std::string m_inputPilot = "none";
    int m_baudratePilot = 115200;
    
    std::string m_imu_input = "none";
    int m_imu_baudrate = 115200;
    
    bool m_gga = false;
    
    int m_lissage_gps_mode = 0;
    double m_lissage_gps_ekf = 0.7;
    
    int m_cap_mode = 0;
    double m_cap_ekf = 0.7;
    double m_cap_custom_d = 3.0;
    
    int m_lissage_imu_mode = 0;
    double m_lissage_imu_ekf = 0.7;
    
    bool m_imu_correction_devers = true;
   
    bool m_sensDraw = true;
    bool m_debug = false;
    bool m_menu_vertical = false;
    bool m_debug_log = true;
    bool m_3d = false;
    
    bool m_fullscreen = true;
    
    int m_pilot_langage = PILOT_LANGAGE_ARDUINO;
    bool m_pilot_motor_inverse = false;
    bool m_pilot_encoder_inverse = false;
    double m_motor_vitesse_max = 100;
    double m_motor_vitesse_min = 40;
    double m_motor_vitesse_agressivite = 3;
    int m_pilot_frequence = 100;
    int m_pilot_auto_deactive = -1;
    int m_pilot_auto_active = -1;
    
    int m_pilot_algo = 0;
    double m_pilot_lookahead_d = 5;
    double m_pilot_rwp_kth = 1;
    double m_pilot_rwp_kte = 0.5;
    
    int m_pilot_adaptive_vitesse = 0;
    
    AlgoVolantMode m_algo_volant_mode = AlgoVolantMode_Encodeur;
    double m_agressivite_yawl = 1.0;
    double m_yawl_kus = 1.0;
    
    double m_volant_pas_by_tour = 4500;
    double m_volant_angle_by_tour = 35;
    double m_volant_derive = 0.01;
    
    double m_distance_cap_vitesse = 10.0;

    bool m_logNMEA = true;
    bool m_api = true;
    
    double m_tracteur_antenne_pont_arriere = 1;
    double m_tracteur_empatement = 2.75;
    double m_tracteur_hauteur = 2.75;
    double m_tracteur_antenne_lateral = 0;
    
    double m_outil_distance = 1.5;
    double m_outil_largeur = 3.0;
    
    bool m_largeur_AB_set = false;
    double m_largeur_AB = 3.0;
    double m_offset_AB = 0;
    double m_angle_AB = 0;
    
    double m_resolution_calcul = 0.5;
    double m_resolution_draw = 1.0;
       
    Config();
    
    void save();
    void load();
};

//-500 =>

#endif // CONFIG_H
