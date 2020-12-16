#ifndef LINE_AB_H
#define LINE_AB_H

#include "gps_module.hpp"
#include "curveAB.hpp"

class LineAB{
public:
    GpsPoint m_point_origin_A;
    GpsPoint m_point_origin_B;
    double m_deplacement = 0;
    
    GpsPoint m_pointA;
    GpsPoint m_pointB;
    
    double m_largeur = 10;
    
    std::string m_name;
    
    double m_a = 0;
    double m_b = 0;
    double m_c = 0;
    double m_sqrt_m_a_m_b;

    double m_ab_x = 0.0;
    double m_ab_y = 0.0;

    double m_angleAB = 0;
    
    double m_current_line = 0;
    
    bool isInit(){
        return m_a != 0  || m_b !=0;
    }
    
    void setAB();
    
    double m_x_h, m_y_h;
    double m_antenne_x_h, m_antenne_y_h;
    
    void calculProjete2(double x, double y, double deplacement_x, double deplacement_y);
    
    double distance(double x, double y, double deplacementX, double deplacementY);
    
    double anglefollowTheCarrot(double x, double y, double deplacementX, double deplacementY, double lk);

    double calculRearWheelPosition(double p_x, double p_y, double angle, double deplacement_x, double deplacement_y, double vitesse, double L, double KTH, double KE);
    
    double m_d_k;
    double m_d_angle;
    double m_d_e;
    double m_d_xk;
    double m_d_xe;
    double m_d_xthe;
    double m_d_res;
    double m_d_delta;
    
    double m_fc_x, m_fc_y;
    double m_fc_lh_x, m_fc_lh_y;
    double m_fc_xh = 0, m_fc_yh = 0;
    
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
