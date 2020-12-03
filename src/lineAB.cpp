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

void LineAB::setAB(){
    m_pointA = m_point_origin_A;
    m_pointB = m_point_origin_B;
    
    double dAB = sqrt(m_pointA.distanceCarre(m_pointB));
    double xAB = (m_pointA.m_x -  m_pointB.m_x)/dAB;
    double yAB = (m_pointA.m_y -  m_pointB.m_y)/dAB;
    INFO(xAB << " " << yAB);
    
    m_pointA.m_x += yAB*m_deplacement;
    m_pointA.m_y += -xAB*m_deplacement;
    
    m_pointB.m_x += yAB*m_deplacement;
    m_pointB.m_y += -xAB*m_deplacement;
       
    m_ab_x = m_pointB.m_x - m_pointA.m_x;
    m_ab_y = m_pointB.m_y - m_pointA.m_y;
    
    
    m_a = -(m_pointB.m_y - m_pointA.m_y);
    m_b = m_pointB.m_x - m_pointA.m_x;
    m_c = -m_a * m_pointA.m_x - m_b *  m_pointA.m_y;
    m_sqrt_m_a_m_b = sqrt(m_a*m_a + m_b*m_b);
    
    if(m_ab_y != 0 && m_ab_x != 0){
        m_angleAB = atan(m_ab_x/m_ab_y);
    } else {
        m_angleAB = 0;
    }
    INFO("yb  " << std::fixed << m_pointB.m_y << " ya " << m_pointA.m_y << " xb " << m_pointB.m_x << " xa " << m_pointA.m_x);
    INFO(m_a << "*x + " << m_b << "*y + " << m_c << " = 0; " << m_sqrt_m_a_m_b);
}


double LineAB::distance(double x, double y, double deplacementX, double deplacementY, double lg){
    if(m_pointA.m_x!=0 && m_pointB.m_x!=0){
        double dist = (m_a * x + m_b * y + m_c)/m_sqrt_m_a_m_b;
        //INFO(dist);
        m_current_line = round(dist/lg);
        
        bool m_sensAB = false;
        if(m_ab_x != 0 || m_ab_y != 0){
            double det = m_a*deplacementY - m_b*deplacementX;
            //m_deplacementAngle = m_deplacementAngle+3.14;
            m_sensAB = (det < 0);
        }
        
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


double LineAB::anglefollowTheCarrot(double x, double y, double deplacementX, double deplacementY, double lg, double angleDeplacement, double lk){
    double angleABDeplacement = m_angleAB - angleDeplacement;
    //INFO(angleABDeplacement/3.14*180);

    double distance = this->distance(x, y, deplacementX, deplacementY, lg);
    
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
    
    double distance = this->distance(p_x, p_y, deplacement_x, deplacement_y, lg);
    
    double e = -distance;
    double th_e = -angle;//todo;
    double v = vitesse*10000.0/3600.0;
    double k = 0;//todo;
    
    double x1 = v * k * cos(th_e) / (1.0 - k * e) ;
    double x2 = KTH * abs(v) * th_e ;
    double x3 = KE * v * sin(th_e) * e / th_e;
    
    //std::cout << "l  e " << e << "  th_e " << th_e << std::endl;
    //std::cout << "l    " << x2 << " " << x3 << std::endl;
    double omega = x1 - x2 - x3;
    
    if (th_e == 0.0 || omega == 0.0){
        return 0.0;
    }

    double delta = atan2(L * omega / v, 1.0);
    
    return delta;
    
}



void LineCurves::load(){
    std::string file_parcelles = ProjectSourceBin + "/line/lines.txt";
    std::ifstream file(file_parcelles);
    std::string line;
    while (std::getline(file, line))
    {
        m_lines_curves.push_back(line);
    }
    INFO("lines_curves "  << m_lines_curves.size());
}

void LineCurves::save(){
    std::ofstream file;
    std::string file_parcelles = ProjectSourceBin + "/line/lines.txt";
    file.open(file_parcelles, std::ios::out);
    for(auto p : m_lines_curves){
        file << p << std::endl;
    }
    INFO("lines_curves "  << m_lines_curves.size());
}

void LineCurves::loadCurveOrLine(std::string name){
    GpsFramework & f = GpsFramework::Instance();
    
    std::string file_job = ProjectSourceBin + "/line/" + name + ".txt";
    std::ifstream file(file_job);
    std::string line;
    
    if(file.is_open()){
        
        std::getline(file, line);
        if(line == "LINE"){
            
        }
        
        std::vector<GpsPoint_ptr> l;
        while (std::getline(file, line))
        {
            std::istringstream iss(line);
            float a, b;
            if (!(iss >> a >> b)) {
                INFO("error");
                break;
                
            } // error
            INFO(a << " " << b);
            GpsPoint_ptr p = GpsPoint_ptr(new GpsPoint());
            p->m_latitude = a;
            p->m_longitude = b;
            l.push_back(p);
            // process pair (a,b)
        }
        
        if(l.size() == 2){
            INFO("line");
            f.m_lineAB.m_point_origin_A = *(l[0]);
            f.m_lineAB.m_point_origin_B = *(l[1]);
            f.setAB();
        } else {
            f.m_line = false;
            f.m_curveAB.clearAll();
            for(auto p : l){
                f.m_curveAB.m_listAB.push_back(p);
            }
            f.setAB();
            //m_etat = Etat_OK;
            
        }
        /*m_name = name;
        compute();*/
    } else {
        INFO(file_job << " is close");
    }
}



void LineCurves::add(LineAB & p){
    GpsFramework & f = GpsFramework::Instance();
    if(p.m_name.size() > 2){
        std::string name = "line_" + p.m_name;
        m_lines_curves.push_back(name);
        
        std::string path = ProjectSourceBin + "/line/" + name + ".txt";
        std::ofstream file;
        file.open(path, std::ios::out);
        file << "LINE" << std::endl;
        file << std::setprecision(14) << p.m_point_origin_A.m_latitude << " " << p.m_point_origin_A.m_longitude << std::endl;
        file << std::setprecision(14) << p.m_point_origin_B.m_latitude << " " << p.m_point_origin_B.m_longitude << std::endl;
        save();
    } else {
        f.addError("line nom trop petit");
    }
}

void LineCurves::add(CurveAB & p){
    GpsFramework & f = GpsFramework::Instance();
    if(p.m_name.size() > 2){
        std::string name = "curve_" + p.m_name;
        m_lines_curves.push_back(name);
        
        std::string path = ProjectSourceBin + "/line/" + name + ".txt";
        std::ofstream file;
        file.open(path, std::ios::out);
        file << "CURVE" << std::endl;
        for(auto p : f.m_curveAB.m_listAB){
            file << std::setprecision(14) << p->m_latitude << " " << p->m_longitude << std::endl;
        }
        save();
    } else {
        f.addError("curve nom trop petit");
    }
}

