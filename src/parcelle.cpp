#include "parcelle.hpp"
#include <math.h>
#include "logging.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "environnement.hpp"
#include "gps_framework.hpp"

Parcelle::Parcelle(){
    clear();
}

void Parcelle::clear(){
    m_contour.clear();
    m_is_init = false;
    m_name = "";
    m_bounding_rect_x = 0;
    m_bounding_rect_y = 0;
    m_bounding_rect_width = 0;
    m_bounding_rect_height = 0;
    m_center_x = 0;
    m_center_y = 0;
};

double cp2(GpsPoint & a, GpsPoint &  b){ //returns cross product
    return a.m_x*b.m_y-a.m_y*b.m_x;
}

void Parcelle::compute(){
    double x_min = 0, y_min, x_max, y_max;
    for(auto p : m_contour){
        if(x_min == 0){
            x_min = p->m_x;
            x_max = p->m_x;
            y_min = p->m_y;
            y_max = p->m_y;
        }
        if(x_min > p->m_x){ x_min = p->m_x; }
        if(x_max < p->m_x){ x_max = p->m_x; }
        if(y_min > p->m_y){ y_min = p->m_y; }
        if(y_max < p->m_y){ y_max = p->m_y; }
    }
    //INFO(x_min << " " << x_max << " "<< y_min << " " << y_max);
    m_bounding_rect_x = x_min;
    m_bounding_rect_y = y_min;
    m_bounding_rect_width = x_max - x_min;
    m_bounding_rect_height = y_max - y_min;
    
    //INFO(m_bounding_rect_x << " " << m_bounding_rect_y << " ("<< m_bounding_rect_width << " " << m_bounding_rect_height << ")");
    m_center_x = m_bounding_rect_x+m_bounding_rect_width/2;
    m_center_y = m_bounding_rect_y+m_bounding_rect_height/2;
    
    m_is_init = true;
    
    int n = m_contour.size();
    double sum=0.0;
    for(int i=0; i<n; i++){
        sum+=cp2(*m_contour[i], *m_contour[(i+1)%n]); //%n is for last triangle
    }
    //INFO(n);
    m_surface_ha = abs(sum)/2.0/10000;
}


void Parcelle::addPoint(GpsPoint_ptr p){
    m_contour.push_back(p);
}

void Parcelle::saveParcelle(std::string name){
    std::string path = ProjectSourceBin + "/parcelle/" + name + ".txt";
    std::ofstream file;
    file.open(path, std::ios::out);
    for(auto p : m_contour){
        file << std::setprecision(11) << p->m_latitude << " " << p->m_longitude << std::endl;
    }
}


void Parcelle::loadParcelle(std::string name){
    clear();
    std::string file_job = ProjectSourceBin + "/parcelle/" + name + ".txt";
    std::ifstream file(file_job);
    std::string line;
    bool init = false;
    if(file.is_open()){
        INFO(file_job << " is open");
        GpsFramework & f = GpsFramework::Instance();
        
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
            
            if(!init){
                f.setRef(a, b);
                init = true;
            }
            f.m_gpsModule.setXY(*p);
            
            addPoint(p);
            // process pair (a,b)
        }
        m_name = name;
        compute();
    } else {
        INFO(file_job << " is close");
    }
}




void Parcelles::load(){
    std::string file_parcelles = ProjectSourceBin + "/parcelle/parcelles.txt";
    std::ifstream file(file_parcelles);
    std::string line;
    while (std::getline(file, line))
    {
        m_parcelles.push_back(line);
    }
    INFO("parcelles "  << m_parcelles.size());
     
}

void Parcelles::save(){
    std::ofstream file;
    std::string file_parcelles = ProjectSourceBin + "/parcelle/parcelles.txt";
    file.open(file_parcelles, std::ios::out);
    for(auto p : m_parcelles){
        file << p << std::endl;
    }
    INFO("parcelles "  << m_parcelles.size());
     
}

void Parcelles::add(Parcelle & p){
    GpsFramework & f = GpsFramework::Instance();
    if(p.m_name.size() > 2){
        m_parcelles.push_back(p.m_name);
        p.saveParcelle(p.m_name);
        save();
    } else {
        f.addError("parcelle nom trop petit");
    }
}

double Parcelle::distance(GpsPoint_ptr p){
    double d = 0;
    for(auto p2 : m_contour){
        double dist = p2->distanceCarre(*p);
        if(d == 0){
            d = dist;
        }
        if(dist < d){
            d = dist;
        }
    }
    return sqrt(d);
}
