#ifndef PILOT_H
#define PILOT_H
#include <string>
#include <memory>

#include "config/config.hpp"

class Serial;

class PilotModule {
    Serial * m_serial = NULL;
    double m_last_value = 0;
    double m_p;
    double m_i;
    double m_d;
    
public:
    PilotModule();
    void run(double value);
    void initOrLoadConfig(Config & config);
    
    void test(int i);
};

#endif //GPS_H
