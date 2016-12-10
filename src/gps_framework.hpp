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
    void onGGAFrame(GGAFrame * frame);
    void onGGAFrame(const std::string & frame);
    void main();
    
    double distance(GpsFrame & gpsPoint);
    
    void savePointA();
    void savePointB();
    
    void test();
    
    GGAFrame * m_pointA = NULL;
    GGAFrame * m_pointB = NULL;
    
    double m_distance;
    
    Config m_config;
    
    //point
    double m_a;
    double m_b;
    double m_c;
    double m_sqrt_m_a_m_b;
    void setAB();
    
    //deplacement
    double m_deplacementX;
    double m_deplacementY;
    double m_deplacementAngle;
    void calculDeplacement();
    
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
