#ifndef CONFIG_H
#define CONFIG_H

#include <string>

#define ALGO_NAIF 0
#define ALGO_FOLLOW_CARROT 1

#define ALGO2_GOTO 0
#define ALGO2_GOTO_REL 1
#define ALGO2_PID 2

class Config  {
public:
    double m_largeur = 27;
    
    std::string m_input;
    int m_baudrate = 9600;
    std::string m_file;
    
    std::string m_inputPilot = "none";
    int m_baudratePilot = 9600;
    
    double m_a_lat = 0;
    double m_a_lon = 0;
    double m_b_lat = 0;
    double m_b_lon = 0;

    
    bool m_sensDraw = false;
    bool m_debug = false;
    
    
    bool m_pilot_inverse = false;
    
    int m_algo = 0;
    double m_algo_naif_k = 100;
    double m_algofk_lookahead_d = 10;
    
    int m_algo2 = 0;
    double m_algo2_goto_k = 100;
    int m_algo2_goto_rel_s = 100;
    double m_algo2_pid_p = 100;
    double m_algo2_pid_d = 100;
    
    bool m_logNMEA = true;
    bool m_api = true;
    
    
    Config();
    
    void save();
    void load();
};

//-500 =>

#endif // CONFIG_H
