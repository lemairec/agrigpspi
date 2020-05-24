#ifndef PILOT_H
#define PILOT_H
#include <string>
#include <memory>
#include <list>
#include <vector>
#include "config/config.hpp"
#include <stdint.h>


class PilotModule {
    bool m_inverse = false;
    
    int m_pilot_langage = 0;
    
    //algo k
    int m_algo2 = 0;
    double m_algo2_goto_k = 100;
    int m_algo2_goto_rel_s = 100;
    
    //algo pid
    double m_integral = 0;
    double m_lastValue = 0;
    double m_algo2_pid_p = 100;
    double m_algo2_pid_i = 100;
    double m_algo2_pid_d = 100;
    
    //algo my
    double m_sum_value = 0;
    double m_algo2_my_k = 100;
    double m_algo2_my_p = 100;
    
    std::list<double> m_lastValues;
    
    std::string m_inputPilot;
public:
    double m_0 = 0;
    std::string m_version_guidage;
    
    PilotModule();
    void run(double value, double time);
    void initOrLoadConfig(Config & config);

    void clear();
    void engage();
    void desengage();
    
    void run(int i);
    void test(int i);
    
    void myGoto(int i);
    void myLeftRight(int i);
    
    void engageHadrien();
    void desengageHadrien();
    void clearHadrien();
    void runHadrienVolant(std::vector<unsigned char> & c);
};

#endif //GPS_H
