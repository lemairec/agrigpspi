#include "ekf_module.hpp"

#include "logging.hpp"
#include <math.h>

void EkfModule::onNewEkfPoint(double x, double y, double z, double v, double ang, double ax, double ay, double az){
    double dt = 0.1;
    
    if(ax!=0){
        double v_x = sin(ang)*v;
        double v_y = cos(ang)*v;
        
        double v_acc_x = x/dt + sin(ang)*ax*dt;
        double v_acc_y = y/dt + cos(ang)*ay*dt;
        
        
        m_v_x = ((1.0-m_coeff_lissage)*m_v_x + m_coeff_lissage*v_acc_x);
        m_v_y = ((1.0-m_coeff_lissage)*m_v_y + m_coeff_lissage*v_acc_y);
        
        double new_v_x = m_old_x + m_v_x*dt;
        double new_v_y = m_old_y + m_v_y*dt;
        
        double new_x = ((1.0-m_coeff_lissage)*x+m_coeff_lissage*new_v_x);
        double new_y = ((1.0-m_coeff_lissage)*y+m_coeff_lissage*new_v_y);
        double new_z = z;
        
        m_old_x = new_x;
        m_old_y = new_y;
        m_old_z = new_z;
    }  else {
        double new_v_x = m_old_x + sin(ang)*v*dt;
        double new_v_y = m_old_y + cos(ang)*v*dt;
        
        double new_x = ((1.0-m_coeff_lissage)*x+m_coeff_lissage*new_v_x);
        double new_y = ((1.0-m_coeff_lissage)*y+m_coeff_lissage*new_v_y);
        double new_z = z;
        
        m_old_x = new_x;
        m_old_y = new_y;
        m_old_z = new_z;
    }
    
    if(((m_old_x - x)*(m_old_x - x) + (m_old_y - y)*(m_old_y - y)) > 1.0){
        m_old_x = x;
        m_old_y = y;
    }
}
