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
    m_file_gps = ProjectSourceDir + "/gps_travail_3m.ubx";
}

void Config::save(){
    std::string s = CONFIG_FILE;
    QString path = QString::fromStdString(s);
    QSettings settings(path, QSettings::IniFormat);
    
     
    
    settings.setValue("largeur", m_largeur);
    settings.setValue("sensDraw", m_sensDraw);
    settings.setValue("debug", m_debug);
    
    QString input_gps = QString::fromStdString(m_input_gps);
    settings.setValue("input_gps", input_gps);
    settings.setValue("baudrate_gps", m_baudrate_gps);
    settings.setValue("file_gps", QString::fromStdString(m_file_gps));
    
    QString inputPilot = QString::fromStdString(m_inputPilot);
    settings.setValue("inputPilot", inputPilot);
    settings.setValue("baudratePilot", m_baudratePilot);
    settings.setValue("vitesse_motor", m_vitesse_motor);
    
    settings.setValue("a_lat", m_a_lat);
    settings.setValue("a_lon", m_a_lon);
    settings.setValue("b_lat", m_b_lat);
    settings.setValue("b_lon", m_b_lon);
    
    settings.setValue("m_pilot_inverse", m_pilot_inverse);
    settings.setValue("m_pilot_langage", m_pilot_langage);
    
    settings.setValue("m_algo", m_algo);
    settings.setValue("m_algo_lookahead_d", m_algo_lookahead_d);
    settings.setValue("m_algo2", m_algo2);
    settings.setValue("m_algo2_goto_pas_by_tour", m_algo2_goto_pas_by_tour);
    settings.setValue("m_algo2_goto_angle_by_tour", m_algo2_goto_angle_by_tour);
    settings.setValue("m_algo2_pid_p", m_algo2_pid_p);
    settings.setValue("m_algo2_pid_i", m_algo2_pid_i);
    settings.setValue("m_algo2_pid_d", m_algo2_pid_d);
    settings.setValue("m_algo2_my_k", m_algo2_my_k);
    settings.setValue("m_algo2_my_p", m_algo2_my_p);
}

void Config::load(){
    std::string s = CONFIG_FILE;
    QString qpath = QString::fromStdString(s);
    QSettings settings(qpath, QSettings::IniFormat);
    
    if(settings.contains("largeur")){
        m_largeur = settings.value("largeur").toDouble();
    }
    if(settings.contains("sensDraw")){
        m_sensDraw = settings.value("sensDraw").toBool();
    }
    if(settings.contains("debug")){
        m_debug = settings.value("debug").toBool();
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
    if(settings.contains("vitesse_motor")){
        m_vitesse_motor = settings.value("vitesse_motor").toInt();
    }
    
    if(settings.contains("a_lat")){
        m_a_lat = settings.value("a_lat").toDouble();
    }
    if(settings.contains("a_lon")){
        m_a_lon = settings.value("a_lon").toDouble();
    }
    if(settings.contains("b_lat")){
        m_b_lat = settings.value("b_lat").toDouble();
    }
    if(settings.contains("b_lon")){
        m_b_lon = settings.value("b_lon").toDouble();
    }
    
    if(settings.contains("m_pilot_inverse")){
        m_pilot_inverse = settings.value("m_pilot_inverse").toBool();
    }
    if(settings.contains("m_pilot_langage")){
        m_pilot_langage = settings.value("m_pilot_langage").toInt();
    }
    

    if(settings.contains("m_algo")){
        m_algo = settings.value("m_algo").toInt();
    }
    if(settings.contains("m_algo_lookahead_d")){
        m_algo_lookahead_d = settings.value("m_algo_lookahead_d").toDouble();
    }
    
    if(settings.contains("m_algo2")){
        m_algo2 = settings.value("m_algo2").toInt();
    }
    if(settings.contains("m_algo2_goto_pas_by_tour")){
        m_algo2_goto_pas_by_tour = settings.value("m_algo2_goto_pas_by_tour").toDouble();
    }
    if(settings.contains("m_algo2_goto_rel_s")){
        m_algo2_goto_angle_by_tour = settings.value("m_algo2_goto_angle_by_tour").toDouble();
    }
    if(settings.contains("m_algo2_pid_p")){
        m_algo2_pid_p = settings.value("m_algo2_pid_p").toDouble();
    }
    if(settings.contains("m_algo2_pid_i")){
        m_algo2_pid_i = settings.value("m_algo2_pid_i").toDouble();
    }
    if(settings.contains("m_algo2_pid_d")){
        m_algo2_pid_d = settings.value("m_algo2_pid_d").toDouble();
    }
    if(settings.contains("m_algo2_my_k")){
        m_algo2_my_k = settings.value("m_algo2_my_k").toDouble();
    }
    if(settings.contains("m_algo2_my_p")){
        m_algo2_my_p = settings.value("m_algo2_my_p").toDouble();
    }
    
    
}
