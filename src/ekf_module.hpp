#ifndef EKF_MODULE_H
#define EKF_MODULE_H


#include "config/config.hpp"
#include "gps_module.hpp"
#include <string>
#include <memory>
#include <list>

class EkfModule {
    double m_coeff_lissage = 0.8;
    EkfMode m_ekf_mode = EkfNone;
public:
    void initOrLoad(Config & config);

    
    double m_old_x;
    double m_old_y;
    double m_old_z;
    
    double m_v_x;
    double m_v_y;
    double m_v_z;

    std::list<GpsPoint_ptr> m_list;
    std::list<GpsPoint_ptr> m_list_tracteur;
    std::list<GpsPoint_ptr> m_list_ekf;
    
    void calculDeplacement(GpsPoint_ptr p);
    void onNewEkfPoint(double x, double y, double z, double v, double ang, double ax, double ay, double az);
};

#endif //EKF_MODULE_H
