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
public:
    void initOrLoad(Config & config);

    bool m_reset = true;
    
    double m_old_x;
    double m_old_y;
    double m_old_z;
    
    
    double m_v_x;
    double m_v_y;
    double m_v_z;
    
    double m_pitch_y_deg;

    double m_deplacementAngle;
    double m_v;
    
    std::list<GpsPoint_ptr> m_list;
    std::list<GpsPoint_ptr> m_list_tracteur;
    std::list<GpsPoint_ptr> m_list_ekf;
    
    void calculDeplacement(GpsPoint_ptr p, Tracteur & tracteur);
    void onNewEkfPoint(double x, double y, double z,double ax, double ay, double az);
    
    void reset();
};

#endif //EKF_MODULE_H
