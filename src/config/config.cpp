#include "config.hpp"
#include <QString>
#include <QSettings>
#include "environnement.hpp"
#include "../logging.hpp"

Config::Config(){
    m_input_gps = "none";
    m_file_gps = GPS_TEST_FILE;
    //m_input= "file";
    //m_file = ProjectSourceDir + "/gps_warmo.ubx";
    
    m_input_gps = "none";
    m_file_gps = GPS_TEST_FILE;
    m_input_gps= "file";
    m_file_gps = ProjectSourceDir + "/gps_samples/gps_travail_3m.ubx";
}

void Config::save(){
    std::string s = CONFIG_FILE;
    QString path = QString::fromStdString(s);
    QSettings settings(path, QSettings::IniFormat);
    
     
    
    settings.setValue("sensDraw", m_sensDraw);
    settings.setValue("debug", m_debug);
    settings.setValue("gga", m_gga);
    
    settings.setValue("menu_vertical", m_menu_vertical);
    settings.setValue("debug_log", m_debug_log);
    settings.setValue("fullscreen", m_fullscreen);
    
    
    QString input_gps = QString::fromStdString(m_input_gps);
    settings.setValue("input_gps", input_gps);
    settings.setValue("baudrate_gps", m_baudrate_gps);
    settings.setValue("file_gps", QString::fromStdString(m_file_gps));
    
    QString inputPilot = QString::fromStdString(m_inputPilot);
    settings.setValue("inputPilot", inputPilot);
    settings.setValue("baudratePilot", m_baudratePilot);
    
    QString inputImu = QString::fromStdString(m_imu_input);
    settings.setValue("inputImu", inputImu);
    settings.setValue("baudrateImu", m_imu_baudrate);
    settings.setValue("m_imu_moy", m_imu_moy);
    
    settings.setValue("motor_vitesse_min", m_motor_vitesse_min);
    settings.setValue("motor_vitesse_max", m_motor_vitesse_max);
    settings.setValue("motor_vitesse_agressivite", m_motor_vitesse_agressivite);
    
    settings.setValue("m_pilot_motor_inverse", m_pilot_motor_inverse);
    settings.setValue("m_pilot_encoder_inverse", m_pilot_encoder_inverse);
    settings.setValue("m_pilot_langage", m_pilot_langage);
    
    settings.setValue("m_pilot_algo", m_pilot_algo);
    settings.setValue("m_pilot_lookahead_d", m_pilot_lookahead_d);
    settings.setValue("m_pilot_lookahead_vd", m_pilot_lookahead_vd);
    settings.setValue("m_pilot_rwp_kth", m_pilot_rwp_kth);
    settings.setValue("m_pilot_rwp_kte", m_pilot_rwp_kte);
    settings.setValue("m_pilot_auto_deactive", m_pilot_auto_deactive);
    settings.setValue("m_pilot_auto_active", m_pilot_auto_active);
    
    settings.setValue("m_volant_pas_by_tour", m_volant_pas_by_tour);
    settings.setValue("m_volant_angle_by_tour", m_volant_angle_by_tour);
    settings.setValue("m_volant_derive", m_volant_derive);
    
    settings.setValue("distance_cap_vitesse", m_distance_cap_vitesse);
    
    settings.setValue("tracteur_antenne_pont_arriere", m_tracteur_antenne_pont_arriere);
    settings.setValue("tracteur_empatement", m_tracteur_empatement);
    settings.setValue("tracteur_hauteur", m_tracteur_hauteur);
    
    settings.setValue("outil_largeur", m_outil_largeur);
    settings.setValue("outil_distance", m_outil_distance);
    
}

void Config::load(){
    std::string s = CONFIG_FILE;
    QString qpath = QString::fromStdString(s);
    QSettings settings(qpath, QSettings::IniFormat);
    
    if(settings.contains("sensDraw")){
        m_sensDraw = settings.value("sensDraw").toBool();
    }
    if(settings.contains("debug")){
        m_debug = settings.value("debug").toBool();
    }
    if(settings.contains("gga")){
        m_gga = settings.value("gga").toBool();
    }
    
    if(settings.contains("menu_vertical")){
        m_menu_vertical = settings.value("menu_vertical").toBool();
    }
    if(settings.contains("debug_log")){
        m_debug_log = settings.value("debug_log").toBool();
    }
    if(settings.contains("fullscreen")){
        m_fullscreen = settings.value("fullscreen").toBool();
    }
    
    if(settings.contains("input_gps")){
        m_input_gps = settings.value("input_gps").toString().toUtf8().constData();
    }
    if(settings.contains("baudrate_gps")){
        m_baudrate_gps = settings.value("baudrate_gps").toInt();
    }
    if(settings.contains("file_gps")){
        m_file_gps = settings.value("file_gps").toString().toUtf8().constData();
    }
    
    if(settings.contains("inputPilot")){
        m_inputPilot = settings.value("inputPilot").toString().toUtf8().constData();
    }
    if(settings.contains("baudratePilot")){
        m_baudratePilot = settings.value("baudratePilot").toInt();
    }
    
    if(settings.contains("inputImu")){
        m_imu_input = settings.value("inputImu").toString().toUtf8().constData();
    }
    if(settings.contains("baudrateImu")){
        m_imu_baudrate = settings.value("baudrateImu").toInt();
    }
    if(settings.contains("m_imu_moy")){
        m_imu_moy = settings.value("m_imu_moy").toDouble();
    }
    
    if(settings.contains("motor_vitesse_min")){
        m_motor_vitesse_min = settings.value("motor_vitesse_min").toDouble();
    }
    if(settings.contains("motor_vitesse_max")){
        m_motor_vitesse_max = settings.value("motor_vitesse_max").toDouble();
    }
    if(settings.contains("motor_vitesse_agressivite")){
        m_motor_vitesse_agressivite = settings.value("motor_vitesse_agressivite").toDouble();
    }
    
    
    if(settings.contains("m_pilot_motor_inverse")){
        m_pilot_motor_inverse = settings.value("m_pilot_motor_inverse").toBool();
    }
    if(settings.contains("m_pilot_encoder_inverse")){
        m_pilot_encoder_inverse = settings.value("m_pilot_encoder_inverse").toBool();
    }
    if(settings.contains("m_pilot_langage")){
        m_pilot_langage = settings.value("m_pilot_langage").toInt();
    }

    if(settings.contains("m_pilot_algo")){
        m_pilot_algo = settings.value("m_pilot_algo").toInt();
    }
    if(settings.contains("m_pilot_lookahead_d")){
        m_pilot_lookahead_d = settings.value("m_pilot_lookahead_d").toDouble();
    }
    if(settings.contains("m_pilot_lookahead_vd")){
        m_pilot_lookahead_vd = settings.value("m_pilot_lookahead_vd").toDouble();
    }
    if(settings.contains("m_pilot_rwp_kth")){
        m_pilot_rwp_kth = settings.value("m_pilot_rwp_kth").toDouble();
    }
    if(settings.contains("m_pilot_rwp_kte")){
        m_pilot_rwp_kte = settings.value("m_pilot_rwp_kte").toDouble();
    }
    if(settings.contains("m_pilot_auto_active")){
        m_pilot_auto_active = settings.value("m_pilot_auto_active").toInt();
    }
    if(settings.contains("m_pilot_auto_deactive")){
        m_pilot_auto_deactive = settings.value("m_pilot_auto_deactive").toInt();
    }
    
    if(settings.contains("m_volant_pas_by_tour")){
        m_volant_pas_by_tour = settings.value("m_volant_pas_by_tour").toDouble();
    }
    if(settings.contains("m_volant_angle_by_tour")){
        m_volant_angle_by_tour = settings.value("m_volant_angle_by_tour").toDouble();
    }
    if(settings.contains("m_volant_derive")){
        m_volant_derive = settings.value("m_volant_derive").toDouble();
    }
    if(settings.contains("distance_cap_vitesse")){
        m_distance_cap_vitesse = settings.value("distance_cap_vitesse").toDouble();
    }
    
    if(settings.contains("tracteur_antenne_pont_arriere")){
        m_tracteur_antenne_pont_arriere = settings.value("tracteur_antenne_pont_arriere").toDouble();
    }
    if(settings.contains("tracteur_empatement")){
        m_tracteur_empatement = settings.value("tracteur_empatement").toDouble();
    }
    if(settings.contains("tracteur_hauteur")){
        m_tracteur_hauteur = settings.value("tracteur_hauteur").toDouble();
    }

    if(settings.contains("outil_largeur")){
        m_outil_largeur = settings.value("outil_largeur").toDouble();
    }
    if(settings.contains("outil_distance")){
        m_outil_distance = settings.value("outil_distance").toDouble();
    }
    
    
}
