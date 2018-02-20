#include "config.hpp"
#include <QString>
#include <QSettings>
#include "environnement.hpp"
#include "../logging.hpp"

Config::Config(){
    m_input = "none";
    m_file = GPS_TEST_FILE;
    //m_input= "file";
    //m_file = ProjectSourceDir + "/gps_warmo.ubx";
    
    m_input = "none";
    m_file = GPS_TEST_FILE;
    m_input= "file";
    m_file = ProjectSourceDir + "/gps_travail_3m.ubx";
}

void Config::save(){
    std::string s = CONFIG_FILE;
    QString path = QString::fromStdString(s);
    QSettings settings(path, QSettings::IniFormat);
    
    settings.setValue("a_lat", m_a_lat);
    settings.setValue("a_lon", m_a_lon);
    settings.setValue("b_lat", m_b_lat);
    settings.setValue("b_lon", m_b_lon);
    
    
    settings.setValue("largeur", m_largeur);
    QString input2 = QString::fromStdString(m_input);
    settings.setValue("input", input2);
}

void Config::load(){
    std::string s = CONFIG_FILE;
    QString qpath = QString::fromStdString(s);
    QSettings settings(qpath, QSettings::IniFormat);
    
    if(settings.contains("largeur")){
        m_largeur = settings.value("largeur").toDouble();
    }
    if(settings.contains("input")){
        m_input = settings.value("input").toString().toUtf8().constData();
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
    
    INFO(m_largeur);
}
