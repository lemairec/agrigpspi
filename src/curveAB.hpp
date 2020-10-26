#ifndef CURVE_AB_H
#define CURVE_AB_H

#include "gps_module.hpp"
#include <list>
#include <vector>
#include <map>

class CurveAB{
public:
    GpsPoint m_pointA;
    GpsPoint m_pointB;
    
    bool m_sensAB = true;
    
    double m_a = 0;
    double m_b = 0;
    double m_c = 0;
    double m_sqrt_m_a_m_b;

    double m_ab_x = 0.0;
    double m_ab_y = 0.0;

    double m_angleAB = 0;
    
    std::vector<GpsPoint_ptr> m_listAB;
    
    std::map<int, std::vector<GpsPoint_ptr>> m_list;
    void addLine(int i);
    
    double distance(double x, double y, double lg);
    void addPoint(GpsPoint_ptr p);
    
    void savePointB();
};


#endif // LINE_AB_H
