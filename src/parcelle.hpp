#ifndef PARCELLE_H
#define PARCELLE_H

#include "gps_module.hpp"
#include <vector>

class Parcelle {
public:
    std::string m_name;
    bool m_is_init = false;
    
    std::vector<GpsPoint_ptr> m_contour;
    void addPoint(GpsPoint_ptr p);
    
    void clear();
    void compute();
    void saveParcelle(std::string name);
    void loadParcelle(std::string name);
};

class Parcelles {
    public:
    std::vector<std::string> m_parcelles;
    void load();
    void add(Parcelle & p);
    void save();
};

#endif // LINE_AB_H
