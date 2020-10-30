#ifndef CURVE_AB_H
#define CURVE_AB_H

#include "gps_module.hpp"
#include <list>
#include <vector>
#include <map>

class Lines{
public:
    std::vector<GpsPoint_ptr> m_points;
};

typedef std::shared_ptr<Lines> Lines_ptr;


class CurveAB{
public:
    GpsPoint m_pointA;
    GpsPoint m_pointB;
    
    double m_largeur = 10;
    bool m_sensAB = true;
    
    std::vector<GpsPoint_ptr> m_listAB;
    
    int m_i_min = 0;
    int m_i_max = 0;
    int m_i_current = 0;
    
    
    Lines_ptr getCurrentLine();
    
    std::map<int, Lines_ptr> m_list;
    void addLine(int i);
    void setCurrent(int i);
    
    double distance(double x, double y, double lg);
    void addPoint(GpsPoint_ptr p);
    
    
    
    void savePointB();
    
    int m_curve_i_min = 0;
    int m_curve_i_min2 = 0;
    double m_distance = 0;
    double x_h = 0, y_h = 0;
    
    void calculProjete2(GpsPoint_ptr p, double deplacement_x, double deplacement_y);
    void calculProjete(GpsPoint_ptr p, double deplacement_x, double deplacement_y);
};


#endif // LINE_AB_H
