#include "curveAB.hpp"
#include <math.h>
#include "logging.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "environnement.hpp"
#include "util/util.hpp"

void CurveAB::clearAll(){
    m_list.clear();
    m_listAB.clear();
}

void CurveAB::clearWithoutAB(){
    m_list.clear();
    savePointB();
}

double CurveAB::distance(double x, double y, double lg){
    return 0.0;
    
}

Lines_ptr CurveAB::getCurrentLine(){
    return m_list[m_i_current];
}

Lines_ptr CurveAB::getCurrentLineRel(int i){
    int res = m_i_current+i;
    verify(res);
    return m_list[res];
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

double longeur = 0.5;


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

void verifyLine(std::vector<GpsPoint_ptr> & l){
    clearLine(l);
    addPoints(l);
    addPoints(l);
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
    m_list[i] = Lines_ptr(new Lines(i));
    if(i<m_i_min){
        m_i_min = i;
    }
    if(i>m_i_max){
        m_i_max = i;
    }
    
    GpsPoint_ptr old_point = nullptr;
    for(auto p : m_list[j]->m_points){
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
            
            if(m_list[i]->m_points.size() > 0){
                double d2 = m_list[i]->m_points.back()->distanceCarre(*p2);
                double d3 = m_list[i]->m_points.back()->distanceCarre(*p3);
                
                if(d2 < d3){
                    auto r = p3;
                    p3 = p2;
                    p2 = r;
                }
                
            } else {
                double d2 = m_list[j2]->m_points.front()->distanceCarre(*p2);
                double d3 = m_list[j2]->m_points.front()->distanceCarre(*p3);
                //INFO(d2 << " " << d3);
                if(d2 > d3){
                    p3 = p2;
                }
            }
            m_list[i]->m_points.push_back(p3);
            
            
        }
        old_point = p;
    }
    verifyLine(m_list[i]->m_points);
}

void CurveAB::setCurrent(int i){
    verify(i);
    m_i_current = i;
}

void CurveAB::savePointB(){
    clearLine(m_listAB);
    m_list.clear();
    GpsPoint_ptr old_point = nullptr;
    
    m_list[0] = Lines_ptr(new Lines(0));
    m_list[1] = Lines_ptr(new Lines(1));
    m_list[-1] = Lines_ptr(new Lines(-1));
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
            
            if(m_list[1]->m_points.size() > 0){
                double d2 = m_list[1]->m_points.back()->distanceCarre(*p2);
                double d3 = m_list[1]->m_points.back()->distanceCarre(*p3);
                
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
                m_list[-1]->m_points.push_back(p2);
                
            }
            
            eliminate = false;
            for(auto p: m_listAB){
                double res = p3->distanceCarre(*p);
                if(res < m_largeur*m_largeur){
                    eliminate = true;
                }
            }
            if(!eliminate){
                m_list[1]->m_points.push_back(p3);
            }
                       
            m_list[0]->m_points.push_back(p);
        }
        old_point = p;
    }
    verifyLine(m_list[0]->m_points);
    verifyLine(m_list[1]->m_points);
    verifyLine(m_list[-1]->m_points);
    
    for(int i = 2; i < 11; ++i){
        addLine(i);
        addLine(-i);
    }
    
    //saveABCurve();;
    
}

const double tol = 0.9999999;
size_t size_cur = 10;

double CurveAB::calculCurbature(Lines_ptr line, size_t i){
    if(line->m_points.size() < size_cur){
        return 0.0;
    }
    if(i < size_cur){
        return 0.0;
    }
    if(i > line->m_points.size()-size_cur-1){
        return 0.0;
    }
    auto pointA = line->m_points[i-size_cur];
    auto pointB = line->m_points[i];
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
    double curbature = (4*sqrt(p*(p-a)*(p-b)*(p-c)))/(a*b*c);
    
    double x_ab = pointB->m_x - pointA->m_x;
    double y_ab = pointB->m_y - pointA->m_y;
    double x_bc = pointC->m_x - pointB->m_x;;
    double y_bc = pointC->m_y - pointB->m_y;
    
    
    double det = x_ab*y_bc-y_ab*x_bc;
    
    if(det < 0){
        curbature = -curbature;
    }
    
    return curbature;
}

void CurveAB::calculProjete2(GpsPoint_ptr p, double deplacement_x, double deplacement_y){
    Lines_ptr list = getCurrentLine();
    if(list == NULL || list->m_points.size() < 5){
        return;
    }
    list->m_curve_i_min = 0;
    list->m_curve_i_min2 = 0;
    double dist_min = 10000;
    
    
    
    for(int i = 0; i < (int)list->m_points.size(); ++i){
        double d = list->m_points[i]->distanceCarre(*p);
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
        
        double d1 = list->m_points[list->m_curve_i_min-1]->distanceCarre(*p);
        double d2 = list->m_points[list->m_curve_i_min+1]->distanceCarre(*p);
        
        if(d1 < d2){
            list->m_curve_i_min = list->m_curve_i_min-1;
        }
        list->m_curve_i_min2 = list->m_curve_i_min+1;
    }
    
    double x_a = p->m_x;
    double y_a = p->m_y;
    
    double x_b = list->m_points[list->m_curve_i_min]->m_x;
    double y_b = list->m_points[list->m_curve_i_min]->m_y;
    double x_m = list->m_points[list->m_curve_i_min2]->m_x;
    double y_m = list->m_points[list->m_curve_i_min2]->m_y;

    //https://fr.wikipedia.org/wiki/Projection_orthogonale
    double x_v = x_m-x_b;
    double y_v = y_m-y_b;
    double d_v = sqrt(x_v*x_v + y_v*y_v);
    x_v = x_v/d_v;
    y_v = y_v/d_v;
    
    
    
    double bh = (x_a-x_b)*x_v+(y_a-y_b)*y_v;
    x_h = x_b + bh*x_v;
    y_h = y_b + bh*y_v;
    
    
    double ah = sqrt((x_h-x_a)*(x_h-x_a) + (y_h-y_a)*(y_h-y_a));
    m_distance = ah;
    
    double x_ab = x_b-x_a;
    double y_ab = y_b-y_a;
    
    double det = x_ab*deplacement_y-y_ab*deplacement_x;
    
    if(det < 0){
        m_distance = -m_distance;
    }
}

double CurveAB::followCarrot(double x_pont, double y_pont, double deplacement_x, double deplacement_y, double lookhead){
    Lines_ptr list = getCurrentLine();
    if(list == NULL || list->m_points.size() < 5){
        return 0;
    }
    list->m_curve_i_min = 0;
    list->m_curve_i_min2 = 0;
    double dist_min = 10000;
    
    
    
    for(int i = 0; i < (int)list->m_points.size(); ++i){
        double d = list->m_points[i]->distanceCarre(x_pont, y_pont);
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
        
        double d1 = list->m_points[list->m_curve_i_min-1]->distanceCarre(x_pont, y_pont);
        double d2 = list->m_points[list->m_curve_i_min+1]->distanceCarre(x_pont, y_pont);
        
        if(d1 < d2){
            list->m_curve_i_min = list->m_curve_i_min-1;
        }
        list->m_curve_i_min2 = list->m_curve_i_min+1;
    }
    
    //double curvature = calculCurbature(list, list->m_curve_i_min2);
    //INFO(curvature);
    
    double x_a = x_pont;
    double y_a = y_pont;
    
    double x_b = list->m_points[list->m_curve_i_min]->m_x;
    double y_b = list->m_points[list->m_curve_i_min]->m_y;
    double x_m = list->m_points[list->m_curve_i_min2]->m_x;
    double y_m = list->m_points[list->m_curve_i_min2]->m_y;

    //https://fr.wikipedia.org/wiki/Projection_orthogonale
    double x_v = x_m-x_b;
    double y_v = y_m-y_b;
    double d_v = sqrt(x_v*x_v + y_v*y_v);
    x_v = x_v/d_v;
    y_v = y_v/d_v;
    
    
    
    double bh = (x_a-x_b)*x_v+(y_a-y_b)*y_v;
    x_h_pont = x_b + bh*x_v;
    y_h_pont = y_b + bh*y_v;
    
    /**
     * Follow carrot PT LOOKHEAD
     */
    
    x_h_lookhead = 0;
    y_h_lookhead = 0;
    
    double d = deplacement_x*x_v+deplacement_y*y_v;
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

        x_v2 = pt1->m_x-x_h_pont;
        y_v2 = pt1->m_y-y_h_pont;
    } else {
        double d = lookhead;
        
        size_t i_min2 = list->m_curve_i_min2;
        while(d > 0 && i_min2 > 1){
            double dh = sqrt(list->m_points[i_min2]->distanceCarre(*list->m_points[i_min2-1]));
            d = d - dh;
            i_min2--;
        }
        
        auto pt1 = list->m_points[i_min2];

        x_v2 = pt1->m_x-x_h_pont;
        y_v2 = pt1->m_y-y_h_pont;
    }
    double d_v2 = sqrt(x_v2*x_v2 + y_v2*y_v2);
    x_v2 = x_v2/d_v2;
    y_v2 = y_v2/d_v2;
    double d2 = deplacement_x*x_v2+deplacement_y*y_v2;
    if(d2 >0){
        x_h_lookhead = x_h_pont + lookhead*x_v2;
        y_h_lookhead = y_h_pont + lookhead*y_v2;
    } else {
        x_h_lookhead = x_h_pont - lookhead*x_v2;
        y_h_lookhead = y_h_pont - lookhead*y_v2;
    }
    
    /**
     * Follow carrot CALCUL ANGLE
     */

    double x_segment = x_h_lookhead - x_pont;
    double y_segment = y_h_lookhead - y_pont;
    
    double m_angle = -my_angle(deplacement_x, deplacement_y, x_segment, y_segment);
    m_angle = angleBetweenPI2(m_angle);
    
    double ha_x = x_h_pont-x_a;
    double ha_y = y_h_pont-y_a;
    
    double ah = sqrt((ha_x)*(ha_x) + (ha_y)*(ha_y));
    m_distance_pont = ah;
    
    double det = x_segment*ha_y-y_segment*ha_x;
    
    if(det < 0){
        m_distance_pont = -m_distance_pont;
    }
    
    return m_angle;
    
}

void CurveAB::calculProjete(GpsPoint_ptr p, double deplacement_x, double deplacement_y){
    calculProjete2(p, deplacement_x, deplacement_y);
    double dist = abs(m_distance);
    if(dist > m_largeur/2){
        double temp_x_h = x_h;
        double temp_y_h = y_h;
        double temp_i = m_i_current;
        double temp_distance = m_distance;
        
        setCurrent(m_i_current + 1);
        calculProjete2(p, deplacement_x, deplacement_y);
        double dist2 = abs(m_distance);
        if(dist2 < dist){
            return;
        }
        setCurrent(m_i_current - 2);
        calculProjete2(p, deplacement_x, deplacement_y);
        double dist3 = abs(m_distance);
        if(dist3 < dist){
            return;
        }
        setCurrent(temp_i);
        x_h = temp_x_h;
        y_h = temp_y_h;
        m_distance = temp_distance;
    }
}


double CurveAB::calculRearWheelPosition(double p_x, double p_y, double deplacement_x, double deplacement_y, double vitesse, double L, double KTH, double KE){
    
    Lines_ptr list = getCurrentLine();
    if(list == NULL || list->m_points.size() < 5){
        return 0.0;
    }
    list->m_curve_i_min = 0;
    list->m_curve_i_min2 = 0;
    double dist_min = 10000;
    
    
    
    for(int i = 0; i < (int)list->m_points.size(); ++i){
        double d = list->m_points[i]->distanceCarre(p_x, p_y);
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
        
        double d1 = list->m_points[list->m_curve_i_min-1]->distanceCarre(p_x, p_y);
        double d2 = list->m_points[list->m_curve_i_min+1]->distanceCarre(p_x, p_y);
        
        if(d1 < d2){
            list->m_curve_i_min = list->m_curve_i_min-1;
        }
        list->m_curve_i_min2 = list->m_curve_i_min+1;
    }
    
    double x_a = p_x;
    double y_a = p_y;
    
    double x_b = list->m_points[list->m_curve_i_min]->m_x;
    double y_b = list->m_points[list->m_curve_i_min]->m_y;
    double x_m = list->m_points[list->m_curve_i_min2]->m_x;
    double y_m = list->m_points[list->m_curve_i_min2]->m_y;

    //https://fr.wikipedia.org/wiki/Projection_orthogonale
    double x_v = x_m-x_b;
    double y_v = y_m-y_b;
    double d_v = sqrt(x_v*x_v + y_v*y_v);
    x_v = x_v/d_v;
    y_v = y_v/d_v;
    
    
    
    double bh = (x_a-x_b)*x_v+(y_a-y_b)*y_v;
    x_h = x_b + bh*x_v;
    y_h = y_b + bh*y_v;
    
    
    double ha_x = x_h-x_a;
    double ha_y = y_h-y_a;
    
    double ah = sqrt((ha_x)*(ha_x) + (ha_y)*(ha_y));
    m_distance = ah;
    
    
    double x_segment = x_m - x_b;
    double y_segment = y_m - y_b;
    
    
    double angle = -my_angle(deplacement_x, deplacement_y, x_segment, y_segment);
    angle = angleBetweenPI2(angle);
    

    double det = x_segment*ha_y-y_segment*ha_x;
    
    if(det > 0){
        ah = -ah;
    }

    double e = -ah;
    double v = vitesse*10000.0/3600.0;
    double k = -calculCurbature(list, list->m_curve_i_min);
    double th_e = -angle;//todo;

    double xk = v * k * cos(th_e) / (1.0 - k * e) ;
    double xthe = KTH * abs(v) * th_e ;
    double xe = KE * v * sin(th_e) * e / th_e;
    
    
    
    //std::cout << "c  e " << e << "  th_e " << th_e << std::endl;
    //std::cout << "c    " << x2 << " " << x3 << std::endl;
    
    double omega = xk - xthe - xe;

    m_d_e = e;
    m_d_angle = th_e;
    m_d_k = k;
    
    m_d_xk = xk;
    m_d_xthe = -xthe;
    m_d_xe = -xe;
    m_d_res = omega;
    
    double delta = atan2(L * omega / v, 1.0);

    m_d_delta = delta;
    
   
    //std::cout << e <<  " " << cos_a << " th_e "  << th_e << " " << v << " " << th_e << " " << k << " " << omega << " d " << delta << std::endl;
    //std::cout << " " << delta << " " << omega << " ++ " << x1 << " "  << x2 << " " << x3 << std::endl;
    //INFO(omega);
    return delta;
}
