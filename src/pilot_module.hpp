#ifndef PILOT_H
#define PILOT_H
#include <string>
#include <memory>
#include <list>
#include <vector>
#include "config/config.hpp"
#include <stdint.h>

class Serial;

class PilotModule {
    Serial * m_serial = NULL;
    
    bool m_inverse = false;
    
    int m_algo2 = 0;
    double m_algo2_goto_k = 100;
    int m_algo2_goto_rel_s = 100;
    double m_algo2_pid_p = 100;
    double m_algo2_pid_d = 100;
    
    double m_lastValue = 0;
    std::list<double> m_lastValues;
    
    std::string m_inputPilot;
public:
    double m_0 = 0;
      std::string m_version_guidage;
    
    PilotModule();
    void run(double value);
    void initOrLoadConfig(Config & config);
    
    void clear();
    void test(int i);
    
    void runAdrienVolant(std::vector<unsigned char> & c);
};

#endif //GPS_H
