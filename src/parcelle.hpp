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
    void save();
    void load();
};

#endif // LINE_AB_H
