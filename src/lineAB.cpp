#include "lineAB.hpp"
#include <math.h>
#include "logging.hpp"
#include "util/util.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "environnement.hpp"
#include "gps_framework.hpp"

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


double LineAB::anglefollowTheCarrot(double x, double y, double lg, double angleDeplacement, double lk){
    double angleABDeplacement = m_angleAB - angleDeplacement;
    //INFO(angleABDeplacement/3.14*180);

    double distance = this->distance(x, y, lg);
    
    double angle_followKarott =  atan(distance/lk);
    double angle = angle_followKarott + angleABDeplacement;
    angle = angleBetweenPI2(angle);
    
    return angle;
}

double LineAB::calculRearWheelPosition(double p_x, double p_y, double lg, double deplacementAngle, double deplacement_x, double deplacement_y, double vitesse, double L, double KTH, double KE){
    double x_segment = m_pointB.m_x - m_pointA.m_x;
    double y_segment = m_pointB.m_y - m_pointA.m_y;
    
    double angle = -my_angle(deplacement_x, deplacement_y, x_segment, y_segment);
    angle = angleBetweenPI2(angle);
    
    double distance = this->distance(p_x, p_y, lg);
    
    double e = -distance;
    double v = vitesse*10000.0/3600.0;
    double k = 0;//todo;
    double th_e = -angle;//todo;

    double omega = v * k * cos(th_e) / (1.0 - k * e) - KTH * abs(v) * th_e - KE * v * sin(th_e) * e / th_e;
    
    if (th_e == 0.0 || omega == 0.0){
        return 0.0;
    }

    double delta = atan2(L * omega / v, 1.0);
    
    return delta;
    
}



void LineCurves::load(){
    std::string file_parcelles = ProjectSourceBin + "/parcelle/parcelles.txt";
    std::ifstream file(file_parcelles);
    std::string line;
    /*while (std::getline(file, line))
    {
        m_parcelles.push_back(line);
    }
    INFO("parcelles "  << m_parcelles.size());*/
     
}

void LineCurves::save(){
    std::ofstream file;
    std::string file_parcelles = ProjectSourceBin + "/parcelle/parcelles.txt";
    file.open(file_parcelles, std::ios::out);
    /*for(auto p : m_parcelles){
        file << p << std::endl;
    }
    INFO("parcelles "  << m_parcelles.size());*/
     
}


void LineCurves::add(LineAB & p){
    GpsFramework & f = GpsFramework::Instance();
    /*if(p.m_name.size() > 2){
        m_parcelles.push_back(p.m_name);
        p.saveParcelle(p.m_name);
        save();
    } else {
        f.addError("parcelle nom trop petit");
    }*+
}

void LineCurves::add(CurveAB & p){
    GpsFramework & f = GpsFramework::Instance();
    /*if(p.m_name.size() > 2){
        m_parcelles.push_back(p.m_name);
        p.saveParcelle(p.m_name);
        save();
    } else {
        f.addError("parcelle nom trop petit");
    }
}

