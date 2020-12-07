#ifndef EKF_MODULE_H
#define EKF_MODULE_H
#include <string>
#include <memory>

class EkfModule {
public:
    double m_old_x;
    double m_old_y;
    double m_old_z;

    void onNewPoint(double x, double y, double z, double v, double ang, double ax, double ay, double az);
};

#endif //EKF_MODULE_H
