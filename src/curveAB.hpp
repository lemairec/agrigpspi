#ifndef CURVE_AB_H
#define CURVE_AB_H

#include "gps_module.hpp"
#include <list>
#include <vector>
#include <map>

class Lines{
public:
    Lines(int i){
        m_i = i;
    }
    int m_i;
    int m_curve_i_min = 0;
    int m_curve_i_min2 = 0;
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
    
    void verify(int i);
    Lines_ptr getCurrentLine();
    Lines_ptr getCurrentLineRel(int i);
    
    std::map<int, Lines_ptr> m_list;
    void addLine(int i);
    void setCurrent(int i);
    
    double distance(double x, double y, double lg);
    void addPoint(GpsPoint_ptr p);
    
    void loadABCurve();
    void saveABCurve();
    
    
    void savePointB();
    
    double m_distance = 0;
    double x_h = 0, y_h = 0;
    double m_distance_pont = 0;
    
    void calculProjete2(GpsPoint_ptr p, double deplacement_x, double deplacement_y);
    void calculProjete(GpsPoint_ptr p, double deplacement_x, double deplacement_y);
    
    double x_h_pont = 0, y_h_pont = 0;
    double x_h_lookhead = 0, y_h_lookhead = 0;
    double m_angle = 0;
    void calculProjetePont(double x_pont, double y_pont, double deplacement_x, double deplacement_y, double lookhead);
};


#endif // LINE_AB_H
