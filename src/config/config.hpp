#ifndef CONFIG_H
#define CONFIG_H

#include <string>

class Config  {
public:
    double m_largeur = 27;
    
    std::string m_input;
    int m_baudrate = 9600;
    std::string m_file;
    
    double m_a_lat = 0;
    double m_a_lon = 0;
    double m_b_lat = 0;
    double m_b_lon = 0;

    Config();
    
    void save();
    void load();
};

//-500 =>

#endif // CONFIG_H
