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
    m_contour.clear();
    m_is_init = false;
    m_name = "";
};


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
        m_is_init = true;
    } else {
        INFO(file_job << " is close");
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
        p.saveParcelle(p.m_name);
        save();
    } else {
        f.addError("parcelle nom trop petit");
    }
}

