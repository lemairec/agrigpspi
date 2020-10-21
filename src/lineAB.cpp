#include "lineAB.hpp"
#include <math.h>

double LineAB::distance(double x, double y, double lg){
    if(m_pointA.m_x!=0 && m_pointB.m_x!=0){
        double dist = (m_a * x + m_b * y + m_c)/m_sqrt_m_a_m_b;
        //INFO(dist);
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
        //INFO("distance Point AB " << dist);
        return dist;
    }
    return 0.0;
    
}
