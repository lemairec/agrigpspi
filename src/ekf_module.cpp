#include "ekf_module.hpp"
#include <math.h>

void EkfModule::onNewPoint(double x, double y, double z, double v, double ang, double ax, double ay, double az){
    if(ax > 0){
        double dt = 0.1;
        double x_acc = m_old_x + ax*dt*dt/2.0;
        double y_acc = m_old_y + ay*dt*dt/2.0;
        double z_acc = m_old_z + az*dt*dt/2.0;
        
        double x_v = m_old_x + cos(ang)*v;
        double y_v = m_old_y + sin(ang)*v;
        
        double new_x = (x+x_acc+x_v)/3.0;
        double new_y = (y+y_acc+y_v)/3.0;
        double new_z = (z+z_acc)/2.0;
        
        m_old_x = new_x;
        m_old_y = new_y;
        m_old_z = new_z;
    } else {
        m_old_x = x;
        m_old_y = y;
        m_old_z = z;
    }
}
