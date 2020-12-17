#include "curveAB.hpp"
#include <math.h>
#include "logging.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "environnement.hpp"
#include "util/util.hpp"

const double longeur = 0.5;


void CurveAB::clearAll(){
    m_curves.clear();
    m_listAB.clear();
}

void CurveAB::clearWithoutAB(){
    m_curves.clear();
    savePointB();
}

double CurveAB::distance(double x, double y, double lg){
    return 0.0;
    
}

Curve_ptr CurveAB::getCurrentLine(){
    return m_curves[m_i_current];
}

Curve_ptr CurveAB::getCurrentLineRel(int i){
    int res = m_i_current+i;
    verify(res);
    return m_curves[res];
}

void CurveAB::verify(int i){
    if(i<m_i_min){
        for(int j = m_i_min-1; j >= i-2; --j){
            addLine(j);
        }
    }
    if(i>m_i_max){
        for(int j = m_i_max+1; j <= i+2; ++j){
            addLine(j);
        }
    }
}
             
void CurveAB::addPoint(GpsPoint_ptr p){
    m_listAB.push_back(p);
}

void clearLine(std::vector<GpsPoint_ptr> & l){
    auto l2 = l;
    l.clear();
    GpsPoint_ptr old_point = nullptr;
    for(auto p : l2){
        if(old_point){
            double dx = p->m_x - old_point->m_x;
            double dy = p->m_y - old_point->m_y;
            
            double res = dx*dx+dy*dy;
            if(res > longeur*longeur){
                l.push_back(p);
                old_point = p;
            }
        } else {
            l.push_back(p);
            old_point = p;
        }
    }
    
}

void addPoints(std::vector<GpsPoint_ptr> & l){
    auto l2 = l;
    l.clear();
    GpsPoint_ptr old_point = nullptr;
    for(auto p : l2){
        if(old_point){
            double dx = p->m_x - old_point->m_x;
            double dy = p->m_y - old_point->m_y;
            
            double res = dx*dx+dy*dy;
            if(res > longeur*longeur){
                GpsPoint_ptr p3(new GpsPoint());
                p3->m_x = (p->m_x + old_point->m_x)/2;
                p3->m_y = (p->m_y + old_point->m_y)/2;
                
                l.push_back(p3);
                l.push_back(p);
                old_point = p;
            }
        } else {
            l.push_back(p);
            old_point = p;
        }
    }
    
}

void CurveAB::addLine(int i){
    int j=i-1;
    int j2=i-2;
    if(i <0){
        j=i+1;
        j2=i+2;
    }
    
    
    //INFO("  ");
    //INFO("  ");
    //INFO("ici " << i << " " << j);
    //INFO(m_list[j].size());
    m_curves[i] = Curve_ptr(new Curve(i));
    if(i<m_i_min){
        m_i_min = i;
    }
    if(i>m_i_max){
        m_i_max = i;
    }
    
    GpsPoint_ptr old_point = nullptr;
    for(auto p : m_curves[j]->m_points){
        if(old_point){
            
            double dx = p->m_x - old_point->m_x;
            double dy = p->m_y - old_point->m_y;
            
            double temp = sqrt(dx*dx + dy*dy);
            
            GpsPoint_ptr p2(new GpsPoint());
            p2->m_x = (old_point->m_x + p->m_x)/2 + dy/temp*m_largeur;
            p2->m_y = (old_point->m_y + p->m_y)/2 - dx/temp*m_largeur;
            
            GpsPoint_ptr p3(new GpsPoint());
            p3->m_x = (old_point->m_x + p->m_x)/2 - dy/temp*m_largeur;
            p3->m_y = (old_point->m_y + p->m_y)/2 + dx/temp*m_largeur;
            
            if(m_curves[i]->m_points.size() > 0){
                double d2 = m_curves[i]->m_points.back()->distanceCarre(*p2);
                double d3 = m_curves[i]->m_points.back()->distanceCarre(*p3);
                
                if(d2 < d3){
                    auto r = p3;
                    p3 = p2;
                    p2 = r;
                }
                
            } else {
                double d2 = m_curves[j2]->m_points.front()->distanceCarre(*p2);
                double d3 = m_curves[j2]->m_points.front()->distanceCarre(*p3);
                //INFO(d2 << " " << d3);
                if(d2 > d3){
                    p3 = p2;
                }
            }
            m_curves[i]->m_points.push_back(p3);
            
            
        }
        old_point = p;
    }
    computeCurve(m_curves[i]);
}

void CurveAB::computeCurve(Curve_ptr curve){
    clearLine(curve->m_points);
    addPoints(curve->m_points);
    addPoints(curve->m_points);
    
    for(size_t i = 0; i< curve->m_points.size(); ++i){
        //if(i%5 == 0 || i == (curve->m_points.size()-1)){
            curve->m_points_simpl.push_back(curve->m_points[i]);
        //}
    }
    
    if(curve->m_points.size() > 5){
        GpsPoint_ptr p0 = curve->m_points[0];
        GpsPoint_ptr p1 = curve->m_points[1];
    
        double d_x = p1->m_x - p0->m_x;
        double d_y = p1->m_y - p0->m_y;
        double d = sqrt(d_x*d_x+d_y*d_y);
        
        d_x = d_x/d;
        d_y = d_y/d;
        
        for(int i = -10; i < 0; ++i){
            GpsPoint_ptr p(new GpsPoint());
            p->m_x = p0->m_x+d_x*10*i;
            p->m_y = p0->m_y+d_y*10*i;
            
            curve->m_points_to_draw.push_back(p);
        }
    }
    for(auto p : curve->m_points_simpl){
        curve->m_points_to_draw.push_back(p);
    }
    
    if(curve->m_points.size() > 5){
        
        size_t l = curve->m_points.size()-2;
        GpsPoint_ptr p0 = curve->m_points[l];
        GpsPoint_ptr p1 = curve->m_points[l+1];
    
        double d_x = p1->m_x - p0->m_x;
        double d_y = p1->m_y - p0->m_y;
        double d = sqrt(d_x*d_x+d_y*d_y);
        
        d_x = d_x/d;
        d_y = d_y/d;
        
        for(int i = 0; i < 10; ++i){
            GpsPoint_ptr p(new GpsPoint());
            p->m_x = p0->m_x+d_x*10*i;
            p->m_y = p0->m_y+d_y*10*i;
            
            curve->m_points_to_draw.push_back(p);
        }
    }
    
}

void CurveAB::setCurrent(int i){
    verify(i);
    m_i_current = i;
}

void CurveAB::savePointB(){
    clearLine(m_listAB);
    m_curves.clear();
    GpsPoint_ptr old_point = nullptr;
    
    m_curves[0] = Curve_ptr(new Curve(0));
    m_curves[1] = Curve_ptr(new Curve(1));
    m_curves[-1] = Curve_ptr(new Curve(-1));
    m_i_max = -1;
    m_i_max = 1;
    for(auto p : m_listAB){
        if(old_point){
            
            double dx = p->m_x - old_point->m_x;
            double dy = p->m_y - old_point->m_y;
            
            double temp = sqrt(dx*dx + dy*dy);
            
            GpsPoint_ptr p2(new GpsPoint());
            p2->m_x = (old_point->m_x + p->m_x)/2 + dy/temp*m_largeur;
            p2->m_y = (old_point->m_y + p->m_y)/2 - dx/temp*m_largeur;
            
            GpsPoint_ptr p3(new GpsPoint());
            p3->m_x = (old_point->m_x + p->m_x)/2 - dy/temp*m_largeur;
            p3->m_y = (old_point->m_y + p->m_y)/2 + dx/temp*m_largeur;
            
            if(m_curves[1]->m_points.size() > 0){
                double d2 = m_curves[1]->m_points.back()->distanceCarre(*p2);
                double d3 = m_curves[1]->m_points.back()->distanceCarre(*p3);
                
                if(d2 < d3){
                    auto r = p3;
                    p3 = p2;
                    p2 = r;
                }
                
            }
            bool eliminate = false;
            for(auto p: m_listAB){
                double res = p2->distanceCarre(*p);
                if(res < m_largeur*m_largeur){
                    eliminate = true;
                }
            }
            if(!eliminate){
                m_curves[-1]->m_points.push_back(p2);
                
            }
            
            eliminate = false;
            for(auto p: m_listAB){
                double res = p3->distanceCarre(*p);
                if(res < m_largeur*m_largeur){
                    eliminate = true;
                }
            }
            if(!eliminate){
                m_curves[1]->m_points.push_back(p3);
            }
                       
            m_curves[0]->m_points.push_back(p);
        }
        old_point = p;
    }
    computeCurve(m_curves[0]);
    computeCurve(m_curves[1]);
    computeCurve(m_curves[-1]);
    
    for(int i = 2; i < 11; ++i){
        addLine(i);
        addLine(-i);
    }
    
    //saveABCurve();;
    
}

const double tol = 0.9999999;
size_t size_cur = 4;

double CurveAB::calculCurbature(Curve_ptr line, size_t i){
    if(line->m_points.size() < size_cur){
        m_curb_l = 0;
        return 0.0;
    }
    if(i < size_cur){
        m_curb_l = 0;
        return 0.0;
    }
    if(i > line->m_points.size()-size_cur-1){
        m_curb_l = 0;
        return 0.0;
    }
    auto pointB = line->m_points[i];
    auto pointA = line->m_points[i-size_cur];
    auto pointC = line->m_points[i+size_cur];
    
    double a = sqrt(pointA->distanceCarre(*pointB));
    double b = sqrt(pointB->distanceCarre(*pointC));
    double c = sqrt(pointC->distanceCarre(*pointA));
    
    double temp = (a + b)/c;
    //INFO("      " << temp << " " << a << " " << b << " " << c);
    if(temp > tol && temp < (1/tol)) {
        return 0;
    }
    double p = (a+b+c)/2;
    double curbature_p = (4*sqrt(p*(p-a)*(p-b)*(p-c)))/(a*b*c);
    
    double x_ab = pointB->m_x - pointA->m_x;
    double y_ab = pointB->m_y - pointA->m_y;
    double x_bc = pointC->m_x - pointB->m_x;;
    double y_bc = pointC->m_y - pointB->m_y;
    
    
    double det = x_ab*y_bc-y_ab*x_bc;
    
    double curbature = curbature_p;
    if(det < 0){
        curbature = -curbature;
    }
    curbature = -curbature;
    
    double x_ca = pointC->m_x - pointA->m_x;
    double y_ca = pointC->m_y - pointA->m_y;
    double d_ca = sqrt(x_ca*x_ca+y_ca*y_ca);
    double x_v = x_ca/d_ca;
    double y_v = y_ca/d_ca;
    
    if(curbature>0){
        m_curb_x = (pointC->m_x+ pointA->m_x)/2+y_v/curbature_p;
        m_curb_y = (pointC->m_y+ pointA->m_y)/2-x_v/curbature_p;
    } else {
        m_curb_x = (pointC->m_x+ pointA->m_x)/2-y_v/curbature_p;
        m_curb_y = (pointC->m_y+ pointA->m_y)/2+x_v/curbature_p;
    }
    m_curb_l = 1/curbature_p;
    m_curb_c_x = pointB->m_x;
    m_curb_c_y = pointB->m_y;
    
    return curbature;
}



void CurveAB::calculProjete2P(GpsPoint_ptr p, double deplacement_x, double deplacement_y){
    calculProjete2(p->m_x, p->m_y, deplacement_x, deplacement_y);
}



void CurveAB::calculProjete2(double x, double y, double deplacement_x, double deplacement_y){
    Curve_ptr list = getCurrentLine();
    if(list == NULL || list->m_points.size() < 5){
        return;
    }
    list->m_curve_i_min = 0;
    list->m_curve_i_min2 = 0;
    double dist_min = 10000;
    
    
    
    for(int i = 0; i < (int)list->m_points.size(); ++i){
        double d = list->m_points[i]->distanceCarre(x, y);
        if(d < dist_min){
            dist_min = d;
            list->m_curve_i_min = i;
        }
    }
    if(list->m_curve_i_min == 0){
        list->m_curve_i_min2 = 1;
    } else if(list->m_curve_i_min == ((int)list->m_points.size())-1){
        list->m_curve_i_min2 = list->m_points.size()-2;
    } else {
        
        double d1 = list->m_points[list->m_curve_i_min-1]->distanceCarre(x, y);
        double d2 = list->m_points[list->m_curve_i_min+1]->distanceCarre(x, y);
        
        if(d1 < d2){
            list->m_curve_i_min = list->m_curve_i_min-1;
        }
        list->m_curve_i_min2 = list->m_curve_i_min+1;
    }
    
    double x_a = x;
    double y_a = y;
    
    double x_b = list->m_points[list->m_curve_i_min]->m_x;
    double y_b = list->m_points[list->m_curve_i_min]->m_y;
    double x_m = list->m_points[list->m_curve_i_min2]->m_x;
    double y_m = list->m_points[list->m_curve_i_min2]->m_y;

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
    
    double x_ab = x_b-x_a;
    double y_ab = y_b-y_a;
    
     
    m_proj_prod_vect = deplacement_x*x_v+deplacement_y*y_v;
    
    double det = x_ab*deplacement_y-y_ab*deplacement_x;
    
    if(det < 0){
        m_proj_distance = -m_proj_distance;
    }
}

void CurveAB::calculProjete(GpsPoint_ptr p, double deplacement_x, double deplacement_y, bool change_line){
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




double CurveAB::anglefollowTheCarrot(double x, double y, double deplacement_x, double deplacement_y, double lookhead){
    m_fc_x = x;
    m_fc_y = y;
    
    Curve_ptr list = getCurrentLine();
    if(list == NULL || list->m_points.size() < 5){
        return 0;
    }
    calculProjete2(x, y, deplacement_x, deplacement_y);
    m_fc_xh = m_proj_x;
    m_fc_yh = m_proj_y;
    
    /**
     * Follow carrot PT LOOKHEAD
     */
    
    double d = m_proj_prod_vect;
    double x_v2 = 0;
    double y_v2 = 0;
    
    if(d>0){
        double d = lookhead;
        
        size_t i_min2 = list->m_curve_i_min2;
        while(d > 0 && i_min2 < list->m_points.size()-1){
            double dh = sqrt(list->m_points[i_min2]->distanceCarre(*list->m_points[i_min2+1]));
            d = d - dh;
            i_min2++;
        }
        
        auto pt1 = list->m_points[i_min2];

        x_v2 = pt1->m_x-m_fc_xh;
        y_v2 = pt1->m_y-m_fc_yh;
    } else {
        double d = lookhead;
        
        size_t i_min2 = list->m_curve_i_min2;
        while(d > 0 && i_min2 > 1){
            double dh = sqrt(list->m_points[i_min2]->distanceCarre(*list->m_points[i_min2-1]));
            d = d - dh;
            i_min2--;
        }
        
        auto pt1 = list->m_points[i_min2];

        x_v2 = pt1->m_x-m_fc_xh;
        y_v2 = pt1->m_y-m_fc_yh;
    }
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

double CurveAB::calculRearWheelPosition(double x_pont, double y_pont, double x, double y, double deplacement_x, double deplacement_y, double vitesse, double L, double KTH, double KE){
    double v = vitesse*10000.0/3600.0;
    
    Curve_ptr list = getCurrentLine();
    if(list == NULL || list->m_points.size() < 5){
        return 0;
    }
    calculProjete2(x_pont, y_pont, deplacement_x, deplacement_y);
    double e = -m_proj_distance;
    
    calculProjete2(x, y, deplacement_x, deplacement_y);
    double x_segment = m_proj_x_segment;
    double y_segment = m_proj_y_segment;

    double angle = my_angle(deplacement_x, deplacement_y, x_segment, y_segment);
    angle = angleBetweenPI2(angle);
    
    double k = calculCurbature(list, list->m_curve_i_min);
    double d = m_proj_prod_vect;
    if(d < 0){
        k = -k;
    }
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
