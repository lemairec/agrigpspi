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
    std::vector<GpsPoint_ptr> m_contour_to_draw;
    std::vector<int> m_flag;
    
    double m_bounding_rect_x, m_bounding_rect_y, m_bounding_rect_width, m_bounding_rect_height;
    double m_center_x, m_center_y;
    
    double m_resolution_to_draw = 0.5;
    
    void addPoint(GpsPoint_ptr p);
    void addFlag();
    
    bool isInit(){
        return m_is_init;
    }
    void clear();
    
    
    void addContours();
    void clearContours();
    void compute();
    void calculSurface();
    void calculContourToDraw();
    
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
