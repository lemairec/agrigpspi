#include "curveAB.hpp"
#include <math.h>
#include "logging.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "environnement.hpp"


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
            if(res > 3*longeur*longeur){
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
    clearLine(m_list[i]->m_points);
    addPoints(m_list[i]->m_points);
    addPoints(m_list[i]->m_points);
    
}

void CurveAB::setCurrent(int i){
    verify(i);
    m_i_current = i;
}


void CurveAB::saveABCurve(){
    std::string file_job = ProjectSourceBin + "/curveAB.job";
    std::ofstream file;
    file.open(file_job, std::ios::out);
    for(auto p : m_list[0]->m_points){
        file << std::setprecision(11) << p->m_latitude << " " << p->m_longitude << std::endl;
    }
}

#include "gps_framework.hpp"

void CurveAB::loadABCurve(){
    std::string file_job = ProjectSourceBin + "/curveAB.job";
    std::ifstream file(file_job);
    std::string line;
    bool init = false;
    clearAll();
    
    
    GpsFramework & f = GpsFramework::Instance();
    
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        float a, b;
        if (!(iss >> a >> b)) {
            INFO("error");
            break;
            
        } // error
        INFO(a << " " << b);
        GpsPoint_ptr p = GpsPoint_ptr(new GpsPoint());
        p->m_latitude = a;
        p->m_longitude = b;
        
        if(!init){
            f.setRef(a, b);
            init = true;
        }
        f.m_gpsModule.setXY(*p);
        
        addPoint(p);
        // process pair (a,b)
    }
    savePointB();
    
}


void CurveAB::savePointB(){
    INFO(m_listAB.size());
    clearLine(m_listAB);
    INFO(m_listAB.size());
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
    clearLine(m_list[1]->m_points);
    clearLine(m_list[-1]->m_points);
    
    for(int i = 2; i < 11; ++i){
        addLine(i);
        addLine(-i);
    }
    
    //saveABCurve();;
    
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

void CurveAB::calculProjetePont(double x_pont, double y_pont, double deplacement_x, double deplacement_y, double lookhead){
    Lines_ptr list = getCurrentLine();
    if(list == NULL || list->m_points.size() < 5){
        return;
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
    
    x_h_lookhead = 0;
    y_h_lookhead = 0;
    
    double d = deplacement_x*x_v+deplacement_y*y_v;
    if(d > 0){
        x_h_lookhead = x_h_pont + x_v*lookhead;
        y_h_lookhead = y_h_pont + y_v*lookhead;
        
    } else {
        x_h_lookhead = x_h_pont - x_v*lookhead;
        y_h_lookhead = y_h_pont - y_v*lookhead;
        
    }
    
    double x_segment = x_h_lookhead - x_pont;
    double y_segment = y_h_lookhead - y_pont;
    
    double cos_a = (deplacement_x*x_segment+deplacement_y*y_segment)/(sqrt(x_segment*x_segment+y_segment*y_segment)*sqrt(deplacement_x*deplacement_x+deplacement_y*deplacement_y));
    
    m_angle = acos(cos_a);
    
    double ah = sqrt((x_h-x_a)*(x_h-x_a) + (y_h-y_a)*(y_h-y_a));
    m_distance_pont = ah;
    
    double x_ab = x_b-x_a;
    double y_ab = y_b-y_a;
    
    double det = x_ab*deplacement_y-y_ab*deplacement_x;
    
    if(det < 0){
        m_distance_pont = -m_distance_pont;
        m_angle = -m_angle;
    }
    
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


void Parcelle::addPoint(GpsPoint_ptr p){
    //m_contour.push_back(p);
}
