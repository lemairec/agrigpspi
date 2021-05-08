#include "lineAB.hpp"
#include <math.h>
#include "logging.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "environnement.hpp"
#include "gps_framework.hpp"
#include "util/util.hpp"
#include "util/directory_manager.hpp"

void Line::calculProjete2(double x, double y){
    double x_a = m_pointA.m_x;
    double y_a = m_pointA.m_y;
    
    double x_b = m_pointB.m_x;
    double y_b = m_pointB.m_y;
    
    double x_v = x_b-x_a;
    double y_v = y_b-y_a;
    double d_v = sqrt(x_v*x_v + y_v*y_v);
    x_v = x_v/d_v;
    y_v = y_v/d_v;
    
    
    
    double bh = (x-x_a)*x_v+(y-y_a)*y_v;
    m_proj_x = x_a + bh*x_v;
    m_proj_y = y_a + bh*y_v;
}

void LineAB::clearAll(){
    m_curves.clear();
}

void LineAB::clearWithoutAB(){
    m_curves.clear();
    m_i_min = 0;
    m_i_max = 0;
    m_i_current = 0;
    
    savePointB();
}

Line_ptr LineAB::getCurrentLine(){
    return m_curves[m_i_current];
}

Line_ptr LineAB::getCurrentLineRel(int i){
    int res = m_i_current+i;
    verify(res);
    return m_curves[res];
}

void LineAB::verify(int i){
    if(i<m_i_min){
        for(int j = m_i_min-1; j >= i-10; --j){
            addLine(j);
        }
    }
    if(i>m_i_max){
        for(int j = m_i_max+1; j <= i+10; ++j){
            addLine(j);
        }
    }
}

void LineAB::addLine(int i){
    int j=i-1;
    int j2=i-2;
    if(i <0){
        j=i+1;
        j2=i+2;
    }
    
    m_curves[i] = Line_ptr(new Line(i));
    if(i<m_i_min){
        m_i_min = i;
    }
    if(i>m_i_max){
        m_i_max = i;
    }
    
    GpsPoint_ptr old_point = nullptr;
    
    m_x_ab = m_pointB.m_x - m_pointA.m_x;
    m_y_ab = m_pointB.m_y - m_pointA.m_y;
    double d  = std::sqrt(m_x_ab*m_x_ab + m_y_ab*m_y_ab);
    m_x_ab = m_x_ab/d;
    m_y_ab = m_y_ab/d;
    
    m_curves[i]->m_pointA.m_x = m_pointA.m_x + m_y_ab*(i*m_largeur+m_offset);
    m_curves[i]->m_pointA.m_y = m_pointA.m_y - m_x_ab*(i*m_largeur+m_offset);
    m_curves[i]->m_pointB.m_x = m_pointB.m_x + m_y_ab*(i*m_largeur+m_offset);
    m_curves[i]->m_pointB.m_y = m_pointB.m_y - m_x_ab*(i*m_largeur+m_offset);
    computeCurve(m_curves[i]);
}

void LineAB::computeCurve(Line_ptr curve){
}

void LineAB::setCurrent(int i){
    verify(i);
    m_i_current = i;
}

void LineAB::savePointB(){
    m_curves.clear();
    GpsPoint_ptr old_point = nullptr;
    
    addLine(0);
    
    for(int i = 1; i < 11; ++i){
        addLine(i);
        addLine(-i);
    }
}


double LineAB::calculCurbature(Line_ptr line, size_t i){
    return 1.0;
}



void LineAB::calculProjete2P(GpsPoint_ptr p, double deplacement_x, double deplacement_y){
    calculProjete2(p->m_x, p->m_y, deplacement_x, deplacement_y);
}



void LineAB::calculProjete2(double x, double y, double deplacement_x, double deplacement_y){
    Line_ptr list = getCurrentLine();
    
    double x_a = x;
    double y_a = y;
    
    double x_b = list->m_pointA.m_x;
    double y_b = list->m_pointA.m_y;
    double x_m = list->m_pointB.m_x;
    double y_m = list->m_pointB.m_y;

    //https://fr.wikipedia.org/wiki/Projection_orthogonale
    double x_v = x_m-x_b;
    double y_v = y_m-y_b;
    m_proj_x_segment = x_v;
    m_proj_y_segment = y_v;
    double d_v = sqrt(x_v*x_v + y_v*y_v);
    x_v = x_v/d_v;
    y_v = y_v/d_v;
    
    
    
    double bh = (x_a-x_b)*x_v+(y_a-y_b)*y_v;
    m_proj_x = x_b + bh*x_v;
    m_proj_y = y_b + bh*y_v;
    
    
    double ah = sqrt((m_proj_x-x_a)*(m_proj_x-x_a) + (m_proj_y-y_a)*(m_proj_y-y_a));
    m_proj_distance = ah;
    
    m_proj_prod_vect = deplacement_x*x_v+deplacement_y*y_v;
    
    double det = (m_proj_x-x_a)*deplacement_y-(m_proj_y-y_a)*deplacement_x;
    
    if(det < 0){
        m_proj_distance = -m_proj_distance;
    }
}

double LineAB::getOffsetAB(GpsPoint_ptr p){
    Line_ptr list = getCurrentLine();
    
    double x_a = p->m_x;
    double y_a = p->m_y;
    
    double x_b = list->m_pointA.m_x;
    double y_b = list->m_pointA.m_y;
    double x_m = list->m_pointB.m_x;
    double y_m = list->m_pointB.m_y;

    //https://fr.wikipedia.org/wiki/Projection_orthogonale
    double x_v = x_m-x_b;
    double y_v = y_m-y_b;
    m_proj_x_segment = x_v;
    m_proj_y_segment = y_v;
    double d_v = sqrt(x_v*x_v + y_v*y_v);
    x_v = x_v/d_v;
    y_v = y_v/d_v;
    
    
    
    double bh = (x_a-x_b)*x_v+(y_a-y_b)*y_v;
    m_proj_x = x_b + bh*x_v;
    m_proj_y = y_b + bh*y_v;
    
    
    double ah = sqrt((m_proj_x-x_a)*(m_proj_x-x_a) + (m_proj_y-y_a)*(m_proj_y-y_a));
    m_proj_distance = ah;
    
    double deplacement_x = m_pointB.m_x - m_pointA.m_x;
    double deplacement_y = m_pointB.m_y - m_pointA.m_y;
    
    m_proj_prod_vect = deplacement_x*x_v+deplacement_y*y_v;
    
    double det = (m_proj_x-x_a)*deplacement_y-(m_proj_y-y_a)*deplacement_x;
    
    if(det < 0){
        m_proj_distance = -m_proj_distance;
    }
    
    return m_proj_distance;
}

void LineAB::calculProjete(GpsPoint_ptr p, double deplacement_x, double deplacement_y, bool change_line){
    calculProjete2P(p, deplacement_x, deplacement_y);
    double dist = abs(m_proj_distance);
    if(change_line && dist > m_largeur/2){
        double temp_x_h = m_proj_x;
        double temp_y_h = m_proj_y;
        double temp_i = m_i_current;
        double temp_distance = m_proj_distance;
        
        setCurrent(m_i_current + 1);
        calculProjete2P(p, deplacement_x, deplacement_y);
        double dist2 = abs(m_proj_distance);
        if(dist2 < dist){
            return;
        }
        setCurrent(m_i_current - 2);
        calculProjete2P(p, deplacement_x, deplacement_y);
        double dist3 = abs(m_proj_distance);
        if(dist3 < dist){
            return;
        }
        setCurrent(temp_i);
        m_proj_x = temp_x_h;
        m_proj_y = temp_y_h;
        m_proj_distance = temp_distance;
    }
}




double LineAB::anglefollowTheCarrot(double x, double y, double deplacement_x, double deplacement_y, double lookhead){
    m_fc_x = x;
    m_fc_y = y;
    
    Line_ptr list = getCurrentLine();
    if(list == NULL){
        return 0;
    }
    calculProjete2(x, y, deplacement_x, deplacement_y);
    m_fc_xh = m_proj_x;
    m_fc_yh = m_proj_y;
    
    /**
     * Follow carrot PT LOOKHEAD
     */
    double x_v2 = m_pointB.m_x - m_pointA.m_x;
    double y_v2 = m_pointB.m_y - m_pointA.m_y;
    
    double d_v2 = sqrt(x_v2*x_v2 + y_v2*y_v2);
    x_v2 = x_v2/d_v2;
    y_v2 = y_v2/d_v2;
    double d2 = deplacement_x*x_v2+deplacement_y*y_v2;
    if(d2 >0){
        m_fc_lh_x = m_fc_xh + lookhead*x_v2;
        m_fc_lh_y = m_fc_yh + lookhead*y_v2;
    } else {
        m_fc_lh_x = m_fc_xh - lookhead*x_v2;
        m_fc_lh_y = m_fc_yh - lookhead*y_v2;
    }
    
    /**
     * Follow carrot CALCUL ANGLE
     */

    double x_segment = m_fc_lh_x - m_fc_x;
    double y_segment = m_fc_lh_y - m_fc_y;

    double angle = my_angle(x_segment, y_segment, deplacement_x, deplacement_y);
    angle = angleBetweenPI2(angle);

    return angle;
}

double LineAB::calculRearWheelPosition(double x_pont, double y_pont, double x, double y, double deplacement_x, double deplacement_y, double vitesse, double L, double KTH, double KE){
    double v = vitesse*10000.0/3600.0;
    
    Line_ptr list = getCurrentLine();
    if(list == NULL){
        return 0;
    }
    calculProjete2(x_pont, y_pont, deplacement_x, deplacement_y);
    double e = -m_proj_distance;
    
    calculProjete2(x, y, deplacement_x, deplacement_y);
    double x_segment = m_proj_x_segment;
    double y_segment = m_proj_y_segment;
    
    
    
    double k = 0;
    double d = m_proj_prod_vect;
    if(d < 0){
        k = -k;
    }
    
    double angle = my_angle(deplacement_x, deplacement_y, x_segment, y_segment);
    angle = angleBetweenPI2(angle);
    double th_e = angle;//todo;

    double xk = v * k * cos(th_e) / (1.0 - k * e) ;
    double xthe = KTH * abs(v) * th_e ;
    double xe = KE * v * sin(th_e) * e / th_e;

    //std::cout << "l  e " << e << "  th_e " << th_e << std::endl;
    //std::cout << "l    " << x2 << " " << x3 << std::endl;
    double omega = xk - xe - xthe;


    m_d_e = e;
    m_d_angle = th_e;
    m_d_k = k;

    m_d_xk = xk;
    m_d_xthe = -xthe;
    m_d_xe = -xe;
    m_d_res = omega;

    double delta = atan2(L * omega / v, 1.0);

    m_d_delta = delta;

    return delta;
}





void LineCurves::load(){
    std::string file_parcelles = DirectoryManager::Instance().getBinDirectory() + "/line/lines.txt";
    std::ifstream file(file_parcelles);
    std::string line;
    while (std::getline(file, line))
    {
        m_lines_curves.push_back(line);
    }
    INFO("lines_curves "  << m_lines_curves.size());
}

void LineCurves::save(){
    std::ofstream file;
    std::string file_parcelles = DirectoryManager::Instance().getBinDirectory() + "/line/lines.txt";
    file.open(file_parcelles, std::ios::out);
    for(auto p : m_lines_curves){
        file << p << std::endl;
    }
    INFO("lines_curves "  << m_lines_curves.size());
}

void LineCurves::loadCurveOrLine(std::string name){
    GpsFramework & f = GpsFramework::Instance();
    
    std::string file_job = DirectoryManager::Instance().getBinDirectory() + "/line/" + name + ".txt";
    std::ifstream file(file_job);
    std::string line;
    
    if(file.is_open()){
        
        std::getline(file, line);
        if(line == "LINE"){
            
        }
        
        std::vector<GpsPoint_ptr> l;
        while (std::getline(file, line))
        {
            std::istringstream iss(line);
            double a, b;
            if (!(iss >> a >> b)) {
                INFO("error");
                break;
                
            } // error
            //INFO(a << " " << b);
            GpsPoint_ptr p = GpsPoint_ptr(new GpsPoint());
            p->m_latitude = a;
            p->m_longitude = b;
            l.push_back(p);
            // process pair (a,b)
        }
        
        if(l.size() == 2){
            INFO("line");
            f.m_line = true;
            f.m_lineAB.m_pointA = *(l[0]);
            f.m_lineAB.m_pointB = *(l[1]);
            f.setAB();
        } else {
            f.m_line = false;
            f.m_curveAB.clearAll();
            for(auto p : l){
                f.m_curveAB.m_listAB.push_back(p);
            }
            f.setAB();
            //m_etat = Etat_OK;
            
        }
        /*m_name = name;
        compute();*/
    } else {
        INFO(file_job << " is close");
    }
}



void LineCurves::add(LineAB & p){
    GpsFramework & f = GpsFramework::Instance();
    if(p.m_name.size() > 2){
        std::string name = "line_" + p.m_name;
        m_lines_curves.push_back(name);
        
        std::string path = DirectoryManager::Instance().getBinDirectory() + "/line/" + name + ".txt";
        std::ofstream file;
        file.open(path, std::ios::out);
        file << "LINE" << std::endl;
        file << std::setprecision(14) << p.m_pointA.m_latitude << " " << p.m_pointA.m_longitude << std::endl;
        file << std::setprecision(14) << p.m_pointB.m_latitude << " " << p.m_pointB.m_longitude << std::endl;
        save();
    } else {
        f.addError("line nom trop petit");
    }
}

void LineCurves::add(CurveAB & p){
    GpsFramework & f = GpsFramework::Instance();
    if(p.m_name.size() > 2){
        std::string name = "curve_" + p.m_name;
        m_lines_curves.push_back(name);
        
        std::string path = DirectoryManager::Instance().getBinDirectory() + "/line/" + name + ".txt";
        std::ofstream file;
        file.open(path, std::ios::out);
        file << "CURVE" << std::endl;
        for(auto p : f.m_curveAB.m_listAB){
            file << std::setprecision(14) << p->m_latitude << " " << p->m_longitude << std::endl;
        }
        save();
    } else {
        f.addError("curve nom trop petit");
    }
}

