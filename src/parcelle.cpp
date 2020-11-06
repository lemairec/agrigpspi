#include "parcelle.hpp"
#include <math.h>
#include "logging.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "environnement.hpp"
#include "gps_framework.hpp"

void Parcelle::clear(){
    
};


void Parcelle::addPoint(GpsPoint_ptr p){
    m_contour.push_back(p);
}

void Parcelle::save(){
    std::string file_job = ProjectSourceBin + "/parcelle.job";
    std::ofstream file;
    file.open(file_job, std::ios::out);
    for(auto p : m_contour){
        file << std::setprecision(11) << p->m_latitude << " " << p->m_longitude << std::endl;
    }
}


void Parcelle::load(){
    std::string file_job = ProjectSourceBin + "/parcelle.job";
    std::ifstream file(file_job);
    std::string line;
    bool init = false;
    
    
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
    
}

void Parcelle::compute(){
    
};
