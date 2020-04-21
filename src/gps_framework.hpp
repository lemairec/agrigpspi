#ifndef GPS_FRAMEWORK_H
#define GPS_FRAMEWORK_H

#include "gps_module.hpp"
#include "pilot_module.hpp"
#include "config/config.hpp"
#include <chrono>
#include <QThread>

class IGpsObserver {
public:
    virtual void onNewPoint() = 0;
};

class GpsFramework {
    GpsFramework();
public:
    static GpsFramework & Instance();
    void initOrLoadConfig();
    
    void addGpsObserver(IGpsObserver *);
    void removeObserver();
    void onGGAFrame(GGAFrame & frame);
    void onGGAFrame(const std::string & frame);
    void setRef(double latitude, double longitude);
    void main();
    
    double distance(GpsPoint & gpsPoint);
    
    void savePointA();
    void savePointB();
    
    void test();
    
    GGAFrame m_pointA;
    GGAFrame m_pointB;
    
    double m_distance;
    
    Config m_config;
    bool m_reloadConfig = true;
    
    bool m_pause = false;
    
    //point
    double m_a;
    double m_b;
    double m_c;
    double m_sqrt_m_a_m_b;
    
    double m_ab_x = 0.0;
    double m_ab_y = 0.0;
    
    double m_angleAB = 0;
    void setAB();
    
    //deplacement
    double m_deplacementX;
    double m_deplacementY;
    double m_deplacementAngle;
    
    double m_distance_last_point;
    double m_time_last_point;
    bool m_sensAB = true;
    double m_vitesse;
    void calculDeplacement();
    
    double m_distanceAB = 0.0;
    int m_ledAB = 0;
    
    //algo pure poursuite
    double m_angle_pure_poursuite;
    double m_poursuite_x, mm_poursuite_y;
    void calculPurePoursuite();
    
    //surface
    double m_surface = 0;
    double m_surface_h = 0;
     
    //contour_exterieur;
    double m_surface_exterieur = 0;
    double m_surface_exterieur_h = 0;
    std::vector<GGAFrame_ptr> m_contour;
    void calculContourExterieur();
    
    void clearSurface();
    void calculSurface();
    
    std::list<GGAFrame_ptr> m_list;
    GGAFrame m_lastGGAFrame;
    
    void exportHtml();
    
    PilotModule m_pilotModule;
    
private:
    IGpsObserver * m_observer = NULL;
    GpsModule m_gpsModule;
    
    void readFile();
};

class GpsThread : public QThread
{
    GpsFramework & m_framework;
public:
    GpsThread(GpsFramework & framework):
    m_framework(framework)
    {
    };
    
    void run(){
        m_framework.main();
    };
};

#endif // GPS_FRAMEWORK_H
