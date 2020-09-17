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
    double m_algo2_goto_pas_by_tour = 100;
    double m_algo2_goto_angle_by_tour = 100;
    int m_algo2_goto_rel_s = 100;
    
    //algo pid
    double m_integral = 0;
    //double m_lastValue = 0;
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
    double m_last_value = 0;
    std::string m_last_order_send;
    
    double m_0 = 0;
    double m_last_goto_tour = 0;
    double m_last_goto_pas = 0;
    double m_last_leftright = 0;
    std::string m_version_guidage;
    
    PilotModule();
    void run(double value, double time);
    void initOrLoadConfig(Config & config);

    void clear();
    void engage();
    void desengage();
    
    void run_test(int i);
    void test(int i);
    
    void myGotoVolant(double i);
    void myLeftRight(double i);
    
    void engageHadrien();
    void desengageHadrien();
    void clearHadrien();
    void runHadrienVolant(std::vector<unsigned char> & c);
    
    double m_hadrien0 = 0.0;
    int m_nbrTourHadrien = 0;
    double m_lastHadrienValue = 0;
    void setHadrienVolant(double val);
    
    double m_volant = 0.0;
    double m_volant0 = 0.0;
    
    double m_volantMesured = 0.0;
    
    void setPasMotorVolant(int pas);
    void setVolant(double vol);
};

#endif //GPS_H
