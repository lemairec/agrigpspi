#ifndef CURVE_AB_H
#define CURVE_AB_H

#include "gps_module.hpp"
#include <list>
#include <vector>
#include <map>

class Curve{
public:
    Curve(int i){
        m_i = i;
    }
    int m_i;
    int m_curve_i_min = 0;
    int m_curve_i_min2 = 0;
    std::vector<GpsPoint_ptr> m_points;
    std::vector<GpsPoint_ptr> m_points_simpl;
    std::vector<GpsPoint_ptr> m_points_to_draw;
};

typedef std::shared_ptr<Curve> Curve_ptr;


class CurveAB{
public:
    GpsPoint m_pointA;
    GpsPoint m_pointB;
    
    double m_largeur = 10;
    bool m_sensAB = true;
    
    std::string m_name;
    
    std::vector<GpsPoint_ptr> m_listAB;
    
    int m_i_min = 0;
    int m_i_max = 0;
    int m_i_current = 0;
    
    void computeCurve(Curve_ptr);
    void verify(int i);
    Curve_ptr getCurrentLine();
    Curve_ptr getCurrentLineRel(int i);
    
    std::map<int, Curve_ptr> m_curves;
    
    void clearAll();
    void clearWithoutAB();
    
    void addLine(int i);
    void setCurrent(int i);
    
    double distance(double x, double y, double lg);
    void addPoint(GpsPoint_ptr p);
    
    void savePointB();
    
    
    double m_curb_x, m_curb_y, m_curb_l;
    double m_curb_c_x, m_curb_c_y;
    double calculCurbature(Curve_ptr line, size_t i);
    
    double m_proj_distance = 0;
    double m_proj_prod_vect = 0;
    double m_proj_x = 0, m_proj_y = 0;
    void calculProjete2(double x, double y, double deplacement_x, double deplacement_y);
    void calculProjete2P(GpsPoint_ptr p, double deplacement_x, double deplacement_y);
    void calculProjete(GpsPoint_ptr p, double deplacement_x, double deplacement_y, bool change_line);
    
    double calculRearWheelPosition(double p_x, double p_y, double deplacement_x, double deplacement_y, double vitesse, double L, double KTH, double KE);

    
    double m_fc_x = 0, m_fc_y = 0;
    double m_fc_lh_x = 0, m_fc_lh_y = 0;
    double m_fc_xh = 0, m_fc_yh = 0;
    double anglefollowTheCarrot(double x_pont, double y_pont, double deplacement_x, double deplacement_y, double lookhead);
    
    double m_d_k;
    double m_d_angle;
    double m_d_e;
    double m_d_xk;
    double m_d_xe;
    double m_d_xthe;
    double m_d_res;
    double m_d_delta;
};

#endif // LINE_AB_H
