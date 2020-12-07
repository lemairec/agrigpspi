#include "ekf_module.hpp"

#include "logging.hpp"
#include <math.h>

void EkfModule::onNewPoint(double x, double y, double z, double v, double ang, double ax, double ay, double az){
    double dt = 0.1;
    double x_v = m_old_x + sin(ang)*v*dt;
    double y_v = m_old_y + cos(ang)*v*dt;
    
    if(ax > 0){
        double x_acc = m_old_x + ax*dt*dt/2.0;
        double y_acc = m_old_y + ay*dt*dt/2.0;
        double z_acc = m_old_z + az*dt*dt/2.0;
        
        double new_x = (x+x_acc+x_v)/3.0;
        double new_y = (y+y_acc+y_v)/3.0;
        double new_z = (z+z_acc)/2.0;
        
        m_old_x = new_x;
        m_old_y = new_y;
        m_old_z = new_z;
    }  else {
        double new_x = (x+x_v)/2.0;
        double new_y = (y+y_v)/2.0;
        double new_z = z;
        
        m_old_x = new_x;
        m_old_y = new_y;
        m_old_z = new_z;
    }
}
