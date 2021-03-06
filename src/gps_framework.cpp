#include "gps_framework.hpp"
#include "logging.hpp"
#include "config/config.hpp"
#include "environnement.hpp"
#include "util/util.hpp"
#include "util/directory_manager.hpp"

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

void makedir(std::string dir2){
    std::string dir = DirectoryManager::Instance().getBinDirectory() + dir2;
    std::string s2 = "mkdir -p "+ dir + ";";
    INFO(s2);
    if(system( s2.c_str() )){
        WARN("can not execute : " << s2);
    };
}

GpsFramework::GpsFramework(){
    QDateTime date = QDateTime::currentDateTime();
    QString s = date.toString("yyyyMMddThhmm");
    date_str = s.toUtf8().constData();
    
    int y = date.date().year();
    if(y>(1011+1011)){
    //    exit(0);
    }
    
    makedir("/job");
    makedir("/parcelle");
    makedir("/line");
    
    file_info = DirectoryManager::Instance().getBinDirectory() + "/job/gps_" + s.toUtf8().constData() + ".info";
    
    std::string file_job = DirectoryManager::Instance().getBinDirectory() + "/job/gps_" + s.toUtf8().constData() + ".job";
    file_job_stream.open(file_job, std::ios::out);
    
    std::string file_debug = DirectoryManager::Instance().getBinDirectory() + "/job/gps_" + s.toUtf8().constData() + ".debug";
    file_debug_stream.open(file_debug, std::ios::out);
    //mode_t mt;
    
    INFO(file_info);
    INFO(file_job);
    INFO(file_debug);
    INFO(file_job_stream.is_open());
    INFO(file_debug_stream.is_open());
    
    m_config.load();
    m_parcelles.load();
    m_lines.load();
    
}

GpsFramework::~GpsFramework(){
    m_serialModule.closeAll();
}

void GpsFramework::initOrLoadConfig(){
    m_config.save();
    
    m_pilotModule.initOrLoadConfig(m_config);
    m_serialModule.initOrLoad(m_config);
    m_fileModule.initOrLoad(m_config);
    m_ekf_module.initOrLoad(m_config);
    
    file_job_stream << "[config]\n";
    m_debug_log = m_config.m_debug_log;
    
    m_pilot_algo = (AlgoPilot)m_config.m_pilot_algo;
    m_pilot_lookahead_d = m_config.m_pilot_lookahead_d;
    m_pilot_adaptive_vitesse = m_config.m_pilot_adaptive_vitesse;
    m_pilot_rwp_kth = m_config.m_pilot_rwp_kth;
    m_pilot_rwp_kte = m_config.m_pilot_rwp_kte;
    
    m_tracteur.m_antenne_essieu_avant = m_config.m_tracteur_empatement - m_config.m_tracteur_antenne_pont_arriere;
    m_tracteur.m_antenne_essieu_arriere = m_config.m_tracteur_antenne_pont_arriere;
    m_tracteur.m_empatement = m_config.m_tracteur_empatement;
    m_tracteur.m_hauteur_antenne = m_config.m_tracteur_hauteur;
    m_tracteur.m_outil_distance = m_config.m_outil_distance;
    m_tracteur.m_outil_largeur = m_config.m_outil_largeur;
    if(m_config.m_tracteur_antenne_lateral>2){
        double d = -m_config.m_tracteur_antenne_lateral+2; //TODO c'est de la grosse merde, a refaire!
        m_tracteur.m_antenne_lateral = d;
    } else {
        m_tracteur.m_antenne_lateral = m_config.m_tracteur_antenne_lateral;
    }
    
    if(m_lineAB.m_largeur != m_config.m_largeur_AB || m_lineAB.m_offset != m_config.m_offset_AB){
        m_lineAB.m_largeur = m_config.m_largeur_AB;
        m_lineAB.m_offset = m_config.m_offset_AB;
        m_curveAB.m_largeur = m_config.m_largeur_AB;
        m_curveAB.m_offset = m_config.m_offset_AB;
        
        m_curveAB.clearWithoutAB();
        m_lineAB.clearWithoutAB();
    }
    
    m_gga = m_config.m_gga;
    
    
    m_parcelle.m_resolution_to_draw = m_config.m_resolution_draw;
    m_parcelle.calculContourToDraw();
    
    m_resolution_draw = m_config.m_resolution_draw;

    INFO(m_config.m_resolution_draw);
    
    //m_lines.loadCurveOrLine("curve_TEST");
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

GpsFramework & GpsFramework::Instance(){
    static GpsFramework gf;
    return gf;
}

void GpsFramework::setRef(double latitude, double longitude){
    m_gpsModule.setRef(latitude, longitude);
    for(auto l: m_ekf_module.m_list){
        m_gpsModule.setXY(*l);
    }
    m_ekf_module.reset();
    for(auto l: m_listSurfaceToDraw){
        m_gpsModule.setXY(*l->m_lastPoint->m_point_left);
        m_gpsModule.setXY(*l->m_lastPoint->m_point_right);
        m_gpsModule.setXY(*l->m_lastPoint->m_point_center);
        //m_gpsModule.setXY(*l->m_lastPointOk);
        for(auto l2: l->m_points){
            m_gpsModule.setXY(*l2->m_point_left);
            m_gpsModule.setXY(*l2->m_point_right);
            m_gpsModule.setXY(*l2->m_point_center);
        }
    }
    for(auto l: m_parcelle.m_contour){
        m_gpsModule.setXY(*l);
    }
    if(m_line){
        m_gpsModule.setXY(m_lineAB.m_pointA);
        m_gpsModule.setXY(m_lineAB.m_pointB);
    } else {
        m_gpsModule.setXY(m_curveAB.m_pointA);
        m_gpsModule.setXY(m_curveAB.m_pointB);
        for(auto p : m_curveAB.m_listAB){
            m_gpsModule.setXY(*p);
        }
    }
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
        m_vitesse = f->m_vitesse_kmh;
        if(m_vitesse > 0.1 && m_config.m_cap_mode == CapMode::CapMode_Rmc){
            m_deplacementAngle = f->m_cap_rad;
        }
        onNewPoint(f);
        
    }
}

void GpsFramework::onNewImuPoint(double ax, double ay, double az, double pitch_x_deg, double pitch_y_deg){
    m_ekf_module.onNewImuPoint(ax, ay, az, pitch_x_deg, pitch_y_deg);
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
    
    calculDeplacement(p);
    if(m_tracteur.m_pt_essieu_arriere){
        processPilot(m_deplacementX, m_deplacementY
                 , m_tracteur.m_x_essieu_avant, m_tracteur.m_y_essieu_avant
                 , m_tracteur.m_pt_essieu_arriere->m_x, m_tracteur.m_pt_essieu_arriere->m_y);
        m_stat_distance.addNewValue(m_distanceAB);
    }
    
    m_gps_time.setNewTime();
    
    m_lastPoint = p;
    if(m_listSurfaceToDraw.size()>0 && !m_pauseDraw){
        OutilPosition_ptr p2(new OutilPosition());
        p2->m_point_center = m_tracteur.m_pt_outil_arriere;
        p2->m_point_left = m_tracteur.m_pt_outil_arriere_gauche;
        p2->m_point_right = m_tracteur.m_pt_outil_arriere_droite;
        
        m_listSurfaceToDraw.front()->m_lastPoint = p2;
    }
    
    if(m_etat == Etat_ParcelleAdd){
        if(m_parcellePoint == ParcellePointMiddle){
            m_parcelle.addPoint(m_tracteur.m_pt_outil_arriere);
        } else if(m_parcellePoint == ParcellePointLeft){
            m_parcelle.addPoint(m_tracteur.m_pt_outil_arriere_gauche);
        } else if(m_parcellePoint == ParcellePointRight){
            m_parcelle.addPoint(m_tracteur.m_pt_outil_arriere_droite);
        }
        
    }
    
    if(m_lastImportantPoint && m_lastImportantPoint->distanceCarre(*p) < m_resolution_draw*m_resolution_draw){
        return;
    } else {
        onNewImportantPoint(p);
    }
    
    
}

void GpsFramework::onNewImportantPoint(GpsPoint_ptr p){
    
    if(m_etat == Etat_PointASaved && !m_line){
        if(m_tracteur.m_pt_antenne_corrige){
            m_curveAB.addPoint(m_tracteur.m_pt_antenne_corrige);
        }
    }
    m_lastImportantPoint = p;
    calculSurface();
    DEBUG("draw");
    if(m_tracteur.m_pt_outil_arriere){
        calculDraw(m_tracteur.m_pt_outil_arriere);
    }
    if(p){
        file_job_stream << p->m_time << "," << std::setprecision(14) << p->m_latitude << "," << p->m_longitude << std::endl;
        saveInfoFile();
    }
    
    
    
    if(m_parcelle.isInit() && m_pilot_auto){
        double dist = m_parcelle.distance(p);
        if(m_config.m_pilot_auto_deactive > 0 && dist < m_config.m_pilot_auto_deactive){
            if(m_pilotModule.m_engaged){
                setVolantEngaged(false);
            }
        }
        if(m_config.m_pilot_auto_active > 0 && dist > m_config.m_pilot_auto_deactive){
            if(!m_pilotModule.m_engaged){
                setVolantEngaged(true);
            }
        }
    }
    
}

void GpsFramework::processPilot(double deplacementX, double deplacementY
                            , double essieu_avant_x, double essieu_avant_y
                            , double essieu_arriere_x, double essieu_arriere_y){
    if(m_etat == Etat_OK){
        if(m_line){
            if(m_pilotModule.m_engaged){
                m_lineAB.calculProjete(m_tracteur.m_pt_essieu_arriere, m_deplacementX, m_deplacementY, !m_pilotModule.m_engaged);
                double dist = m_lineAB.m_proj_distance;
                setDistance(dist, false);
            }  else {
                m_lineAB.calculProjete(m_tracteur.m_pt_antenne_corrige, m_deplacementX, m_deplacementY, !m_pilotModule.m_engaged);
                double dist = m_lineAB.m_proj_distance;
                setDistance(dist, true);
            }
            
            
            if(m_pilot_algo == AlgoPilot::FollowCarrotPontAvant){
                m_angle_correction = m_lineAB.anglefollowTheCarrot(essieu_avant_x, essieu_avant_y, m_deplacementX, m_deplacementY, m_pilot_lookahead_d);
            } else if(m_pilot_algo == AlgoPilot::FollowCarrotPontArriere){
                m_angle_correction = m_lineAB.anglefollowTheCarrot(essieu_arriere_x, essieu_arriere_y, m_deplacementX, m_deplacementY, m_pilot_lookahead_d);
            } else if(m_pilot_algo == AlgoPilot::RearWheelPosition){
                m_angle_correction = m_lineAB.calculRearWheelPosition(essieu_arriere_x, essieu_arriere_y, (essieu_arriere_x+essieu_avant_x)/2, (essieu_arriere_y+essieu_avant_y)/2, m_deplacementX, m_deplacementY, m_vitesse, 1.5, m_pilot_rwp_kth, m_pilot_rwp_kte);
            } else {
                m_angle_correction = 0;
            }
        } else {
            if(m_pilotModule.m_engaged){
                m_curveAB.calculProjete(m_tracteur.m_pt_essieu_arriere, m_deplacementX, m_deplacementY, !m_pilotModule.m_engaged);
                double dist = m_curveAB.m_proj_distance;
                setDistance(dist, false);
            } else {
                m_curveAB.calculProjete(m_tracteur.m_pt_antenne_corrige, m_deplacementX, m_deplacementY, !m_pilotModule.m_engaged);
                double dist = m_curveAB.m_proj_distance;
                setDistance(dist, true);
            }
            
            
            if(m_pilot_algo == AlgoPilot::FollowCarrotPontAvant){
                m_angle_correction = m_curveAB.anglefollowTheCarrot(essieu_avant_x, essieu_avant_y, m_deplacementX, m_deplacementY, m_pilot_lookahead_d);
            } else if(m_pilot_algo == AlgoPilot::FollowCarrotPontArriere){
                m_angle_correction = m_curveAB.anglefollowTheCarrot(essieu_arriere_x, essieu_arriere_y, m_deplacementX, m_deplacementY, m_pilot_lookahead_d);
            } else if(m_pilot_algo == AlgoPilot::RearWheelPosition){
                m_angle_correction = m_curveAB.calculRearWheelPosition(essieu_arriere_x, essieu_arriere_y, (essieu_arriere_x+essieu_avant_x)/2, (essieu_arriere_y+essieu_avant_y)/2, m_deplacementX, m_deplacementY, m_vitesse, 1.5, m_pilot_rwp_kth, m_pilot_rwp_kte);
            } else {
                m_angle_correction = 0;
            }
        }
    } else {
        setDistance(0, false);
        m_angle_correction = 0;
    }
    
    if(m_pilot_adaptive_vitesse){
        m_angle_correction = m_angle_correction*8.0/m_vitesse;
    }
    
    double angle_max = 0.5;
    if(m_angle_correction < -angle_max){
        m_angle_correction = -angle_max;
    }
    if(m_angle_correction > angle_max){
        m_angle_correction = angle_max;
    }
    
    if(m_pilotModule.m_engaged){
        if(m_vitesse < 1.0){
            GpsFramework::Instance().addError("desengagement, vitesse trop faible");
            m_pilotModule.desengage();
        }
    }
    
    if(m_config.m_algo_volant_mode == AlgoVolantMode_CapteurVitesse){
        double correction = m_angle_correction - m_angle_roue_by_yawl_deg;
        m_pilotModule.turnRoue(correction*0.25);
    } else {
        m_pilotModule.setAngleRoueDesired(m_angle_correction, m_time_last_point);
    }
    
}

double GpsFramework::getOffsetAB(){
    if(m_line){
        return m_lineAB.getOffsetAB(m_tracteur.m_pt_antenne_corrige);
    } else {
        return m_curveAB.getOffsetAB(m_tracteur.m_pt_antenne_corrige);
    }
}


void GpsFramework::updateWithoutGps(){
    if(m_tracteur.m_pt_essieu_arriere){
        m_virtual_point.setNewTime();
        
        
        auto begin = std::chrono::system_clock::now();
        std::chrono::duration<double> diff = begin - m_tracteur.m_time_received;

        double seconds = diff.count();
        double vitesse_m_s = m_vitesse*1000/3600;
        double deplacement = vitesse_m_s*seconds;
        
        double essieu_avant_x = m_tracteur.m_x_essieu_avant + cos(m_deplacementAngle)*deplacement;
        double essieu_avant_y = m_tracteur.m_y_essieu_avant + sin(m_deplacementAngle)*deplacement;
        
        double essieu_arriere_x = m_tracteur.m_pt_essieu_arriere->m_x + cos(m_deplacementAngle)*deplacement;
        double essieu_arriere_y = m_tracteur.m_pt_essieu_arriere->m_y + sin(m_deplacementAngle)*deplacement;
        
        processPilot(m_deplacementX, m_deplacementY, essieu_avant_x, essieu_avant_y, essieu_arriere_x, essieu_arriere_y);
    }
}


bool GpsFramework::isGpsConnected(){
    return m_gps_time.isConnected();
}

bool GpsFramework::isPilotConnected(){
    return m_pilot_time.isConnected();
}





void GpsFramework::onFrame(const std::string &frame){
    if(m_debug_log){
        if(m_imuModule.m_pitch_y_deg != 0){
            file_debug_stream << "IMU_ANG," << m_imuModule.m_roll_x_deg << "," << m_imuModule.m_pitch_y_deg << "," << m_imuModule.m_yaw_z_deg << ",";
            file_debug_stream << "IMU_ACC," << m_imuModule.m_ax << "," << m_imuModule.m_ay << "," << m_imuModule.m_az << ",";
            file_debug_stream << "IMU_ACA," << m_imuModule.m_w_x << "," << m_imuModule.m_w_y << "," << m_imuModule.m_w_z << ",";
            file_debug_stream << "IMU_MAG," << m_imuModule.m_mag_x << "," << m_imuModule.m_mag_y << "," << m_imuModule.m_mag_z << ",FIN\n";
        }
        file_debug_stream << frame << "\n";
        
    }
}


void GpsFramework::setDistance(double distance, bool led){
    m_distanceAB = distance;
    
    double coeff = m_config.m_outil_largeur/(2*10);
    if(m_distanceAB < 0.0){
        m_ledAB = round(-m_distanceAB/coeff);
    } else {
        m_ledAB = -round(m_distanceAB/coeff);
    }
}

void GpsFramework::savePointA(){
    if(m_tracteur.m_pt_antenne_corrige){
        m_lineAB.m_pointA = *m_tracteur.m_pt_antenne_corrige;
    }
    setRef(m_lineAB.m_pointA.m_latitude, m_lineAB.m_pointA.m_longitude);
    
    file_job_stream << "[savePointA]\n";
    INFO(m_lineAB.m_pointA.m_time << " " << m_lineAB.m_pointA.m_latitude << " " << m_lineAB.m_pointA.m_longitude);
    clearSurface();
    
    m_etat = Etat_PointASaved;
}

void GpsFramework::savePointB(){
    if(m_tracteur.m_pt_antenne_corrige){
        m_lineAB.m_pointB = *m_tracteur.m_pt_antenne_corrige;
    }
    
    setAB();
    file_job_stream << "[savePointB]\n";
}

void GpsFramework::setEtat(Etat etat){
    m_etat = etat;
    if(etat == Etat_Reset){
        m_surface = 0;
        m_lastPoint = nullptr;
        //m_listSurfaceToDraw.clear();
        m_ledAB = 0;
        m_ekf_module.m_list.clear();
        m_curveAB.m_curves.clear();
        m_curveAB.m_listAB.clear();
    }
}

Etat GpsFramework::getEtat(){
    return m_etat;
}




void GpsFramework::setAB(){
    if(m_line){
        setRef((m_lineAB.m_pointA.m_latitude + m_lineAB.m_pointB.m_latitude)/2, (m_lineAB.m_pointA.m_longitude + m_lineAB.m_pointB.m_longitude)/2);
        m_lineAB.savePointB();
    } else {
        m_curveAB.m_pointA = *(m_curveAB.m_listAB[0]);
        m_curveAB.m_pointB = *(m_curveAB.m_listAB[m_curveAB.m_listAB.size()-1]);
        
        setRef((m_curveAB.m_pointA.m_latitude + m_curveAB.m_pointB.m_latitude)/2, (m_curveAB.m_pointA.m_longitude + m_curveAB.m_pointB.m_longitude)/2);
        m_curveAB.savePointB();
    }
    m_etat = Etat_OK;
}

void GpsFramework::calculDeplacement(GpsPoint_ptr p){
    if(m_vitesse < 0.3){
        return;
    }
    onNewImu();

    m_tracteur.m_pt_outil_arriere = nullptr;
    m_tracteur.m_pt_outil_arriere_droite = nullptr;
    m_tracteur.m_pt_outil_arriere_gauche = nullptr;
    m_tracteur.m_pt_antenne_corrige = nullptr;

    m_ekf_module.calculDeplacement(p, m_tracteur);
       
    if(m_ekf_module.m_cap_mode == CapMode_Rmc){
        m_deplacementX = cos(m_deplacementAngle)*m_vitesse*0.1;
        m_deplacementY = sin(m_deplacementAngle)*m_vitesse*0.1;
    } else {
        m_deplacementX = m_ekf_module.m_v_x;
        m_deplacementY = m_ekf_module.m_v_y;
        
        INFO("ici");
        m_deplacementAngle = m_ekf_module.m_deplacementAngle;
    }
    
    GpsPoint_ptr p2(new GpsPoint());
    p2->m_x = m_ekf_module.m_final_x;
    p2->m_y = m_ekf_module.m_final_y;
    
    m_ekf_module.m_list_ekf.push_front(p2);
    if(m_ekf_module.m_list_ekf.size()>100){
        m_ekf_module.m_list_ekf.pop_back();
    };
    m_tracteur.setPoint(p2, m_deplacementAngle);
    m_gpsModule.SetLatLong(*(m_tracteur.m_pt_antenne_corrige));
    m_gpsModule.SetLatLong(*(m_tracteur.m_pt_outil_arriere));
    m_gpsModule.SetLatLong(*(m_tracteur.m_pt_outil_arriere_gauche));
    m_gpsModule.SetLatLong(*(m_tracteur.m_pt_outil_arriere_droite));
    
    
    if(m_gga && m_time_last_point > 0 ){
        m_vitesse = m_distance_last_point/1000.0/m_time_last_point;
        if(m_vitesse >50){
            INFO("erreur");
        }
    }
}

void GpsFramework::changeDraw(){
    onNewImu();
    if(m_pauseDraw == false){
        m_pauseDraw = true;
    } else {
        SurfaceToDraw_ptr p(new SurfaceToDraw());
        m_listSurfaceToDraw.push_front(p);
        m_pauseDraw = false;
        onNewImportantPoint(m_lastPoint);
    }
}

void GpsFramework::changeDrawBool(bool b){
    if(b){
        m_pauseDraw = true;
    } else {
        SurfaceToDraw_ptr p(new SurfaceToDraw());
        m_listSurfaceToDraw.push_front(p);
        m_pauseDraw = false;
        if(m_lastPoint){
            onNewImportantPoint(m_lastPoint);
        }
    }
}

void GpsFramework::calculDraw(GpsPoint_ptr p){
    if(m_listSurfaceToDraw.size()==0){
        SurfaceToDraw_ptr s2(new SurfaceToDraw());
        m_listSurfaceToDraw.push_front(s2);
    }
    SurfaceToDraw_ptr s = m_listSurfaceToDraw.front();
    if(m_pauseDraw){
        return;
    }
    OutilPosition_ptr p2(new OutilPosition());
    p2->m_point_center = m_tracteur.m_pt_outil_arriere;
    p2->m_point_left = m_tracteur.m_pt_outil_arriere_gauche;
    p2->m_point_right = m_tracteur.m_pt_outil_arriere_droite;
    s->m_points.push_front(p2);
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
        if(s && s->m_points.size()>0){
            if(s->m_lastPoint && s->m_lastPoint->m_point_center){
                auto last_frame = s->m_lastPoint->m_point_center;
                double x1 = last_frame->m_x;
                double y1 = last_frame->m_y;
                for(auto it = s->m_points.begin(); it != s->m_points.end(); ++it){
                    auto frame = (*it);
                    double x0 = frame->m_point_center->m_x;
                    double y0 = frame->m_point_center->m_y;
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


void GpsFramework::setVolantEngaged(bool value){
    m_pilotModule.clear();
    m_stat_distance.clear();
    if(value){
        file_job_stream << "[engage]\n";
        m_pilotModule.engage();
        changeDrawBool(false);
    } else {
        file_job_stream << "[desengage]\n";
        m_pilotModule.desengage();
        changeDrawBool(true);
    }
}

bool GpsFramework::getVolantEngaged(){
    return m_pilotModule.m_engaged;
}

void GpsFramework::loadParcelle(const std::string & name, int flags_i, bool line, bool demi_outil){
    m_parcelle.loadParcelle(name);

    if(flags_i >=0){
        if(m_parcelle.m_flag.size()>1){
            int debut = m_parcelle.m_flag[flags_i%m_parcelle.m_flag.size()];
            int fin = m_parcelle.m_flag[(flags_i+1)%m_parcelle.m_flag.size()];
            
            auto p1 = m_parcelle.m_contour[debut];
            auto p2 = m_parcelle.m_contour[fin];
            
            if(line){
                m_line = true;
                m_lineAB.m_pointA = *p1;
                m_lineAB.m_pointB = *p2;
                /*m_lineAB.m_deplacement = 0;
                if(demi_outil){
                    m_lineAB.m_deplacement = m_config.m_outil_largeur/2;
                }*/
                setAB();
            } else {
                m_curveAB.clearAll();
                if(debut < fin){
                    m_line = false;
                    for(int i = debut; i < fin; ++i){
                        m_curveAB.addPoint(m_parcelle.m_contour[i]);
                    }
                    setAB();
                } else {
                    addError("pas la derniere courbe :(");
                }
            }
        }
    }
}

void GpsFramework::onNewImu(){
    //
    if(m_vitesse > 0.1){
        double vitesse_ms = m_vitesse*1000.0/3600.0;
        double temp = vitesse_ms/(m_tracteur.m_empatement+(m_config.m_yawl_kus/9.8)*vitesse_ms*vitesse_ms);
        
        m_angle_roue_by_yawl_deg = m_imuModule.m_w_z/temp;
        
          
    } else {
        m_angle_roue_by_yawl_deg = 0;
    }
    if(m_config.m_algo_volant_mode == AlgoVolantMode_CapteurVitesse){
        
        
    }
}
