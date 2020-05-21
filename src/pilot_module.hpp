#ifndef PILOT_H
#define PILOT_H
#include <string>
#include <memory>

#include "config/config.hpp"

class Serial;

class PilotModule {
    Serial * m_serial = NULL;
    
    bool m_inverse = false;
    double m_0 = 0;
    
    int m_algo2 = 0;
    double m_algo2_goto_k = 100;
    double m_algo2_goto_rel = 0.01;
    double m_algo2_pid_p = 100;
    double m_algo2_pid_d = 100;
    
    double m_lastValue = 0;
    
    std::string m_inputPilot;
public:
    std::string m_version_guidage;
    
    PilotModule();
    void run(double value);
    void initOrLoadConfig(Config & config);
    
    void clear();
    void test(int i);
};

#endif //GPS_H
