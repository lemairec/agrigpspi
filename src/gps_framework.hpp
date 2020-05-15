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

class SurfaceToDraw{
public:
    bool m_isClose = false;
    std::list<GGAFrame_ptr> m_points;
    GGAFrame_ptr m_lastPointOk = nullptr;
    GGAFrame_ptr m_lastPoint = nullptr;
};

typedef std::shared_ptr<SurfaceToDraw> SurfaceToDraw_ptr;

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
    int m_algo;
    double m_algofk_lookahead_d;
    double m_angle_correction;
    void calculAngleCorrection();
    
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
    
    void calculDraw(GGAFrame_ptr);
    
    
    std::list<GGAFrame_ptr> m_list;
    std::list<SurfaceToDraw_ptr> m_listSurfaceToDraw;
    GGAFrame m_lastGGAFrame;
    
    void exportHtml();
    
    PilotModule m_pilotModule;
    
    void changeDraw();
    bool m_pauseDraw = false;
         
    void setVolantEngaged(bool value);
    bool getVolantEngaged();
private:
    bool m_volantEngaged = false;
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
