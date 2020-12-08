#ifndef EKF_MODULE_H
#define EKF_MODULE_H
#include <string>
#include <memory>

class EkfModule {
public:
    double m_coeff_lissage = 0.8;
    
    double m_old_x;
    double m_old_y;
    double m_old_z;
    
    double m_v_x;
    double m_v_y;
    double m_v_z;

    void onNewEkfPoint(double x, double y, double z, double v, double ang, double ax, double ay, double az);
};

#endif //EKF_MODULE_H
