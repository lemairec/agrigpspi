#ifndef PILOT_H
#define PILOT_H
#include <string>
#include <memory>

#include "config/config.hpp"

class Serial;

class PilotModule {
    Serial * m_serial = NULL;
    
    bool m_inverse = false;
    double m_last_value = 0;
    double m_algo_k;
       
    int m_value_volant = 0;
public:
    PilotModule();
    void run(double value);
    void initOrLoadConfig(Config & config);
    
    void clear();
    void test(int i);
};

#endif //GPS_H
