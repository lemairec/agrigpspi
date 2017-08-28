#ifndef CONFIG_H
#define CONFIG_H

#include <string>

class Config  {
public:
    double m_largeur = 27;
    std::string m_input = "none";
    int m_baudrate = 9600;

    void save();
    void load();
};

//-500 =>

#endif // CONFIG_H
