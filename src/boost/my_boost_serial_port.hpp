#ifndef SERIAL_QT_H
#define SERIAL_QT_H

#include "../config/config.hpp"

#include <vector>

class MyBoostSerialPorts {
    std::string m_gps_serial_input;
    std::string m_pilot_serial_input;
    
    std::vector<std::string> m_serials;
    
public:
    MyBoostSerialPorts();
    
    void initOrLoad(Config & config);
    void open();
    
    void writePilotSerialD(std::vector<unsigned char> & l);
    void writePilotSerialS(const std::string & l);
    
    void addSerialPorts(std::string s);
    std::vector<std::string> & getAvailablePorts();
    
    
};


#endif
