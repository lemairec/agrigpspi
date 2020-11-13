#ifndef PARCELLE_H
#define PARCELLE_H

#include "gps_module.hpp"
#include <vector>

class Parcelle {
    bool m_is_init = false;
public:
    Parcelle();
    
    std::string m_name;
    double m_surface_ha = 0;
    
    std::vector<GpsPoint_ptr> m_contour;
    std::vector<int> m_flag;
    
    double m_bounding_rect_x, m_bounding_rect_y, m_bounding_rect_width, m_bounding_rect_height;
    double m_center_x, m_center_y;
    
    void addPoint(GpsPoint_ptr p);
    void addFlag();
    
    bool isInit(){
        return m_is_init;
    }
    void clear();
    void compute();
    void saveParcelle(std::string name);
    void loadParcelle(std::string name);
    
    double distance(GpsPoint_ptr p);
};

class Parcelles {
    public:
    std::vector<std::string> m_parcelles;
    void load();
    void add(Parcelle & p);
    void save();
};

#endif // LINE_AB_H
