#ifndef LINE_AB_H
#define LINE_AB_H

#include "gps_module.hpp"
#include "curveAB.hpp"

class LineAB{
public:
    GpsPoint m_pointA;
    GpsPoint m_pointB;
    
    bool m_sensAB = true;
    
    std::string m_name;
    
    double m_a = 0;
    double m_b = 0;
    double m_c = 0;
    double m_sqrt_m_a_m_b;

    double m_ab_x = 0.0;
    double m_ab_y = 0.0;

    double m_angleAB = 0;
    
    double m_current_line = 0;
    
    void setAB();
    
    double distance(double x, double y, double lg);
    
    double anglefollowTheCarrot(double x, double y, double lg, double angleDeplacement, double lk);

    double calculRearWheelPosition(double p_x, double p_y, double lg, double angle, double deplacement_x, double deplacement_y, double vitesse, double L, double KTH, double KE);
};

class LineCurves {
public:
    std::vector<std::string> m_lines_curves;
    void load();
    void loadCurveOrLine(std::string name);
    void add(LineAB & p);
    void add(CurveAB & p);
    void save();
};

#endif // LINE_AB_H
