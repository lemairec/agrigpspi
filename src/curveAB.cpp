#include "curveAB.hpp"
#include <math.h>
#include "logging.hpp"

double CurveAB::distance(double x, double y, double lg){
    if(m_pointA.m_x!=0 && m_pointB.m_x!=0){
        double dist = (m_a * x + m_b * y + m_c)/m_sqrt_m_a_m_b;
        //INFO(dist);
        if(!m_sensAB){
            dist = -dist;
        }
        dist = fmod(dist, lg);
        if(dist > lg/2){
         dist -= lg;
        }
        if(dist < -lg/2){
         dist += lg;
        }
        if(dist < -lg/2){
         dist += lg;
        }
        if(dist > lg/2){
         dist -= lg;
        }
        //INFO("distance Point AB " << dist);
        return dist;
    }
    return 0.0;
    
}

void CurveAB::addPoint(GpsPoint_ptr p){
    m_listAB.push_front(p);
}


void clearLine(std::list<GpsPoint_ptr> & l){
    double longeur = 1.0;
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

double l = 10.0;

void CurveAB::addLine(int i){
    int j=i-1;
    int j2=i-2;
    if(i <0){
        j=i+1;
        j2=i+2;
    }
    
    
    INFO("ici " << i << " " << j);
    INFO("  ");
    INFO(m_list[j].size());
    
    GpsPoint_ptr old_point = nullptr;
    for(auto p : m_list[j]){
        if(old_point){
            
            double dx = p->m_x - old_point->m_x;
            double dy = p->m_y - old_point->m_y;
            
            double temp = sqrt(dx*dx + dy*dy);
            
            GpsPoint_ptr p2(new GpsPoint());
            p2->m_x = (old_point->m_x + p->m_x)/2 + dy/temp*l;
            p2->m_y = (old_point->m_y + p->m_y)/2 - dx/temp*l;
            
            GpsPoint_ptr p3(new GpsPoint());
            p3->m_x = (old_point->m_x + p->m_x)/2 - dy/temp*l;
            p3->m_y = (old_point->m_y + p->m_y)/2 + dx/temp*l;
            
            if(m_list[i].size() > 0){
                double d2 = m_list[i].back()->distance(*p2);
                double d3 = m_list[i].back()->distance(*p3);
                
                if(d2 < d3){
                    auto r = p3;
                    p3 = p2;
                    p2 = r;
                }
                
            } else {
                double d2 = m_list[j2].front()->distance(*p2);
                double d3 = m_list[j2].front()->distance(*p3);
                INFO(d2 << " " << d3);
                if(d2 > d3){
                    p3 = p2;
                }
            }
            bool eliminate = false;
            for(auto p: m_list[j]){
                double res = p3->distance(*p);
                if(res < l*l){
                    eliminate = true;
                }
            }
            if(!eliminate){
            m_list[i].push_back(p3);
            
            }
            
            
            
        }
        old_point = p;
    }
    clearLine(m_list[i]);
    
}

void CurveAB::savePointB(){
    
    clearLine(m_listAB);
    m_list.clear();
    GpsPoint_ptr old_point = nullptr;
    for(auto p : m_listAB){
        if(old_point){
            
            double dx = p->m_x - old_point->m_x;
            double dy = p->m_y - old_point->m_y;
            
            double temp = sqrt(dx*dx + dy*dy);
            
            GpsPoint_ptr p2(new GpsPoint());
            p2->m_x = (old_point->m_x + p->m_x)/2 + dy/temp*l;
            p2->m_y = (old_point->m_y + p->m_y)/2 - dx/temp*l;
            
            GpsPoint_ptr p3(new GpsPoint());
            p3->m_x = (old_point->m_x + p->m_x)/2 - dy/temp*l;
            p3->m_y = (old_point->m_y + p->m_y)/2 + dx/temp*l;
            
            if(m_list[1].size() > 0){
                double d2 = m_list[1].back()->distance(*p2);
                double d3 = m_list[1].back()->distance(*p3);
                
                if(d2 < d3){
                    auto r = p3;
                    p3 = p2;
                    p2 = r;
                }
                
            }
            bool eliminate = false;
            for(auto p: m_listAB){
                double res = p2->distance(*p);
                if(res < l*l){
                    eliminate = true;
                }
            }
            if(!eliminate){
                m_list[-1].push_back(p2);
                
            }
            
            eliminate = false;
            for(auto p: m_listAB){
                double res = p3->distance(*p);
                if(res < l*l){
                    eliminate = true;
                }
            }
            if(!eliminate){
                m_list[1].push_back(p3);
            }
            
            
                       
            m_list[0].push_back(p);
            
            
            
        }
        old_point = p;
    }
    clearLine(m_list[1]);
    clearLine(m_list[-1]);
    
    for(int i = 2; i < 10; ++i){
        addLine(i);
        addLine(-i);
    }
    
}
