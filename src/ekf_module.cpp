#include "ekf_module.hpp"
#include "gps_framework.hpp"

#include "logging.hpp"
#include <math.h>

const double vitesse_mini_km_h = 0.5;
const double vitesse_mini_m_s = vitesse_mini_km_h*1000.0/3600.0;

void EkfModule::initOrLoad(Config & config){
    m_ekf_mode = (EkfMode)config.m_ekf;
    m_coeff_lissage = config.m_ekf_coeff_lissage;
    m_ekf_correction_devers = config.m_ekf_correction_devers;
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


void EkfModule::onNewEkfPoint(double x, double y, double z, double ax, double ay, double az){
    if(m_reset){
        m_old_x = x;
        m_old_y = y;
        m_reset = false;
    }
    
    GpsFramework & f = GpsFramework::Instance();
    double dt = 0.1;
    double ang = m_deplacementAngle;
    double v = m_v;
    
    if(m_ekf_mode == Custom3m ){
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
    } else if(m_ekf_mode == EkfWithoutImu ){
        double new_v_x = m_old_x + sin(ang)*v*dt;
        double new_v_y = m_old_y + cos(ang)*v*dt;
        
        double new_x = ((1.0-m_coeff_lissage)*x+m_coeff_lissage*new_v_x);
        double new_y = ((1.0-m_coeff_lissage)*y+m_coeff_lissage*new_v_y);
        double new_z = z;
        
        m_v_x = (m_coeff_lissage)*m_v_x + (1.0-m_coeff_lissage)*(new_x-m_old_x)/0.1;
        m_v_y = (m_coeff_lissage)*m_v_y + (1.0-m_coeff_lissage)*(new_y-m_old_y)/0.1;
        
        m_old_x = new_x;
        m_old_y = new_y;
        m_old_z = new_z;
    } else if(m_ekf_mode == Ekf1 ){
        /*
        GpsFramework & f = GpsFramework::Instance();
        
        double new_v_x = m_old_x + m_v_x*dt;
        double new_v_y = m_old_y + m_v_y*dt;
        
        double new_x = ((1.0-m_coeff_lissage)*x+m_coeff_lissage*new_v_x);
        double new_y = ((1.0-m_coeff_lissage)*y+m_coeff_lissage*new_v_y);
        double new_z = z;
        
        double cos_a = cos(m_deplacementAngle);
        double sin_a = sin(m_deplacementAngle);
        
        double v_acc_x = m_v_x + cos_a*f.m_imuModule.m_ax*dt + sin_a*f.m_imuModule.m_ay*dt;
        double v_acc_y = m_v_y + sin_a*f.m_imuModule.m_ax*dt + cos_a*f.m_imuModule.m_ay*dt;
        
        double v_inst_x = (new_x-m_old_x)/0.1;
        double v_inst_y = (new_y-m_old_y)/0.1;
        
        
        m_v_x = m_coeff_lissage*m_v_x + (1.0-m_coeff_lissage)*(v_acc_x+v_inst_x)*0.5;
        m_v_y = m_coeff_lissage*m_v_y + (1.0-m_coeff_lissage)*(v_acc_y+v_inst_y)*0.5;
        
        m_old_x = new_x;
        m_old_y = new_y;
        m_old_z = 0;*/
        double old_x = m_old_x;
        double old_y = m_old_y;
               
        double v_inst = sqrt(m_v_x*m_v_x+m_v_y*m_v_y) + m_a_x*dt;
               
        m_the = atan2(m_v_y,m_v_x)  + m_the_v*dt;
        
        double v_x = v_inst*cos(m_the);
        double v_y = v_inst*sin(m_the);
        
        m_old_x = m_old_x + v_x*dt;
        m_old_y = m_old_y + v_y*dt;
        
        m_old_x = m_coeff_lissage*m_old_x + (1.0-m_coeff_lissage)*x;
        m_old_y = m_coeff_lissage*m_old_y + (1.0-m_coeff_lissage)*y;
        m_the_v = m_coeff_lissage*m_the_v + (1.0-m_coeff_lissage)*f.m_imuModule.m_a_v_z/180*3.14;
        m_a_x = m_coeff_lissage*m_a_x + (1.0-m_coeff_lissage)*f.m_imuModule.m_ax;
        
        m_v_x = m_coeff_lissage*m_v_x + (1.0-m_coeff_lissage)*(v_x+(m_old_x - old_x)/dt)*0.5;
        m_v_y = m_coeff_lissage*m_v_y + (1.0-m_coeff_lissage)*(v_y+(m_old_y - old_y)/dt)*0.5;
        
        m_pitch_y_deg = m_coeff_lissage*m_pitch_y_deg + (1.0-m_coeff_lissage)*f.m_imuModule.m_pitch_y_deg;
    } else if(m_ekf_mode == Ekf2){
        double new_v_x = m_old_x + m_v_x*dt;
        double new_v_y = m_old_y + m_v_y*dt;
        
        double new_x = ((1.0-m_coeff_lissage)*x+m_coeff_lissage*new_v_x);
        double new_y = ((1.0-m_coeff_lissage)*y+m_coeff_lissage*new_v_y);
        double new_z = z;
        
        double v_inst = m_v-f.m_imuModule.m_ay*dt;
        double cos_a = cos(3.14/2-m_deplacementAngle + f.m_imuModule.m_a_v_z/180*3.14*dt);
        double sin_a = sin(3.14/2-m_deplacementAngle + f.m_imuModule.m_a_v_z/180*3.14*dt);
        
        
        double v_acc_x = m_v_x + cos_a*v_inst*dt;
        double v_acc_y = m_v_y + sin_a*v_inst*dt;
        
        double v_inst_x = (new_x-m_old_x)/0.1;
        double v_inst_y = (new_y-m_old_y)/0.1;
        
        
        m_v_x = m_coeff_lissage*m_v_x + (1.0-m_coeff_lissage)*(v_acc_x+v_inst_x)*0.5;
        m_v_y = m_coeff_lissage*m_v_y + (1.0-m_coeff_lissage)*(v_acc_y+v_inst_y)*0.5;
        
        m_pitch_y_deg = m_coeff_lissage*m_pitch_y_deg + (1.0-m_coeff_lissage)*f.m_imuModule.m_pitch_y_deg;
        
        //INFO(m_pitch_y_deg);
        
        m_old_x = new_x;
        m_old_y = new_y;
        m_old_z = new_z;
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
    
    /*double erreur = sqrt((x-m_old_x)*(x-m_old_x) + (y-m_old_y)*(y-m_old_y));
    m_erreurs.push_back(erreur);
    
    double moy = 0;
    for(auto s : m_erreurs){
        moy += s;
    }
    INFO(moy/m_erreurs.size());*/
    
    m_v = sqrt(m_v_x*m_v_x + m_v_y*m_v_y);
    if(m_v_x != 0 && m_v_y != 0 && m_v>vitesse_mini_m_s){
        m_deplacementAngle = atan2(m_v_y,m_v_x);
    }
    
}

void EkfModule::calculDeplacement(GpsPoint_ptr p, Tracteur & tracteur){
    GpsPoint_ptr p2(new GpsPoint());
    m_list.push_front(p);
    if(m_list.size()>100){
        m_list.pop_back();
    };
    
    p2->m_x = p->m_x;
    p2->m_y = p->m_y;
    p2->m_timeHour = p->m_timeHour;
    
    if(m_ekf_correction_devers){
        tracteur.m_correction_lateral_imu = sin(m_pitch_y_deg/180.0*3.14)*tracteur.m_hauteur_antenne;
    } else {
        tracteur.m_correction_lateral_imu = 0;
    }
    tracteur.m_correction_lateral = tracteur.m_correction_lateral_imu + tracteur.m_antenne_lateral;
    p2->m_x = p2->m_x + sin(m_deplacementAngle)*tracteur.m_correction_lateral;
    p2->m_y = p2->m_y - cos(m_deplacementAngle)*tracteur.m_correction_lateral;
    m_list_tracteur.push_front(p2);
    if(m_list_tracteur.size()>100){
        m_list_tracteur.pop_back();
    };
    
    onNewEkfPoint(p2->m_x, p2->m_y, 0, 0, 0, 0);
    
    
    
}

void EkfModule::reset(){
    m_list_tracteur.clear();
    m_list_ekf.clear();
    
    m_reset = true;
}

/*
 
 */
