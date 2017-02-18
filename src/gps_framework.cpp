#include "gps_framework.hpp"
#include "logging.hpp"
#include "config/config.hpp"
#include "environnement.hpp"

#include <math.h>
#include <iostream>
#include <fstream>
std::ofstream gpslogFile;


GpsFramework::GpsFramework(){
    gpslogFile.open(GPS_LOG_FILE, std::ios::out);
    m_config.load();
    initOrLoadConfig();
}

void GpsFramework::addGpsObserver(IGpsObserver * o){
    m_observer = o;
}

void GpsFramework::removeObserver(){
    m_observer = NULL;
}

void GpsFramework::initOrLoadConfig(){
    m_config.save();
}

GpsFramework & GpsFramework::Instance(){
    static GpsFramework gf;
    return gf;
}

void GpsFramework::onGGAFrame(GGAFrame & f){
    GGAFrame * frame = new GGAFrame(f);
    m_list.push_front(frame);
    calculDeplacement();
    m_distance = distance(*frame);
    if(m_observer){
        m_observer->onNewPoint();
    }
}

void GpsFramework::onGGAFrame(const std::string &frame){
    gpslogFile << frame << "\n";
}


double GpsFramework::distance(GpsPoint & gpsPoint){
    if(m_pointA.m_x!=0 && m_pointB.m_x!=0){
        double dist = (m_a * gpsPoint.m_x + m_b * gpsPoint.m_y + m_c)/m_sqrt_m_a_m_b;
        if(!m_sensAB){
            dist = -dist;
        }
        dist = fmod(dist, m_config.m_largeur);
        if(dist > m_config.m_largeur/2){
            dist -= m_config.m_largeur;
        }
        if(dist < -m_config.m_largeur/2){
            dist += m_config.m_largeur;
        }
        if(dist < -m_config.m_largeur/2){
            dist += m_config.m_largeur;
        }
        if(dist > m_config.m_largeur/2){
            dist -= m_config.m_largeur;
        }
        double coeff = m_config.m_largeur/(2*6);
        if(dist < 0.0){
            dist = -dist;
            m_ledAB = round(dist/coeff) + 1;
        } else {
            m_ledAB = -round(dist/coeff) - 1;
        }
        m_distanceAB = dist;
        //INFO("distance Point AB " << dist);
        return dist;
    }else if(m_pointA.m_x!=0){
        //double dist = distanceBetween(m_pointA, gpsPoint);
        double dx = m_pointA.m_x - gpsPoint.m_x;
        double dy = m_pointA.m_y - gpsPoint.m_y;
        double dist = sqrt(dx*dx + dy*dy);
        //INFO("distance Point A " << dist);
        return dist;
        
   } else {
        return 0.0;
    }
}

void GpsFramework::savePointA(){
    if(!m_list.empty()){
        m_pointA = *(*m_list.begin());
    }
    if(m_observer){
        m_observer->onNewPoint();
    }
    gpslogFile << "[savePointA]\n";
    INFO(m_pointA.m_time << " " << m_pointA.m_latitude << " " << m_pointA.m_longitude);
}

void GpsFramework::savePointB(){
    if(!m_list.empty()){
        m_pointB = *(*m_list.begin());
    }
    INFO(m_pointB.m_time << " " << m_pointB.m_latitude << " " << m_pointB.m_longitude);
    if(m_pointA.m_x!=0 && m_pointB.m_x!=0){
        setAB();
    }
    if(m_observer){
        m_observer->onNewPoint();
    }
    gpslogFile << "[savePointB]\n";
}

void GpsFramework::setAB(){
    m_ab_x = m_pointB.m_x - m_pointA.m_x;
    m_ab_y = m_pointB.m_y - m_pointA.m_y;
    
    
    m_a = -(m_pointB.m_y - m_pointA.m_y);
    m_b = m_pointB.m_x - m_pointA.m_x;
    m_c = -m_a * m_pointA.m_x - m_b *  m_pointA.m_y;
    m_sqrt_m_a_m_b = sqrt(m_a*m_a + m_b*m_b);
    INFO("yb  " << m_pointB.m_y << " ya " << m_pointA.m_y << " xb " << m_pointB.m_x << " xa " << m_pointA.m_x);
    INFO(m_a << "*x + " << m_b << "*y + " << m_c << " = 0; " << m_sqrt_m_a_m_b);
}

void GpsFramework::calculDeplacement(){
    if(m_list.size() > 2){
        GpsPoint * point1 = m_list.front();
        GpsPoint * point2 = NULL;
        int i = 0;
        for(auto point : m_list){
            point2 = point;
            i++;
            if(i > 20){
                break;
            }
        }
        m_deplacementX = point1->m_x - point2->m_x;
        m_deplacementY = point1->m_y - point2->m_y;
        if(m_deplacementX != 0){
            m_deplacementAngle = atan(m_deplacementY/m_deplacementX);
        }
        
        if(m_ab_x != 0 || m_ab_y != 0){
            double det = m_ab_x*m_deplacementY + m_ab_y*m_deplacementX;
            m_sensAB = (det < 0);
        }
        
        double distance = sqrt(m_deplacementX*m_deplacementX + m_deplacementY*m_deplacementY)/1000.0;
        double deplacementTime = point1->m_timeHour - point2->m_timeHour;
        m_vitesse = distance/deplacementTime;
        //INFO(deplacementTime << " " << vitesse);
    }
}

#include <iostream>
#include "serial.hpp"

using namespace std;
using namespace boost;


void GpsFramework::exportHtml(){
    ostringstream oss;
    oss << "<html><body>" << std::endl;

    oss << "<div id=\"mapdiv\"></div>" << std::endl;
    oss << "<script src=\"http://www.openlayers.org/api/OpenLayers.js\"></script>" << std::endl;
    oss << "<script>" << std::endl;
    oss << "map = new OpenLayers.Map(\"mapdiv\");" << std::endl;
    oss << "map.addLayer(new OpenLayers.Layer.OSM());" << std::endl;
    
    oss << "function addPoint(lat, long, markers){" << std::endl;
    oss << "    var lonLat = new OpenLayers.LonLat( lat, long )" << std::endl;
    oss << "    .transform(" << std::endl;
    oss << "               new OpenLayers.Projection(\"EPSG:4326\"), // transform from WGS 1984" << std::endl;
    oss << "               map.getProjectionObject() // to Spherical Mercator Projection" << std::endl;
    oss << "               );" << std::endl;
    oss << "    markers.addMarker(new OpenLayers.Marker(lonLat));" << std::endl;
    oss << "    return lonLat;" << std::endl;
    oss << "}" << std::endl;
    oss << "var markers = new OpenLayers.Layer.Markers( \"Markers\" );" << std::endl;
    oss << "map.addLayer(markers);" << std::endl;

    for(auto it = m_list.rbegin(); it != m_list.rend(); ++it){
        auto p = (*it);
        oss << "lonLat = addPoint(" << p->m_longitude << "," << p->m_latitude << ", markers)" << std::endl;
    }
    
    oss << "map.setCenter (lonLat, 18);" << std::endl;
    oss << "</script>" << std::endl;
    oss << "</body></html>" << std::endl;
    INFO(oss.str());
    std::ofstream infile("/Users/lemairec/fablab/agrigpspi/build/test.html");
    infile << oss.str();
}

void GpsFramework::readFile(){
    std::ifstream infile("/Users/lemairec/fablab/agrigpspi/gps_warmo.ubx");
    std::string line;
    while (std::getline(infile, line))
    {
        if(!line.empty() && line[0] == '$'){
            m_gpsModule.readFrame(line);
            QThread::msleep(10);
        } else if(line == "[savePointA]"){
            savePointA();
        } else if(line == "[savePointB]"){
            savePointB();
        }
        //INFO(line);
        
    }
    exportHtml();
}



void GpsFramework::main(){
    try {
        
        Serial serial(m_config.m_port,m_config.m_baudrate);
        
        //serial.writeString("Hello world\n");
        
        while(true){
            char c = serial.readChar();
            m_gpsModule.readChar(c);
        }
        
    } catch(boost::system::system_error& e)
    {
        cout<<"Error: "<<e.what()<<endl;
        INFO(m_config.m_port << " " << m_config.m_baudrate);
        return;
    }
    
    /*for(;;){
        //INFO("tutu");
        if(m_observer){
            m_observer->onNewPoint();
        }
        QThread::msleep(10);
    }*/
    
    
}

void GpsFramework::test(){
    m_gpsModule.readFrame("$GNGGA,110138.80,4902.71554,N,00324.04388,E,1,07,1.94,46.6,M,46.3,M,,*71");
    GGAFrame * f = m_list.front();
    INFO(f->m_latitude << " " << f->m_longitude);
    savePointA();
    GGAFrame * f2 = new GGAFrame();
    f2->m_latitude = 49.1453;
    f2->m_longitude = 3.50073;
    m_gpsModule.setXY(*f2);
    INFO(f2->m_x << " " << f2->m_y);
    onGGAFrame(*f2);
    INFO("res " << 13291);
    INFO(m_distance << " " << m_distance/13291);
    
    f2 = new GGAFrame();
    f2->m_latitude = 49.04;
    f2->m_longitude = 3.40;
    m_gpsModule.setXY(*f2);
    onGGAFrame(*f2);
    INFO("res " << 592);
    INFO(m_distance << " " << m_distance/592);
}

