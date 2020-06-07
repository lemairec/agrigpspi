#ifndef GPS_FRAMEWORK_H
#define GPS_FRAMEWORK_H

#include "gps_module.hpp"
#include "pilot_module.hpp"
#include "gui/qt/my_qt_serial_port.hpp"
#include "config/config.hpp"
#include <chrono>
#include <QThread>
#include <time.h>

class IGpsObserver {
public:
    virtual void onNewPoint() = 0;
};

class SurfaceToDraw{
public:
    bool m_isClose = false;
    std::list<GpsPoint_ptr> m_points;
    GpsPoint_ptr m_lastPointOk = nullptr;
    GpsPoint_ptr m_lastPoint = nullptr;
};

typedef std::shared_ptr<SurfaceToDraw> SurfaceToDraw_ptr;

class GpsFramework {
    GpsFramework();
public:
    static GpsFramework & Instance();
    void initOrLoadConfig();

    std::string m_messages_errors;
    void addError(std::string s);
    
    void addGpsObserver(IGpsObserver *);
    void removeObserver();
    void onGGAFrame(GGAFrame & frame);
    void onRMCFrame(RMCFrame_ptr frame);
    void onFrame(const std::string & frame);
    void setRef(double latitude, double longitude);
    
    double distance(GpsPoint & gpsPoint);
    
    void savePointA();
    void savePointB();
    
    void test();
    
    GpsPoint m_pointA;
    GpsPoint m_pointB;
    
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
    double m_algo_naif_k;
    double m_algofk_lookahead_d;
    double m_angle_correction;
    void calculAngleCorrection();
    
    //surface
    double m_surface = 0;
    double m_surface_h = 0;
     
    //contour_exterieur;
    double m_surface_exterieur = 0;
    double m_surface_exterieur_h = 0;
    std::vector<GpsPoint_ptr> m_contour;
    void calculContourExterieur();
    
    void clearSurface();
    void calculSurface();
    
    void calculDraw(GpsPoint_ptr);
    
    
    std::list<GpsPoint_ptr> m_list;
    std::list<SurfaceToDraw_ptr> m_listSurfaceToDraw;
    GGAFrame m_lastGGAFrame;
    RMCFrame_ptr m_lastRMCFrame;
    time_t m_last_gga_received;
    
    bool isGpsConnected();
    void exportHtml();
    
     
    void changeDraw();
    bool m_pauseDraw = false;
         
    void setVolantEngaged(bool value);
    bool getVolantEngaged();
    
    GpsModule m_gpsModule;
    MyQTSerialPorts m_serialModule;
    PilotModule m_pilotModule;
    
private:
    bool m_volantEngaged = false;
    IGpsObserver * m_observer = NULL;
    
    void readFile();
};

#endif // GPS_FRAMEWORK_H
