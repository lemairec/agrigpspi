#include "lineAB.hpp"
#include <math.h>
#include "logging.hpp"

double LineAB::distance(double x, double y, double lg){
    if(m_pointA.m_x!=0 && m_pointB.m_x!=0){
        double dist = (m_a * x + m_b * y + m_c)/m_sqrt_m_a_m_b;
        //INFO(dist);
        m_current_line = round(dist/lg);
        if(!m_sensAB){
            dist = -dist;
        }
        dist = fmod(dist, lg);
        if(dist > lg/2){
            dist -= lg;
        }
        if(dist < -lg/2){
         dist += lg;
        }
        if(dist < -lg/2){
         dist += lg;
        }
        if(dist > lg/2){
         dist -= lg;
        }
        return dist;
    }
    return 0.0;
    
}


double LineAB::anglefollowTheCarrot(double x, double y, double lg, double lk){
    double distance = this->distance(x, y, lg);
    return atan(distance/lk);
}

double LineAB::calculRearWheelPosition(double p_x, double p_y, double lg, double angle, double deplacement_x, double deplacement_y, double vitesse, double L, double KTH, double KE){
    double distance = this->distance(p_x, p_y, lg);
    
    
    double e = -distance;
    double v = vitesse*10000.0/3600.0;
    double k = 0;//todo;
    double th_e = -angle;//todo;

    double omega = - KTH * abs(v) * th_e - KE * v * sin(th_e) * e / th_e;
    
    
    
    if (th_e == 0.0 || omega == 0.0){
        return 0.0;
    }

    double delta = atan2(L * omega / v, 1.0);
    
    return delta;
    
}
