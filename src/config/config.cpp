#include "config.hpp"
#include <QString>
#include <QSettings>
#include "environnement.hpp"
#include "../logging.hpp"
#include "../util/directory_manager.hpp"

Config::Config(){
    m_input_gps = "none";
    m_file_gps = GPS_TEST_FILE;
    //m_input= "file";
    //m_file = ProjectSourceDir + "/gps_warmo.ubx";
    
    m_input_gps = "none";
    m_file_gps = GPS_TEST_FILE;
    m_input_gps= "file";
    m_file_gps = DirectoryManager::Instance().getSourceDirectory() + "/gps_samples/gps_travail_3m.ubx";
}

void Config::save(){
    std::string s = CONFIG_FILE;
    QString path = QString::fromStdString(s);
    QSettings settings(path, QSettings::IniFormat);
    
     
    
    settings.setValue("sensDraw", m_sensDraw);
    settings.setValue("debug", m_debug);
    settings.setValue("gga", m_gga);
    
    settings.setValue("m_lissage_gps_mode", m_lissage_gps_mode);
    settings.setValue("m_lissage_gps_ekf", m_lissage_gps_ekf);
    settings.setValue("m_cap_mode", m_cap_mode);
    settings.setValue("m_cap_ekf", m_cap_ekf);
    settings.setValue("m_cap_custom_d", m_cap_custom_d);
    settings.setValue("m_lissage_imu_mode", m_lissage_imu_mode);
    settings.setValue("m_lissage_imu_ekf", m_lissage_imu_ekf);
    
    settings.setValue("imu_correction_devers", m_imu_correction_devers);
    
    
    
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
    
    settings.setValue("motor_vitesse_min", m_motor_vitesse_min);
    settings.setValue("motor_vitesse_max", m_motor_vitesse_max);
    settings.setValue("motor_vitesse_agressivite", m_motor_vitesse_agressivite);
    
    settings.setValue("m_pilot_motor_inverse", m_pilot_motor_inverse);
    settings.setValue("m_pilot_encoder_inverse", m_pilot_encoder_inverse);
    settings.setValue("m_pilot_langage", m_pilot_langage);
    
    settings.setValue("m_pilot_algo", m_pilot_algo);
    settings.setValue("m_pilot_lookahead_d", m_pilot_lookahead_d);
    settings.setValue("m_pilot_adaptive_vitesse", m_pilot_adaptive_vitesse);
    settings.setValue("m_pilot_rwp_kth", m_pilot_rwp_kth);
    settings.setValue("m_pilot_rwp_kte", m_pilot_rwp_kte);
    settings.setValue("m_pilot_auto_deactive", m_pilot_auto_deactive);
    settings.setValue("m_pilot_auto_active", m_pilot_auto_active);
    settings.setValue("m_pilot_frequence", m_pilot_frequence);
    
    settings.setValue("m_algo_volant_mode", m_algo_volant_mode);
    settings.setValue("m_agressivite_yawl", m_agressivite_yawl);
    settings.setValue("m_yawl_kus", m_yawl_kus);
    
    settings.setValue("m_volant_pas_by_tour", m_volant_pas_by_tour);
    settings.setValue("m_volant_angle_by_tour", m_volant_angle_by_tour);
    settings.setValue("m_volant_derive", m_volant_derive);
    
    settings.setValue("distance_cap_vitesse", m_distance_cap_vitesse);
    
    settings.setValue("tracteur_antenne_pont_arriere", m_tracteur_antenne_pont_arriere);
    settings.setValue("tracteur_empatement", m_tracteur_empatement);
    settings.setValue("tracteur_hauteur", m_tracteur_hauteur);
    settings.setValue("tracteur_antenne_lateral", m_tracteur_antenne_lateral);
    
    settings.setValue("outil_largeur", m_outil_largeur);
    settings.setValue("outil_distance", m_outil_distance);
    
    settings.setValue("largeur_AB", m_largeur_AB);
    settings.setValue("largeur_AB_set", m_largeur_AB_set);
    /*settings.setValue("offset_AB", m_offset_AB);
    settings.setValue("angle_AB", m_angle_AB);*/
    
    settings.setValue("3d", m_3d);
    
    m_resolution_draw = 3;
    m_resolution_calcul = 3;
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
    
    if(settings.contains("m_lissage_gps_mode")){
        m_lissage_gps_mode = settings.value("m_lissage_gps_mode").toInt();
    }
    if(settings.contains("m_lissage_gps_ekf")){
        m_lissage_gps_ekf = settings.value("m_lissage_gps_ekf").toDouble();
    }
    if(settings.contains("m_cap_mode")){
        m_cap_mode = settings.value("m_cap_mode").toInt();
    }
    if(settings.contains("m_cap_ekf")){
        m_cap_ekf = settings.value("m_cap_ekf").toDouble();
    }
    if(settings.contains("m_cap_custom_d")){
        m_cap_custom_d = settings.value("m_cap_custom_d").toDouble();
    }
    
    
    if(settings.contains("m_lissage_imu_mode")){
        m_lissage_imu_mode = settings.value("m_lissage_imu_mode").toInt();
    }
    if(settings.contains("m_lissage_imu_ekf")){
        m_lissage_imu_ekf = settings.value("m_lissage_imu_ekf").toDouble();
    }
    
    if(settings.contains("imu_correction_devers")){
        m_imu_correction_devers = settings.value("imu_correction_devers").toBool();
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
    
    if(settings.contains("motor_vitesse_min")){
        m_motor_vitesse_min = settings.value("motor_vitesse_min").toDouble();
    }
    if(settings.contains("motor_vitesse_max")){
        m_motor_vitesse_max = settings.value("motor_vitesse_max").toDouble();
    }
    if(settings.contains("motor_vitesse_agressivite")){
        m_motor_vitesse_agressivite = settings.value("motor_vitesse_agressivite").toDouble();
    }
    if(settings.contains("m_pilot_frequence")){
        m_pilot_frequence = settings.value("m_pilot_frequence").toInt();
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
    if(settings.contains("m_pilot_adaptive_vitesse")){
        m_pilot_adaptive_vitesse = settings.value("m_pilot_adaptive_vitesse").toDouble();
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
    
    if(settings.contains("m_algo_volant_mode")){
        m_algo_volant_mode = (AlgoVolantMode) settings.value("m_algo_volant_mode").toInt();
    }
    
    if(settings.contains("m_agressivite_yawl")){
        m_agressivite_yawl = settings.value("m_agressivite_yawl").toDouble();
    }
    
    if(settings.contains("m_yawl_kus")){
        m_yawl_kus = settings.value("m_yawl_kus").toDouble();
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
    if(settings.contains("tracteur_antenne_lateral")){
        m_tracteur_antenne_lateral = settings.value("tracteur_antenne_lateral").toDouble();
    }

    if(settings.contains("outil_largeur")){
        m_outil_largeur = settings.value("outil_largeur").toDouble();
    }
    if(settings.contains("outil_distance")){
        m_outil_distance = settings.value("outil_distance").toDouble();
    }
    if(settings.contains("largeur_AB")){
        m_largeur_AB = settings.value("largeur_AB").toDouble();
    }
    if(settings.contains("largeur_AB_set")){
        m_largeur_AB_set = settings.value("largeur_AB_set").toDouble();
    }
    /*if(settings.contains("offset_AB")){
        m_offset_AB = settings.value("offset_AB").toDouble();
    }
    if(settings.contains("angle_AB")){
        m_angle_AB = settings.value("angle_AB").toDouble();
    }*/
    
    if(settings.contains("3d")){
        m_3d = settings.value("3d").toBool();
    }
}
