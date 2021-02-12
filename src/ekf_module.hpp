#ifndef EKF_MODULE_H
#define EKF_MODULE_H


#include "config/config.hpp"
#include "gps_module.hpp"
#include "tracteur.hpp"
#include <string>
#include <memory>
#include <list>

class EkfModule {
    double m_coeff_lissage = 0.8;
    EkfMode m_ekf_mode = EkfNone;
    bool m_ekf_correction_devers = true;
public:
    void initOrLoad(Config & config);

    bool m_reset = true;
    
    double m_old_x;
    double m_old_y;
    double m_old_z;
    double m_the;
    
    double m_the_v;
    double m_v_x;
    double m_v_y;
    double m_v_z;
    
    double m_a_x;
    double m_a_y;
    double m_a_z;
    
    
    
    double m_pitch_y_deg;

    double m_deplacementAngle;
    double m_v;
    
    std::list<GpsPoint_ptr> m_list;
    std::list<GpsPoint_ptr> m_list_tracteur;
    std::list<GpsPoint_ptr> m_list_ekf;
    
    void calculDeplacement(GpsPoint_ptr p, Tracteur & tracteur);
    
    void onNewImuPoint(double ax, double ay, double az, double pitch_x_deg, double pitch_y_deg);
    void onNewGpsPoint(double x, double y, double z);
    
    void reset();
};

#endif //EKF_MODULE_H
