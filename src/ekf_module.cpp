#include "ekf_module.hpp"
#include "gps_framework.hpp"

#include "logging.hpp"
#include <math.h>

const double vitesse_mini_km_h = 0.5;
const double vitesse_mini_m_s = vitesse_mini_km_h*1000.0/3600.0;

void EkfModule::initOrLoad(Config & config){
    m_lissage_gps_mode = (LissageGpsMode)config.m_lissage_gps_mode;
    m_lissage_gps_ekf = config.m_lissage_gps_ekf;
    m_cap_mode = (CapMode)config.m_cap_mode;
    m_cap_ekf = config.m_cap_ekf;
    m_cap_custom_d = config.m_cap_custom_d;
    m_lissage_imu_mode = (LissageImuMode)config.m_lissage_imu_mode;
    m_lissage_imu_ekf = config.m_lissage_imu_ekf;
    m_imu_correction_devers = config.m_imu_correction_devers;
}

double lissageAngle(double old_value, double new_value, double coeff){
    double new_value2 = new_value;
    double old_value2 = old_value;
    
    if(old_value2 - new_value2 > 3.14){
        old_value2 = old_value2-2*3.14;
    }
    if(old_value2 - new_value2 < -3.14){
        old_value2 = old_value2+2*3.14;
    }
    if(old_value2 - new_value2 > 3.14){
        INFO("problem");
    }
    if(old_value2 - new_value2 < -3.14){
        INFO("problem");
    }
    return (1.0-coeff)*new_value+coeff*old_value;
}

std::list<double> m_erreurs;

void EkfModule::onNewImuPoint(double ax, double ay, double az, double pitch_x_deg, double pitch_y_deg){
    if(m_lissage_imu_mode == LissageImuMode_None){
        m_a_x = ax;
        m_a_y = ay;
        m_pitch_y_deg = pitch_y_deg;
    } else {
        m_a_x = m_lissage_imu_ekf*m_a_x + (1.0-m_lissage_imu_ekf)*ax;
        m_a_y = m_lissage_imu_ekf*m_a_y + (1.0-m_lissage_imu_ekf)*ay;
        m_pitch_y_deg = m_lissage_imu_ekf*m_pitch_y_deg + (1.0-m_lissage_imu_ekf)*pitch_y_deg;
    }
    
    
    
}

void EkfModule::lissageGpsPoint(double x, double y, double z){
    if(m_reset){
        m_old_x = x;
        m_old_y = y;
        m_reset = false;
    }
    
    if(m_lissage_gps_mode == LissageGpsMode_None){
        m_old_x = x;
        m_old_y = y;
        m_old_z = z;
    } else {
        m_old_x = m_lissage_gps_ekf*m_old_x + (1.0-m_lissage_gps_ekf)*x;
        m_old_y = m_lissage_gps_ekf*m_old_y + (1.0-m_lissage_gps_ekf)*y;
        m_old_z = m_lissage_gps_ekf*m_old_z + (1.0-m_lissage_gps_ekf)*z;
        
    }
};


    
    /*if(m_ekf_mode == Custom3m ){
        if(m_list_tracteur.size() > 3){
            GpsPoint_ptr point1 = m_list_tracteur.front();
            GpsPoint_ptr point2 = NULL;
            int i = 0;
            for(auto point : m_list_tracteur){
                point2 = point;
                
                double x = point1->m_x - point2->m_x;
                double y = point1->m_y - point2->m_y;
                
                double d = x*x+y*y;
                if(d>(3.0*m_coeff_lissage*3.0*m_coeff_lissage)){
                    break;
                }
                
                //point2 = NULL;
                ++i;
            }
            
            double d_x = point1->m_x - point2->m_x;
            double d_y = point1->m_y - point2->m_y;
            
            
            double time = (point1->m_timeHour - point2->m_timeHour)*3600;
            
            m_v_x = d_x/time;
            m_v_y = d_y/time;
        }
        m_old_x = x;
        m_old_y = y;
        m_pitch_y_deg = m_coeff_lissage*m_pitch_y_deg + (1.0-m_coeff_lissage)*f.m_imuModule.m_pitch_y_deg;
    } else if(m_ekf_mode == Ekf ){
        double old_x = m_old_x;
        double old_y = m_old_y;
        
        double v_inst = sqrt(m_v_x*m_v_x+m_v_y*m_v_y) + m_a_x*dt;
        
        m_the = atan2(m_v_y,m_v_x);
        
        double v_x = v_inst*cos(m_the) - m_a_y*dt*sin(m_the);
        double v_y = v_inst*sin(m_the) + m_a_y*dt*cos(m_the);
        
        m_old_x = m_old_x + v_x*dt;
        m_old_y = m_old_y + v_y*dt;
        
        m_old_x = m_coeff_lissage*m_old_x + (1.0-m_coeff_lissage)*x;
        m_old_y = m_coeff_lissage*m_old_y + (1.0-m_coeff_lissage)*y;
        m_the_v = m_coeff_lissage*m_the_v + (1.0-m_coeff_lissage)*f.m_imuModule.m_a_v_z/180*3.14;
        
        m_v_x = m_coeff_lissage*m_v_x + (1.0-m_coeff_lissage)*(m_old_x - old_x)/dt;
        m_v_y = m_coeff_lissage*m_v_y + (1.0-m_coeff_lissage)*(m_old_y - old_y)/dt;
    } else {
        m_v_x = (x-m_old_x)/0.1;
        m_v_y = (y-m_old_y)/0.1;
        
        m_old_x = x;
        m_old_y = y;
        m_pitch_y_deg = f.m_imuModule.m_pitch_y_deg;
    }
    
    if(((m_old_x - x)*(m_old_x - x) + (m_old_y - y)*(m_old_y - y)) > 1.0){
        //m_old_x = x;
        //m_old_y = y;
    }
    
    double erreur = sqrt((x-m_old_x)*(x-m_old_x) + (y-m_old_y)*(y-m_old_y));
    m_erreurs.push_back(erreur);
    
    double moy = 0;
    for(auto s : m_erreurs){
        moy += s;
    }
    //INFO(moy/m_erreurs.size());
    
    m_v = sqrt(m_v_x*m_v_x + m_v_y*m_v_y);
    if(m_v_x != 0 && m_v_y != 0 && m_v>vitesse_mini_m_s){
        m_deplacementAngle = atan2(m_v_y,m_v_x);
    }*/
void EkfModule::calculCap(){
    if(m_cap_mode == CapMode_Custom){
        if(m_list_tracteur.size() > 3){
            GpsPoint_ptr point1 = m_list_tracteur.front();
            GpsPoint_ptr point2 = NULL;
            int i = 0;
            for(auto point : m_list_tracteur){
                point2 = point;
                
                double x = point1->m_x - point2->m_x;
                double y = point1->m_y - point2->m_y;
                
                double d = x*x+y*y;
                if(d>(m_cap_custom_d*m_cap_custom_d)){
                    break;
                }
                
                
                //point2 = NULL;
                ++i;
            }
            
            double d_x = point1->m_x - point2->m_x;
            double d_y = point1->m_y - point2->m_y;
            INFO(m_list_tracteur.size() << " " << d_x << " " << d_y);
            
            
            double time = (point1->m_timeHour - point2->m_timeHour)*3600;
            
            m_v_x = d_x/time;
            m_v_y = d_y/time;
            
            m_v = sqrt(m_v_x*m_v_x + m_v_y*m_v_y);
            if(m_v_x != 0 && m_v_y != 0 && m_v>vitesse_mini_m_s){
                m_deplacementAngle = atan2(m_v_y,m_v_x);
            }
        }
    }
    if(m_cap_mode == CapMode_Ekf){
        if(m_list_tracteur.size() > 3){
            auto it = m_list_tracteur.begin();
             GpsPoint_ptr point1 = (*it);
             ++it;
             GpsPoint_ptr point2 = (*it);
            
            double d_x = point1->m_x - point2->m_x;
            double d_y = point1->m_y - point2->m_y;
            
            
            double time = (point1->m_timeHour - point2->m_timeHour)*3600;
            
            m_v_x = m_cap_ekf*m_v_x + (1.0-m_cap_ekf)*d_x/time;
            m_v_y = m_cap_ekf*m_v_y + (1.0-m_cap_ekf)*d_y/time;
            
            m_v = sqrt(m_v_x*m_v_x + m_v_y*m_v_y);
            if(m_v_x != 0 && m_v_y != 0 && m_v>vitesse_mini_m_s){
                m_deplacementAngle = atan2(m_v_y,m_v_x);
            }
        }
    }
}

void EkfModule::calculDeplacement(GpsPoint_ptr p, Tracteur & tracteur){
    if(p->m_timeHour == 0){
        return;
    }
    
    GpsPoint_ptr p2(new GpsPoint());
    p2->m_x = p->m_x;
    p2->m_y = p->m_y;
    p2->m_timeHour = p->m_timeHour;
    m_list.push_front(p);
    if(m_list.size()>100){
        m_list.pop_back();
    };
    
    lissageGpsPoint(p->m_x, p->m_y, 0);
    
    if(m_imu_correction_devers){
        tracteur.m_correction_lateral_imu = -sin(m_pitch_y_deg/180.0*3.14)*tracteur.m_hauteur_antenne;
    } else {
        tracteur.m_correction_lateral_imu = 0;
    }
    //INFO(m_pitch_y_deg);
    //INFO(tracteur.m_correction_lateral_imu);
    
    
    tracteur.m_correction_lateral = tracteur.m_correction_lateral_imu + tracteur.m_antenne_lateral;
    p2->m_x = p2->m_x + sin(m_deplacementAngle)*tracteur.m_correction_lateral;
    p2->m_y = p2->m_y - cos(m_deplacementAngle)*tracteur.m_correction_lateral;
    m_list_tracteur.push_front(p2);
    if(m_list_tracteur.size()>100){
        m_list_tracteur.pop_back();
    };
    
    calculCap();
    
    
    
}

void EkfModule::reset(){
    m_list_tracteur.clear();
    m_list_ekf.clear();
    
    m_reset = true;
}

/*
 
 */
