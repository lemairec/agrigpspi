#ifndef SERIAL_QT_H
#define SERIAL_QT_H

#include "../config/config.hpp"
#include "serial_port.hpp"

#include <vector>

class MyBoostSerialPorts {
    SerialPort * serialPilot;
    SerialPort * serialGps;
    
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
