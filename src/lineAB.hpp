#ifndef LINE_AB_H
#define LINE_AB_H

#include "gps_module.hpp"
#include "curveAB.hpp"

class Line{
public:
    Line(int i){
        m_i = i;
    }
    int m_i;
    
    GpsPoint m_pointA;
    GpsPoint m_pointB;
    
    double m_proj_x, m_proj_y;
    
    void calculProjete2(double x, double y);
};

typedef std::shared_ptr<Line> Line_ptr;


class LineAB{
public:
    GpsPoint m_pointA;
    GpsPoint m_pointB;
    
    double m_largeur = 10;

    std::string m_name;
    
    //spec
    double m_x_ab , m_y_ab;
    
    int m_i_min = 0;
    int m_i_max = 0;
    int m_i_current = 0;
    
    void computeCurve(Line_ptr);
    void verify(int i);
    Line_ptr getCurrentLine();
    Line_ptr getCurrentLineRel(int i);
    
    std::map<int, Line_ptr> m_curves;
    
    void clearAll();
    void clearWithoutAB();
    
    void addLine(int i);
    void setCurrent(int i);
    
    void savePointB();
    
    
    double m_curb_x, m_curb_y, m_curb_l;
    double m_curb_c_x, m_curb_c_y;
    double calculCurbature(Line_ptr line, size_t i);
    
    double m_proj_x_segment;
    double m_proj_y_segment;
    double m_proj_distance = 0;
    double m_proj_prod_vect = 0;
    double m_proj_x = 0, m_proj_y = 0;
    void calculProjete2(double x, double y, double deplacement_x, double deplacement_y);
    void calculProjete2P(GpsPoint_ptr p, double deplacement_x, double deplacement_y);
    void calculProjete(GpsPoint_ptr p, double deplacement_x, double deplacement_y, bool change_line);
    
    double calculRearWheelPosition(double x_pont, double y_pont, double x, double y, double deplacement_x, double deplacement_y, double vitesse, double L, double KTH, double KE);

    
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
