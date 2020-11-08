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

void Parcelle::save(std::string path){
    std::ofstream file;
    file.open(path, std::ios::out);
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
        std::string file_parcelle = ProjectSourceBin + "/parcelle/" + p.m_name + ".txt";
        
        p.save(file_parcelle);
        save();
    } else {
        f.addError("parcelle nom trop petit");
    }
}

