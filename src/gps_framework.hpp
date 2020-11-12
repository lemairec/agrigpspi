#ifndef GPS_FRAMEWORK_H
#define GPS_FRAMEWORK_H

#include "gps_module.hpp"
#include "pilot_module.hpp"
#include "gui/qt/my_qt_serial_port.hpp"
#include "gui/qt/my_qt_file.hpp"
#include "config/config.hpp"
#include "lineAB.hpp"
#include "curveAB.hpp"
#include "parcelle.hpp"
#include <chrono>
#include <QThread>
#include <time.h>

class IGpsObserver {
public:
    virtual void onNewPoint() = 0;
};

struct Tracteur {
    double m_antenne_essieu_avant = 1.5;
    double m_antenne_essieu_arriere = 1.2;
    double m_empatement = 2.7;
    double m_outil_arriere = 2;
    
    double m_x_antenne = 0;
    double m_y_antenne = 0;
    
    double m_x_essieu_avant = 0;
    double m_y_essieu_avant = 0;
    double m_x_essieu_arriere = 0;
    double m_y_essieu_arriere = 0;
    
    double m_x_outil_arriere = 0;
    double m_y_outil_arriere = 0;
    
};

enum Etat {
    Etat_Reset,
    Etat_PointASaved,
    Etat_OK,
    Etat_ParcelleAdd
};

enum AlgoPilot {
    FollowKarott = 0,
    FollowKarottVitesse = 1,
    RearWheelPosition = 2
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
    
    double m_distance_cap_vitesse;
    GpsPoint_ptr m_lastPoint;
    GpsPoint_ptr m_lastImportantPoint;
    
    void saveInfoFile();
public:
    static GpsFramework & Instance();
    void initOrLoadConfig();

    std::string m_messages_errors;
    void addError(std::string s);
    
    void onGGAFrame(GGAFrame & frame);
    void onRMCFrame(RMCFrame_ptr frame);
    
    void onNewPoint(GpsPoint_ptr point);
    void onNewImportantPoint(GpsPoint_ptr point);

    void onFrame(const std::string & frame);
    void setRef(double latitude, double longitude);
    
    void setDistance(double distance);
    
    void savePointA();
    void savePointB();
    
    void test();
    
    Config m_config;
    
    bool m_debug_log = false;
    bool m_gga = false;
    
    bool m_reloadConfig = true;
    
    bool m_line = true;
    Etat m_etat = Etat_OK;
    LineAB m_lineAB;
    CurveAB m_curveAB;
    LineCurves m_lines;
    
    Parcelle m_parcelle;
    Parcelles m_parcelles;
    
    void setEtat(Etat etat);
    void setAB();
    
    //deplacement
    double m_deplacementX;
    double m_deplacementY;
    double m_deplacementAngle;
    
    double m_distance_last_point;
    double m_time_last_point;
    double m_vitesse;
    void calculDeplacement();
    
    double m_distanceAB = 0.0;
    int m_ledAB = 0;
    
    AlgoPilot m_pilot_algo = FollowKarott;
    double m_angle_correction;
    
    //algo follow carott
    double m_pilot_lookahead_d;
    double m_pilot_lookahead_vd;
    
    //algo rear wheel position
    double m_pilot_rwp_kth = 0.5;
    double m_pilot_rwp_kte = 0.5;
    
    //surface
    double m_surface = 0;
    double m_surface_h = 0;
    
    bool m_pilot_auto = false;
     
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
    
    void setNewGpsTime();
    std::list<int> m_gps_times;
    double m_gps_time_moy;
    double m_gps_time_et;
    std::chrono::system_clock::time_point m_last_gps_received;
   
    void setNewPilotTime();
    std::list<int> m_pilot_times;
    double m_pilot_time_moy;
    double m_pilot_time_et;
    std::chrono::system_clock::time_point m_last_pilot_received;
    
    bool isGpsConnected();
    bool isPilotConnected();
    void exportHtml();
    
     
    void changeDraw();
    bool m_pauseDraw = false;
         
    void setVolantEngaged(bool value);
    bool getVolantEngaged();
    
    Tracteur m_tracteur;
    GpsModule m_gpsModule;
    MyQTSerialPorts m_serialModule;
    MyQTFile m_fileModule;
    PilotModule m_pilotModule;
    
    std::list<std::string> m_listLog;
private:
    void readFile();
};

#endif // GPS_FRAMEWORK_H
