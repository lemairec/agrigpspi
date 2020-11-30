#ifndef GPS_FRAMEWORK_H
#define GPS_FRAMEWORK_H

#include "gps_module.hpp"
#include "pilot_module.hpp"
#include "imu_module.hpp"
#include "gui/qt/my_qt_serial_port.hpp"
#include "gui/qt/my_qt_file.hpp"
#include "config/config.hpp"
#include "lineAB.hpp"
#include "curveAB.hpp"
#include "parcelle.hpp"
#include <chrono>
#include <QThread>
#include <time.h>
#include <math.h>
#include "logging.hpp"

class IGpsObserver {
public:
    virtual void onNewPoint() = 0;
};

struct OutilPosition {
    GpsPoint_ptr m_point_right;
    GpsPoint_ptr m_point_center;
    GpsPoint_ptr m_point_left;
};

typedef std::shared_ptr<OutilPosition> OutilPosition_ptr;

struct Tracteur {
    std::chrono::system_clock::time_point m_time_received;

    double m_antenne_essieu_avant = 1.5;
    double m_antenne_essieu_arriere = 1.2;
    double m_empatement = 2.7;
    double m_hauteur_antenne = 2;
    double m_antenne_lateral = 0;
    
    double m_outil_distance = 2;
    double m_outil_largeur = 2;
       
    double m_x_antenne = 0;
    double m_y_antenne = 0;
    
    
    double m_correction_lateral_imu = 0;
    double m_correction_lateral = 0;
    GpsPoint_ptr m_pt_antenne_corrige;
    GpsPoint_ptr m_pt_essieu_arriere;
     
    double m_x_essieu_avant = 0;
    double m_y_essieu_avant = 0;
    
    GpsPoint_ptr m_pt_outil_arriere;
    GpsPoint_ptr m_pt_outil_arriere_gauche;
    GpsPoint_ptr m_pt_outil_arriere_droite;
    
};

enum Etat {
    Etat_Reset,
    Etat_PointASaved,
    Etat_OK,
    Etat_ParcelleAdd,
    Etat_ParcellePause
};

enum ParcellePoint {
    ParcellePointMiddle = 0,
    ParcellePointLeft = 1,
    ParcellePointRight = 2
};

enum AlgoPilot {
    FollowKarott = 0,
    RearWheelPosition = 1,
    RWPAndFK = 2,
    
};



class SurfaceToDraw{
public:
    bool m_isClose = false;
    std::list<OutilPosition_ptr> m_points;
    OutilPosition_ptr m_lastPoint = nullptr;
};

typedef std::shared_ptr<SurfaceToDraw> SurfaceToDraw_ptr;

class TimeObserver{
    std::list<int> m_values;
    std::chrono::system_clock::time_point m_last_time_received;
public:
    double m_moy;
    double m_et;
    
    void setNewTime(){
        auto begin = std::chrono::system_clock::now();
        std::chrono::duration<double> diff = begin - m_last_time_received;

        double seconds = diff.count()*1000;
        addNewValue(seconds);
        m_last_time_received = begin;
    }
    
    bool isConnected(){
        auto begin = std::chrono::system_clock::now();
        std::chrono::duration<double> diff = begin - m_last_time_received;

        if(diff.count() < 2.0){
            return true;
        } else {
            return false;
        }
    }
    
    
    void addNewValue(double value){
        std::list<int> m_values;
        if(value > 10000){
            return;
        }
        m_values.push_front(value);
        while(m_values.size()>10){
            m_values.pop_back();
        }
        
        double sum = 0;
        for(auto c : m_values){
            sum += c;
        }
        double moy = sum/m_values.size();
        
        
        double sum2 = 0;
        for(auto c : m_values){
            sum2 += (moy-c)*(moy-c);
        }
        m_et = std::sqrt(sum2/m_values.size());
        m_moy = moy;
    }
};

class GpsFramework {
    GpsFramework();
    
    double m_distance_cap_vitesse;
    GpsPoint_ptr m_lastPoint;
    GpsPoint_ptr m_lastImportantPoint;
    
    void saveInfoFile();
public:
    static GpsFramework & Instance();
    ~GpsFramework();
    void initOrLoadConfig();

    std::string m_messages_errors;
    void addError(std::string s);
    
    void onGGAFrame(GGAFrame & frame);
    void onRMCFrame(RMCFrame_ptr frame);
    
    void onNewPoint(GpsPoint_ptr point);
    void onNewImportantPoint(GpsPoint_ptr point);
    
    void processPilot(double deplacementX, double deplacementY
        , double essieu_avant_x, double essieu_avant_y
        , double essieu_arriere_x, double essieu_arriere_y);
    void updateWithoutGps();
    
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
    Etat m_etat = Etat_Reset;
    LineAB m_lineAB;
    CurveAB m_curveAB;
    LineCurves m_lines;
    
    ParcellePoint m_parcellePoint = ParcellePointMiddle;
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
    
    std::string m_pilot_algo_str = "none";
    AlgoPilot m_pilot_algo = FollowKarott;
    double m_angle_correction;
    
    //algo follow carott
    double m_pilot_lookahead_d;
    int m_pilot_adaptive_vitesse;
    
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
    
    std::string m_pilot_last_error;
    
    TimeObserver m_gps_time;
    TimeObserver m_pilot_time;
    TimeObserver m_draw_time;
    TimeObserver m_imu_time;
    
    bool isGpsConnected();
    bool isPilotConnected();
    void exportHtml();
    
     
    void changeDraw();
    void changeDrawBool(bool b);
    bool m_pauseDraw = false;
         
    void setVolantEngaged(bool value);
    bool getVolantEngaged();
    
    Tracteur m_tracteur;
    GpsModule m_gpsModule;
    MyQTSerialPorts m_serialModule;
    MyQTFile m_fileModule;
    PilotModule m_pilotModule;
    ImuModule m_imuModule;
    
    std::list<std::string> m_listLog;
private:
    void readFile();
};

#endif // GPS_FRAMEWORK_H
