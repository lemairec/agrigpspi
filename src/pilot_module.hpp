#ifndef PILOT_H
#define PILOT_H
#include <string>
#include <memory>

#include "config/config.hpp"

class Serial;

class PilotModule {
    Serial * m_serial = NULL;
public:
    PilotModule();
    void initOrLoadConfig(Config & config);
    
    void test(int i);
};

#endif //GPS_H
