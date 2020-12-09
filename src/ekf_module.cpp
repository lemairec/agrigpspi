#include "ekf_module.hpp"

#include "logging.hpp"
#include <math.h>

void EkfModule::initOrLoad(Config & config){
    m_ekf_mode = (EkfMode)config.m_ekf;
    m_coeff_lissage = config.m_ekf_coeff_lissage;
}


void EkfModule::onNewEkfPoint(double x, double y, double z, double v, double ang, double ax, double ay, double az){
    double dt = 0.1;
    
    if(m_ekf_mode == EkfWithoutImu ){
        double new_v_x = m_old_x + sin(ang)*v*dt;
        double new_v_y = m_old_y + cos(ang)*v*dt;
        
        double new_x = ((1.0-m_coeff_lissage)*x+m_coeff_lissage*new_v_x);
        double new_y = ((1.0-m_coeff_lissage)*y+m_coeff_lissage*new_v_y);
        double new_z = z;
        
        m_old_x = new_x;
        m_old_y = new_y;
        m_old_z = new_z;
    } else if(m_ekf_mode == Ekf1 ){
        double v_x = sin(ang)*v;
        double v_y = cos(ang)*v;
        
        double v_acc_x = v_x + sin(ang)*ax*dt;
        double v_acc_y = v_y + cos(ang)*ay*dt;
        
        
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
    } else if(m_ekf_mode == Ekf2){
        double v_inst = sqrt((x-m_old_x)/dt*(x-m_old_x)/dt+(y-m_old_y)/dt*(y-m_old_y)/dt );
        double a_inst = atan((y-m_old_y)/(x-m_old_x));
        
        double v_acc_x =  + sin(ang)*ax*dt;
        double v_acc_y = (y-m_old_y)/dt + cos(ang)*ay*dt;
        
        m_v_x = (m_coeff_lissage)*m_v_x + (1.0-m_coeff_lissage)*v_acc_x;
        m_v_y = (m_coeff_lissage)*m_v_y + (1.0-m_coeff_lissage)*v_acc_y;
        
        double new_v_x = m_old_x + m_v_x*dt;
        double new_v_y = m_old_y + m_v_y*dt;
        
        double new_x = ((1.0-m_coeff_lissage)*x+m_coeff_lissage*new_v_x);
        double new_y = ((1.0-m_coeff_lissage)*y+m_coeff_lissage*new_v_y);
        double new_z = z;
        
        m_old_x = new_x;
        m_old_y = new_y;
        m_old_z = new_z;
    } else {
        m_old_x = x;
        m_old_y = y;
    }
    
    if(((m_old_x - x)*(m_old_x - x) + (m_old_y - y)*(m_old_y - y)) > 1.0){
        m_old_x = x;
        m_old_y = y;
    }
    
    
}

void EkfModule::calculDeplacement(GpsPoint_ptr p){
    m_ekf_module.m_list.push_front(p);
    if(m_ekf_module.m_list.size()>100){
        m_ekf_module.m_list.pop_back();
    };
}

/*
 
 */
