#ifndef PARCELLE_H
#define PARCELLE_H

#include "gps_module.hpp"
#include <vector>

class Parcelle {
public:
    Parcelle();
    
    std::string m_name;
    bool m_is_init = false;
    
    std::vector<GpsPoint_ptr> m_contour;
    
    double m_bounding_rect_x, m_bounding_rect_y, m_bounding_rect_width, m_bounding_rect_height;
    double m_center_x, m_center_y;
    
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
