#include "gps_framework.hpp"
#include "logging.hpp"
#include "config/config.hpp"
#include "environnement.hpp"

#include <QDateTime>
#include <math.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

std::string file_info;
std::ofstream file_job_stream;
std::ofstream file_debug_stream;
std::string date_str;

GpsFramework::GpsFramework(){
    QDateTime date = QDateTime::currentDateTime();
    QString s = date.toString("yyyyMMddThhmm");
    date_str = s.toUtf8().constData();
    
    int y = date.date().year();
    if(y>(1011+1011)){
    //    exit(0);
    }
    
    std::string dir = ProjectSourceBin + "/job";
    std::string s2 = "mkdir -p "+ dir + ";";
    if(system( s2.c_str() )){
        WARN("can not execute : " << s2);
    };
    
    file_info = ProjectSourceBin + "/job/gps_" + s.toUtf8().constData() + ".info";
    
    std::string file_job = ProjectSourceBin + "/job/gps_" + s.toUtf8().constData() + ".job";
    file_job_stream.open(file_job, std::ios::out);
    
    
    std::string file_debug = ProjectSourceBin + "/job/gps_" + s.toUtf8().constData() + ".debug";
    file_debug_stream.open(file_debug, std::ios::out);
    //mode_t mt;
    
    INFO(file_info);
    INFO(file_job);
    INFO(file_debug);
    INFO(file_job_stream.is_open());
    INFO(file_debug_stream.is_open());
    
    m_config.load();
}

void GpsFramework::saveInfoFile(){
    std::ofstream file;
    file.open(file_info, std::ios::out);
    
    QDateTime date_end = QDateTime::currentDateTime();
    QString s = date_end.toString("yyyyMMddThhmm");
    std::string date_end_str = s.toUtf8().constData();
    
    
    file << "{" << std::endl;
    file << "\"date_begin\":\"" << date_str << "\"" << std::endl;
    file << ",\"date_end\":\"" << date_end_str << "\"" << std::endl;
    file << ",\"surface\":" << m_surface << std::endl;
    file << "}" << std::endl;
}

void GpsFramework::addError(std::string s){
    WARN(s);
    std::ostringstream oss;
    oss << m_messages_errors << "\n" << s;
    m_messages_errors = oss.str();
}

void GpsFramework::initOrLoadConfig(){
    m_config.save();
    
    m_pilotModule.initOrLoadConfig(m_config);
    m_serialModule.initOrLoad(m_config);
    m_fileModule.initOrLoad(m_config);
    
    m_distance_cap_vitesse = 3;
    m_lineAB.m_pointA.m_latitude = m_config.m_a_lat;
    m_lineAB.m_pointA.m_longitude = m_config.m_a_lon;
    m_lineAB.m_pointB.m_latitude = m_config.m_b_lat;
    m_lineAB.m_pointB.m_longitude = m_config.m_b_lon;
    file_job_stream << "[config]\n";
    setAB();
    m_reloadConfig = true;
    m_debug_log = m_config.m_debug_log;
    
    m_pilot_lookahead_d = m_config.m_pilot_lookahead_d;
    
    m_tracteur.m_antenne_essieu_avant = m_config.m_tracteur_empatement - m_config.m_tracteur_antenne_pont_arriere;
    m_tracteur.m_antenne_essieu_arriere = m_config.m_tracteur_antenne_pont_arriere;
    
    m_curveAB.m_largeur = m_config.m_outil_largeur;
    m_gga = m_config.m_gga;
}

GpsFramework & GpsFramework::Instance(){
    static GpsFramework gf;
    return gf;
}

void GpsFramework::setRef(double latitude, double longitude){
    m_gpsModule.setRef(latitude, longitude);
    for(auto l: m_list){
        m_gpsModule.setXY(*l);
    }
    for(auto l: m_listSurfaceToDraw){
        m_gpsModule.setXY(*l->m_lastPoint);
        //m_gpsModule.setXY(*l->m_lastPointOk);
        for(auto l2: l->m_points){
           m_gpsModule.setXY(*l2);
       }
    }
    m_gpsModule.setXY(m_lineAB.m_pointA);
    m_gpsModule.setXY(m_lineAB.m_pointB);
}



std::list<double> distances;
double moyDistance(double distance){
    distances.push_front(distance);
    while(distances.size()>10){
        distances.pop_back();
    }
    double sum = 0;
    for(auto d : distances){
        sum += d;
    }
    return sum/distances.size();
}


std::list<double> deplacements;
double moyDeplacement(double deplacement){
    deplacements.push_front(deplacement);
    while(deplacements.size()>10){
        deplacements.pop_back();
    }
    double sum = 0;
    for(auto d : deplacements){
        sum += d;
    }
    return sum/deplacements.size();
}

void GpsFramework::onGGAFrame(GGAFrame & f){
    m_lastGGAFrame = GGAFrame(f);
    if(m_gga){
        if(f.isOk()){
            GGAFrame_ptr frame = GGAFrame_ptr(new GGAFrame(f));
            onNewPoint(frame);
        }
    }
}

void GpsFramework::onRMCFrame(RMCFrame_ptr f){
    m_lastRMCFrame = f;
    if(!m_gga){
        onNewPoint(f);
            
        m_vitesse = f->m_vitesse_kmh;
    }
}

void GpsFramework::onNewPoint(GpsPoint_ptr p){
    if(m_gpsModule.m_latitudeRef == 0){
        setRef(p->m_latitude, p->m_longitude);
        return;
    }
    
    if(m_lastImportantPoint && m_lastImportantPoint->distanceCarre(*p) > 100*100){
        INFO("point");
        return;
    }
    
    m_list.push_front(p);

    if(m_list.size()>100){
        m_list.pop_back();
    };

    calculDeplacement();

    calculDistance(p);
    
    calculAngleCorrection();
    m_pilotModule.run(m_angle_correction, m_time_last_point, m_vitesse);

    setNewGpsTime();
    
    m_lastPoint = p;
    if(m_listSurfaceToDraw.size()>0 && !m_pauseDraw){
        m_listSurfaceToDraw.front()->m_lastPoint = p;
    }
    if(m_lastImportantPoint && m_lastImportantPoint->distanceCarre(*p) < m_distance_cap_vitesse*m_distance_cap_vitesse){
        return;
    }
    
    
    onNewImportantPoint(p);
}

void GpsFramework::onNewImportantPoint(GpsPoint_ptr p){
    
    if(m_etat == Etat_PointASaved && !m_line){
        m_curveAB.addPoint(p);
    }
    m_lastImportantPoint = p;
    calculSurface();
    DEBUG("draw");
    calculDraw(p);
    
    file_job_stream << p->m_time << "," << std::setprecision(11) << p->m_latitude << "," << p->m_longitude << std::endl;
    saveInfoFile();
}


void GpsFramework::setNewGpsTime(){
    auto begin = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = begin - m_last_gps_received;

    double seconds = diff.count()*1000;
    m_last_gps_received = begin;
    
    m_gps_times.push_front(seconds);
    while(m_gps_times.size()>10){
        m_gps_times.pop_back();
    }
    
    double sum = 0;
    for(auto c : m_gps_times){
        sum += c;
    }
    double moy = sum/m_gps_times.size();
    
    
    double sum2 = 0;
    for(auto c : m_gps_times){
        sum2 += (moy-c)*(moy-c);
    }
    double error = std::sqrt(sum2/m_gps_times.size());
    
    m_gps_time_moy = moy;
    m_gps_time_et = error;
}

bool GpsFramework::isGpsConnected(){
    auto begin = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = begin - m_last_gps_received;

    if(diff.count() < 2.0){
        return true;
    } else {
        return false;
    }
}

void GpsFramework::setNewPilotTime(){
    auto begin = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = begin - m_last_pilot_received;

    double seconds = diff.count()*1000;
    m_last_pilot_received = begin;
    
    m_pilot_times.push_front(seconds);
    while(m_pilot_times.size()>10){
        m_pilot_times.pop_back();
    }
    
    double sum = 0;
    for(auto c : m_pilot_times){
        sum += c;
    }
    double moy = sum/m_pilot_times.size();
    
    
    double sum2 = 0;
    for(auto c : m_pilot_times){
        sum2 += (moy-c)*(moy-c);
    }
    double error = std::sqrt(sum2/m_pilot_times.size());
    
    m_pilot_time_moy = moy;
    m_pilot_time_et = error;
}

bool GpsFramework::isPilotConnected(){
    auto begin = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = begin - m_last_pilot_received;

    if(diff.count() < 2.0){
        return true;
    } else {
        return false;
    }
}





void GpsFramework::onFrame(const std::string &frame){
    if(m_debug_log){
        file_debug_stream << frame << "\n";
    }
}


void GpsFramework::calculDistance(GpsPoint_ptr p){
    
    if(m_etat == Etat_OK){
        if(m_line == false){
            m_curveAB.calculProjete(p, m_deplacementX, m_deplacementY);
            m_distanceAB = m_curveAB.m_distance;
        } else {
            m_distanceAB = m_lineAB.distance(p->m_x, p->m_y, m_config.m_outil_largeur);
        }
    } else {
        m_distanceAB = 0;
    }
    
    double coeff = m_config.m_outil_largeur/(2*6);
    if(m_distanceAB < 0.0){
        m_ledAB = round(-m_distanceAB/coeff) + 1;
    } else {
        m_ledAB = -round(m_distanceAB/coeff) - 1;
    }
}

void GpsFramework::savePointA(){
    if(!m_list.empty()){
        m_lineAB.m_pointA = *(*m_list.begin());
    }
    setRef(m_lineAB.m_pointA.m_latitude, m_lineAB.m_pointA.m_longitude);
    
    file_job_stream << "[savePointA]\n";
    INFO(m_lineAB.m_pointA.m_time << " " << m_lineAB.m_pointA.m_latitude << " " << m_lineAB.m_pointA.m_longitude);
    clearSurface();
    
    m_etat = Etat_PointASaved;
}

void GpsFramework::savePointB(){
    if(!m_list.empty()){
        m_lineAB.m_pointB = *(*m_list.begin());
    }
    
    INFO(m_lineAB.m_pointB.m_time << " " << m_lineAB.m_pointB.m_latitude << " " << m_lineAB.m_pointB.m_longitude);
    if(m_lineAB.m_pointA.m_isOk!=0 && m_lineAB.m_pointB.m_isOk!=0){
        setAB();
    }
    file_job_stream << "[savePointB]\n";
    
    m_etat = Etat_OK;
    m_curveAB.savePointB();
}

void GpsFramework::setEtat(Etat etat){
    m_etat = etat;
    if(etat == Etat_Reset){
        m_surface = 0;
        m_lastPoint = nullptr;
        m_listSurfaceToDraw.clear();
        m_ledAB = 0;
        m_list.clear();
        m_contour.clear();
        m_curveAB.m_list.clear();
        m_curveAB.m_listAB.clear();
    }
}

void GpsFramework::setAB(){
    m_config.m_a_lat = m_lineAB.m_pointA.m_latitude;
    m_config.m_a_lon = m_lineAB.m_pointA.m_longitude;
    m_config.m_b_lat = m_lineAB.m_pointB.m_latitude;
    m_config.m_b_lon = m_lineAB.m_pointB.m_longitude;
    m_config.save();
    
    //m_pointA.m_x = 1; m_pointA.m_y = 1;
    setRef((m_lineAB.m_pointA.m_latitude + m_lineAB.m_pointB.m_latitude)/2, (m_lineAB.m_pointA.m_longitude + m_lineAB.m_pointB.m_longitude)/2);
    m_lineAB.m_ab_x = m_lineAB.m_pointB.m_x - m_lineAB.m_pointA.m_x;
    m_lineAB.m_ab_y = m_lineAB.m_pointB.m_y - m_lineAB.m_pointA.m_y;
    
    
    m_lineAB.m_a = -(m_lineAB.m_pointB.m_y - m_lineAB.m_pointA.m_y);
    m_lineAB.m_b = m_lineAB.m_pointB.m_x - m_lineAB.m_pointA.m_x;
    m_lineAB.m_c = -m_lineAB.m_a * m_lineAB.m_pointA.m_x - m_lineAB.m_b *  m_lineAB.m_pointA.m_y;
    m_lineAB.m_sqrt_m_a_m_b = sqrt(m_lineAB.m_a*m_lineAB.m_a + m_lineAB.m_b*m_lineAB.m_b);
    
    if(m_lineAB.m_ab_y != 0 && m_lineAB.m_ab_x != 0){
        m_lineAB.m_angleAB = atan(m_lineAB.m_ab_x/m_lineAB.m_ab_y);
    } else {
        m_lineAB.m_angleAB = 0;
    }
    INFO("yb  " << std::fixed << m_lineAB.m_pointB.m_y << " ya " << m_lineAB.m_pointA.m_y << " xb " << m_lineAB.m_pointB.m_x << " xa " << m_lineAB.m_pointA.m_x);
    INFO(m_lineAB.m_a << "*x + " << m_lineAB.m_b << "*y + " << m_lineAB.m_c << " = 0; " << m_lineAB.m_sqrt_m_a_m_b);
}

/*void GpsFramework::calculDeplacement(){
    if(m_list.size() > 3){
        GpsPoint_ptr point1 = m_list.front();
        GpsPoint_ptr point2 = NULL;
        GpsPoint_ptr point3 = NULL;
        int i = 0;
        for(auto point : m_list){
            if(point3 == NULL){
                point2 = point;
                
                double x = point1->m_x - point2->m_x;
                double y = point1->m_y - point2->m_y;
                
                double d = x*x+y*y;
                if(d>(m_distance_cap_vitesse*m_distance_cap_vitesse)){
                    point3 = point;
                }
            } else {
                point3 = point;
                
                double x = point2->m_x - point3->m_x;
                double y = point2->m_y - point3->m_y;
                
                double d = x*x+y*y;
                if(d>(m_distance_cap_vitesse*m_distance_cap_vitesse)){
                    break;
                }
            }
            //point2 = NULL;
            ++i;
        }
        if(point2!=NULL){
            double x = point1->m_x - point2->m_x;
            double y = point1->m_y - point2->m_y;
            
            m_deplacementX = x;
            m_deplacementY = y;
            
            if(m_deplacementY != 0){
                double temp = atan(m_deplacementX/m_deplacementY);
                if(m_deplacementY>0){
                    m_deplacementAngle = temp;
                } else {
                    m_deplacementAngle = temp+3.14;
                }
                //int perc = temp/m_deplacementAngle*100;
                //INFO(perc << " " << temp/3.14*180 << " " << m_deplacementAngle/3.14*180);
            }
            
            if(m_ab_x != 0 || m_ab_y != 0){
                double det = m_a*m_deplacementY - m_b*m_deplacementX;
                //m_deplacementAngle = m_deplacementAngle+3.14;
                m_sensAB = (det < 0);
            }
            
            
            m_distance_last_point = sqrt(m_deplacementX*m_deplacementX + m_deplacementY*m_deplacementY);
            m_time_last_point = point1->m_timeHour - point2->m_timeHour;
            if(m_time_last_point > 0){
                m_vitesse = m_distance_last_point/1000.0/m_time_last_point;
                if(m_vitesse >50){
                    INFO("erreur");
                }
            }
        }
        
        
        int i2 = 0;
        GpsPoint_ptr poin1 = point1 ,poin2 = point2,poin3 = point3;
        
        if(point3){
        
            double num = (poin1->m_x - poin3->m_x)*(poin2->m_x - poin3->m_x) + (poin1->m_y - poin3->m_y)*(poin2->m_y - poin3->m_y);
            
            double den1 = std::sqrt((poin1->m_x - poin3->m_x)*(poin1->m_x - poin3->m_x)+(poin1->m_y - poin3->m_y)*(poin1->m_y - poin3->m_y));
            double den2 = std::sqrt((poin2->m_x - poin3->m_x)*(poin2->m_x - poin3->m_x)+(poin2->m_y - poin3->m_y)*(poin2->m_y - poin3->m_y));
            
            double cosa = num/(den1*den2);
            double a = acos(cosa);
            double a2 = a/3.14*180;
            
            //INFO(a2);
        }
        //INFO(deplacementTime << " " << vitesse);
    }
    //calculContourExterieur();
}*/

void GpsFramework::calculDeplacement(){
    
    
    if(m_list.size() > 3){
        GpsPoint_ptr point1 = m_list.front();
              
        
        
        GpsPoint_ptr point2 = NULL;
        int i = 0;
        for(auto point : m_list){
            point2 = point;
            
            double x = point1->m_x - point2->m_x;
            double y = point1->m_y - point2->m_y;
            
            double d = x*x+y*y;
            if(d>(m_distance_cap_vitesse*m_distance_cap_vitesse)){
                break;
            }
            
            //point2 = NULL;
            ++i;
        }
        if(point2!=NULL){
            double x = point1->m_x - point2->m_x;
            double y = point1->m_y - point2->m_y;
            
            m_deplacementX = x;
            m_deplacementY = y;
            
            if(m_deplacementY != 0){
                double temp = atan(m_deplacementX/m_deplacementY);
                if(m_deplacementY>0){
                    m_deplacementAngle = temp;
                } else {
                    m_deplacementAngle = temp+3.14;
                }
                //int perc = temp/m_deplacementAngle*100;
                //INFO(perc << " " << temp/3.14*180 << " " << m_deplacementAngle/3.14*180);
            }
            
            if(m_lineAB.m_ab_x != 0 || m_lineAB.m_ab_y != 0){
                double det = m_lineAB.m_a*m_deplacementY - m_lineAB.m_b*m_deplacementX;
                //m_deplacementAngle = m_deplacementAngle+3.14;
                m_lineAB.m_sensAB = (det < 0);
            }
            
            
            m_distance_last_point = sqrt(m_deplacementX*m_deplacementX + m_deplacementY*m_deplacementY);
            m_time_last_point = point1->m_timeHour - point2->m_timeHour;
            
            m_tracteur.m_x_antenne = point1->m_x;
            m_tracteur.m_y_antenne = point1->m_y;
            m_tracteur.m_x_essieu_avant = point1->m_x + sin(m_deplacementAngle)*m_tracteur.m_antenne_essieu_avant;
            m_tracteur.m_y_essieu_avant = point1->m_y + cos(m_deplacementAngle)*m_tracteur.m_antenne_essieu_avant;
            
            if(m_time_last_point > 0){
                m_vitesse = m_distance_last_point/1000.0/m_time_last_point;
                if(m_vitesse >50){
                    INFO("erreur");
                }
            }
            
        }
        
            
        
        
        //INFO(deplacementTime << " " << vitesse);
    }
    //calculContourExterieur();
}

void GpsFramework::changeDraw(){
    if(m_pauseDraw == false){
        m_pauseDraw = true;
    } else {
        SurfaceToDraw_ptr p(new SurfaceToDraw());
        m_listSurfaceToDraw.push_front(p);
        m_pauseDraw = false;
        onNewImportantPoint(m_lastPoint);
    }
}

void GpsFramework::calculDraw(GpsPoint_ptr p){
    if(m_listSurfaceToDraw.size()==0){
        SurfaceToDraw_ptr s(new SurfaceToDraw());
        m_listSurfaceToDraw.push_front(s);
        s->m_points.push_front(p);
        return;
    }
    if(m_pauseDraw){
        return;
    }
    
    auto s = m_listSurfaceToDraw.front();
    s->m_points.push_front(p);
}

//surface
void GpsFramework::clearSurface(){
    m_surface = 0;
}

void GpsFramework::calculSurface(){
    m_surface = 0;
    m_surface_h = 0;
    //INFO("****");
    for(auto s : m_listSurfaceToDraw){
        if(s->m_points.size()>0){
            auto last_frame = s->m_lastPoint;
            double x1 = last_frame->m_x;
            double y1 = last_frame->m_y;
            for(auto it = s->m_points.begin(); it != s->m_points.end(); ++it){
                auto frame = (*it);
                double x0 = frame->m_x;
                double y0 = frame->m_y;
                double dist = (x0-x1)*(x0-x1) + (y0-y1)*(y0-y1);
                
                double surface = std::sqrt(dist)*m_config.m_outil_largeur/10000.0;
                m_surface += surface;
                /*if(m_surface_h == 0){
                    if(last_frame->m_timeHour!=frame->m_timeHour){
                        m_surface_h = m_surface/(last_frame->m_timeHour-frame->m_timeHour);
                                
                    }
                }*/
                
                x1 = x0;
                y1 = y0;
            }
        }
    }
        
    /*if(m_distance_last_point <30){
        double diff_angle = m_angleAB - m_deplacementAngle;
        //if(std::abs(diff_angle) < 0.1){
            double surface = m_distance_last_point*m_config.m_largeur/10000.0;
            m_surface += surface/2;
            m_surface_h = surface/m_time_last_point;
            
        //}

        
    }*/
}



// pure pousuite
void GpsFramework::calculAngleCorrection(){
    //naif
    //m_angle_correction = atan(m_distanceAB/m_algo_lookahead_d);
   
    //follow karott
    double angleABDeplacement = m_lineAB.m_angleAB - m_deplacementAngle;
    if(angleABDeplacement>3.14/2){
        angleABDeplacement = angleABDeplacement-3.14;
    }
    if(angleABDeplacement < -3.14/2){
        angleABDeplacement = angleABDeplacement+3.14;
    }
    //INFO(angleABDeplacement/3.14*180);
    
    double distance = m_distanceAB;
    distance = m_lineAB.distance(m_tracteur.m_x_essieu_avant, m_tracteur.m_y_essieu_avant, m_config.m_outil_largeur);
    m_angle_correction = atan(distance/m_pilot_lookahead_d)+angleABDeplacement;
    //INFO(m_angle_correction);
    //m_angle_correction = ;
    
    double angle_max = 0.5;
    if(m_angle_correction < -angle_max){
        m_angle_correction = -angle_max;
    }
    if(m_angle_correction > angle_max){
        m_angle_correction = angle_max;
    }
    //m_angle_correction = -3.14/6;
    
}
int orientation(GpsPoint & p, GpsPoint & q, GpsPoint & r)
{
    double val = (q.m_y - p.m_y) * (r.m_x - q.m_x) -
    (q.m_x - p.m_x) * (r.m_y - q.m_y);
    
    if (val == 0) return 0;  // colinear
    return (val > 0)? 1: 2; // clock or counterclock wise
}

double cp(GpsPoint & a, GpsPoint &  b){ //returns cross product
    return a.m_x*b.m_y-a.m_y*b.m_x;
}

double polygonArea(std::vector<GpsPoint_ptr> tab)
{
    int n = tab.size();
    double sum=0.0;
    for(int i=0; i<n; i++){
        sum+=cp(*tab[i], *tab[(i+1)%n]); //%n is for last triangle
    }
    return abs(sum)/2.0;
}


void GpsFramework::calculContourExterieur(){
    if(m_list.size()>10){
        m_contour.clear();
        // Find the leftmost point
        std::vector<GpsPoint_ptr> points;
        for(auto p: m_list){
            points.push_back(p);
        }
        
        int l = 0;
        int n = points.size();
        for (int i = 1; i < n; i++)
            if (points[i]->m_x < points[l]->m_x)
                l = i;
        
        // Start from leftmost point, keep moving counterclockwise
        // until reach the start point again.  This loop runs O(h)
        // times where h is number of points in result or output.
        int p = l, q;
        do
        {
            // Add current point to result
            m_contour.push_back(points[p]);
            
            // Search for a point 'q' such that orientation(p, x,
            // q) is counterclockwise for all points 'x'. The idea
            // is to keep track of last visited most counterclock-
            // wise point in q. If any point 'i' is more counterclock-
            // wise than q, then update q.
            q = (p+1)%n;
            for (int i = 0; i < n; i++)
            {
                // If i is more counterclockwise than current q, then
                // update q
                if (orientation(*points[p], *points[i], *points[q]) == 2)
                    q = i;
            }
            
            // Now q is the most counterclockwise with respect to p
            // Set p as q for next iteration, so that q is added to
            // result 'hull'
            p = q;
            
        } while (p != l);  // Whil
        m_contour.push_back(m_contour[0]);
        m_surface_exterieur = polygonArea(m_contour)/10000.0;
        m_surface_exterieur_h = m_surface_exterieur/(m_list.front()->m_timeHour - m_lineAB.m_pointA.m_timeHour);
    }
}


void GpsFramework::setVolantEngaged(bool value){
    m_pilotModule.clear();
    if(value){
        file_job_stream << "[engage]\n";
        m_pilotModule.engage();
        m_pauseDraw = false;
    } else {
        file_job_stream << "[desengage]\n";
        m_pilotModule.desengage();
        m_pauseDraw = true;
    }
}

bool GpsFramework::getVolantEngaged(){
    return m_pilotModule.m_engaged;
}

