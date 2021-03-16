#ifndef EKF_MODULE_H
#define EKF_MODULE_H


#include "config/config.hpp"
#include "gps_module.hpp"
#include "tracteur.hpp"
#include <string>
#include <memory>
#include <list>

class EkfModule {
    LissageGpsMode m_lissage_gps_mode = LissageGpsMode_Ekf;
    double m_lissage_gps_ekf = 0.7;

public:
    CapMode m_cap_mode = CapMode_Custom;
private:
    double m_cap_ekf = 0.7;
    double m_cap_custom_d = 3.0;
    
    LissageImuMode m_lissage_imu_mode = LissageImuMode_None;
    double m_lissage_imu_ekf = 0.7;
    
    bool m_imu_correction_devers = true;
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
    void lissageGpsPoint(double x, double y, double z);
    void calculCap();
    
    void reset();
};

#endif //EKF_MODULE_H
