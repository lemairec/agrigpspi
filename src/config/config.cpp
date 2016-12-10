#include "config.hpp"
#include <QString>
#include <QSettings>
#include "../environnement.hpp"
#include "../logging.hpp"


void Config::save(){
    std::string s = ProjectSourceBin + "/agrigps.ini";
    QString path = QString::fromStdString(s);
    QSettings settings(path, QSettings::IniFormat);
    
    settings.setValue("largeur", m_largeur);
}

void Config::load(){
    std::string s = ProjectSourceBin + "/agrigps.ini";
    QString qpath = QString::fromStdString(s);
    QSettings settings(qpath, QSettings::IniFormat);
    
    if(settings.contains("largeur")){
        m_largeur = settings.value("largeur").toInt();
    }
    INFO(m_largeur);
}
