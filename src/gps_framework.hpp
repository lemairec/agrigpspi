#ifndef GPS_FRAMEWORK_H
#define GPS_FRAMEWORK_H

#include <QThread>
#include "gps_module.hpp"
#include "config/config.hpp"

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
    void setAB();
    
    //deplacement
    double m_deplacementX;
    double m_deplacementY;
    double m_deplacementAngle;
    bool m_sensAB = true;
    double m_vitesse;
    void calculDeplacement();
    
    double m_distanceAB = 0.0;
    int m_ledAB = 0;
    
    std::list<GGAFrame *> m_list;
    
    void exportHtml();
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
