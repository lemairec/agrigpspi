#ifndef CONFIG_H
#define CONFIG_H

#include <string>

class Config  {
public:
    double m_largeur = 27;
    std::string m_port = "/dev/cu.wchusbserial1420";
    int m_baudrate = 9600;

    void save();
    void load();
};

#endif // CONFIG_H
