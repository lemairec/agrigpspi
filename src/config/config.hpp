#ifndef CONFIG_H
#define CONFIG_H

#include <string>

#define ALGO_PID 0
#define ALGO_2 1

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
    
    int m_algo = 0;
    
    double m_algo1_kp = 100;
    double m_algo1_kd = 100;
    double m_algo2_k = 100;
    
    
    Config();
    
    void save();
    void load();
};

//-500 =>

#endif // CONFIG_H
