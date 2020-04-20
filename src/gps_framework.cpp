#include "gps_framework.hpp"
#include "logging.hpp"
#include "config/config.hpp"
#include "environnement.hpp"

#include <QDateTime>
#include <math.h>
#include <iostream>
#include <fstream>
std::ofstream gpslogFile;


GpsFramework::GpsFramework(){
    QDateTime date = QDateTime::currentDateTime();
    QString s = date.toString("yyyyMMdd_hhmm");
    
    int y = date.date().year();
    if(y>2020){
        exit(0);
    }
    
    std::string file = ProjectSourceBin + "/gps_" + s.toUtf8().constData() + ".ubx";
    INFO(file);
    gpslogFile.open(file, std::ios::out);
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
    
    m_pilotModule.initOrLoadConfig(m_config);
     
    m_pointA.m_latitude = m_config.m_a_lat;
    m_pointA.m_longitude = m_config.m_a_lon;
    m_pointA.m_nbrSat = 100;
    m_pointB.m_latitude = m_config.m_b_lat;
    m_pointB.m_longitude = m_config.m_b_lon;
    m_pointB.m_nbrSat = 100;
    setAB();
    m_reloadConfig = true;
}

GpsFramework & GpsFramework::Instance(){
    static GpsFramework gf;
    return gf;
}

void GpsFramework::setRef(double latitude, double longitude){
    m_gpsModule.setRef(latitude, longitude);
    for(auto l: m_list){
        m_gpsModule.setXY(*l);
    }
    if(m_pointA.isOk()){
        m_gpsModule.setXY(m_pointA);
    }
    if(m_pointB.isOk()){
        m_gpsModule.setXY(m_pointB);
    }
}

void GpsFramework::onGGAFrame(GGAFrame & f){
    m_lastGGAFrame = GGAFrame(f);
    if(f.isOk()){
        if(m_gpsModule.m_latitudeRef == 0){
            setRef(f.m_latitude, f.m_longitude);
            return;
        }
        GGAFrame_ptr frame = GGAFrame_ptr(new GGAFrame(f));
        m_list.push_front(frame);
        
        calculDeplacement();
        
        calculSurface();
        m_distance = distance(*frame);
    }
   
    if(m_observer){
        m_observer->onNewPoint();
    }
}

void GpsFramework::onGGAFrame(const std::string &frame){
    gpslogFile << frame << "\n";
}


double GpsFramework::distance(GpsPoint & gpsPoint){
    if(m_pointA.m_x!=0 && m_pointB.m_x!=0){
        //INFO("-ym "<< gpsPoint.m_y << " xm " << gpsPoint.m_x << " " << gpsPoint.m_y - m_a * gpsPoint.m_x - m_b);
        //INFO("-m_a " << m_a << " m_b " << m_b << " " << " m_c " << m_c << " m_sqrt_m_a_m_b " << m_sqrt_m_a_m_b);
        //INFO("-res1 " << (m_a * gpsPoint.m_x + m_b * gpsPoint.m_y + m_c));
        
        double dist = (m_a * gpsPoint.m_x + m_b * gpsPoint.m_y + m_c)/m_sqrt_m_a_m_b;
        //INFO(dist);
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
        m_distanceAB = dist;
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
    setRef(m_pointA.m_latitude, m_pointA.m_longitude);
    
    gpslogFile << "[savePointA]\n";
    INFO(m_pointA.m_time << " " << m_pointA.m_latitude << " " << m_pointA.m_longitude);
    clearSurface();
}

void GpsFramework::savePointB(){
    if(!m_list.empty()){
        m_pointB = *(*m_list.begin());
    }
    
    INFO(m_pointB.m_time << " " << m_pointB.m_latitude << " " << m_pointB.m_longitude);
    if(m_pointA.m_time!=0 && m_pointB.m_time!=0){
        setAB();
    }
    if(m_observer){
        m_observer->onNewPoint();
    }
    gpslogFile << "[savePointB]\n";
}

void GpsFramework::setAB(){
    m_config.m_a_lat = m_pointA.m_latitude;
    m_config.m_a_lon = m_pointA.m_longitude;
    m_config.m_b_lat = m_pointB.m_latitude;
    m_config.m_b_lon = m_pointB.m_longitude;
    m_config.save();
    
    //m_pointA.m_x = 1; m_pointA.m_y = 1;
    setRef((m_pointA.m_latitude + m_pointB.m_latitude)/2, (m_pointA.m_longitude + m_pointB.m_longitude)/2);
    m_ab_x = m_pointB.m_x - m_pointA.m_x;
    m_ab_y = m_pointB.m_y - m_pointA.m_y;
    
    
    m_a = -(m_pointB.m_y - m_pointA.m_y);
    m_b = m_pointB.m_x - m_pointA.m_x;
    m_c = -m_a * m_pointA.m_x - m_b *  m_pointA.m_y;
    m_sqrt_m_a_m_b = sqrt(m_a*m_a + m_b*m_b);
    
    if(m_ab_y != 0 && m_ab_x != 0){
        m_angleAB = atan(m_ab_y/m_ab_x);
    } else {
        m_angleAB = 0;
    }
    INFO("yb  " << std::fixed << m_pointB.m_y << " ya " << m_pointA.m_y << " xb " << m_pointB.m_x << " xa " << m_pointA.m_x);
    INFO(m_a << "*x + " << m_b << "*y + " << m_c << " = 0; " << m_sqrt_m_a_m_b);
}

void GpsFramework::calculDeplacement(){
    if(m_list.size() > 2){
        GGAFrame_ptr point1 = m_list.front();
        GGAFrame_ptr point2 = NULL;
        int i = 0;
        for(auto point : m_list){
            point2 = point;
            i++;
            if(i > 2){
                break;
            }
        }
        m_deplacementX = point1->m_x - point2->m_x;
        m_deplacementY = point1->m_y - point2->m_y;
        if(m_deplacementX != 0){
            m_deplacementAngle = atan(m_deplacementY/m_deplacementX);
        }
        
        if(m_ab_x != 0 || m_ab_y != 0){
            double det = m_a*m_deplacementY - m_b*m_deplacementX;
            m_sensAB = (det < 0);
        }
        
        m_distance_last_point = sqrt(m_deplacementX*m_deplacementX + m_deplacementY*m_deplacementY);
        m_time_last_point = point1->m_timeHour - point2->m_timeHour;
        m_vitesse = m_distance_last_point/1000.0/m_time_last_point;
        //INFO(deplacementTime << " " << vitesse);
    }
    //calculContourExterieur();
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

#define SLEEP_TIME 50
void GpsFramework::readFile(){
    std::ifstream infile(m_config.m_file);
    if(!infile.is_open()){
        INFO("can not open " << m_config.m_file);
        return;
    }
    std::string line;
    
    std::chrono::time_point<std::chrono::system_clock> start_time = std::chrono::system_clock::now();
    while (std::getline(infile, line) && !m_reloadConfig)
    {
        if(!line.empty() && line[0] == '$'){
            m_gpsModule.readFrame(line);
            QThread::msleep(SLEEP_TIME);
        }else if(!line.empty() && line[0] == 'G'){
            line = '$'+line;
            m_gpsModule.readFrame(line);
            QThread::msleep(SLEEP_TIME);
        } else if(line == "[savePointA]"){
            savePointA();
        } else if(line == "[savePointB]"){
            savePointB();
        }
        //INFO(line);
        
    }
    std::chrono::time_point<std::chrono::system_clock> end_time = std::chrono::system_clock::now();
    INFO("Wall time passed: " << chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count());
    if(!m_reloadConfig){
        m_config.m_input = "none";
    } else {
        m_list.clear();
        clearSurface();
    }
    //exportHtml();
}



void GpsFramework::main(){
    for(;;){
        m_reloadConfig = false;
        if(m_observer){
            m_observer->onNewPoint();
        }
        if(m_config.m_input == "file"){
            readFile();
        } else if (m_config.m_input == "none"){
            
        } else {
            try {
                
                Serial serial(m_config.m_input,m_config.m_baudrate);
                
                //serial.writeString("Hello world\n");
                
                while(!m_reloadConfig){
                    char c = serial.readChar();
                    m_gpsModule.readChar(c);
                }
                
            } catch(boost::system::system_error& e)
            {
                cout<<"Error: "<<e.what()<<endl;
                INFO(m_config.m_input << " " << m_config.m_baudrate);
                return;
            }
        }
        QThread::msleep(1000);
    }
    
    
}


void GpsFramework::test(){
    m_gpsModule.readFrame("$GNGGA,110138.80,4902.71554,N,00324.04388,E,1,07,1.94,46.6,M,46.3,M,,*71");
    GGAFrame_ptr f = m_list.front();
    INFO(f->m_latitude << " " << f->m_longitude);
    savePointA();
    GGAFrame f2;
    f2.m_latitude = 49.1453;
    f2.m_longitude = 3.50073;
    m_gpsModule.setXY(f2);
    INFO(f2.m_x << " " << f2.m_y);
    onGGAFrame(f2);
    INFO("res " << 13291);
    INFO(m_distance << " " << m_distance/13291 <<  " %");
    
    f2.m_latitude = 49.1453;
    f2.m_longitude = 3.40;
    m_gpsModule.setXY(f2);
    INFO(f2.m_x << " " << f2.m_y);
    onGGAFrame(f2);
    INFO("res " << 11124);
    INFO(m_distance << " " << m_distance/11124 <<  " %");
    
    f2.m_latitude = 49.04;
    f2.m_longitude = 3.50073;
    m_gpsModule.setXY(f2);
    INFO(f2.m_x << " " << f2.m_y);
    onGGAFrame(f2);
    INFO("res " << 7333);
    INFO(m_distance << " " << m_distance/7333 <<  " %");
    
    f2.m_latitude = 49.04;
    f2.m_longitude = 3.40;
    m_gpsModule.setXY(f2);
    onGGAFrame(f2);
    INFO("res " << 592);
    INFO(m_distance << " " << m_distance/592<<  " %");
}

//surface
void GpsFramework::clearSurface(){
    m_surface = 0;
}

void GpsFramework::calculSurface(){
    if(m_distance_last_point <30){
        double diff_angle = m_angleAB - m_deplacementAngle;
        if(std::abs(diff_angle) < 0.1){
            double surface = m_distance_last_point*m_config.m_largeur/10000.0;
            m_surface += surface/2;
            m_surface_h = surface/m_time_last_point;
            
        }
        if(m_list.size()>2){
            //INFO(m_list.front()->m_timeHour<< " " << m_pointA.m_timeHour);
            //INFO(m_list.front()->m_timeHour - m_pointA.m_timeHour);
        }
        
    }
}



// pure pousuite
void GpsFramework::calculPurePoursuite(){
    
}
int orientation(GGAFrame & p, GGAFrame & q, GGAFrame & r)
{
    double val = (q.m_y - p.m_y) * (r.m_x - q.m_x) -
    (q.m_x - p.m_x) * (r.m_y - q.m_y);
    
    if (val == 0) return 0;  // colinear
    return (val > 0)? 1: 2; // clock or counterclock wise
}

double cp(GGAFrame & a, GGAFrame &  b){ //returns cross product
    return a.m_x*b.m_y-a.m_y*b.m_x;
}

double polygonArea(std::vector<GGAFrame_ptr> tab)
{
    double area = 0;
    int n = tab.size();
    double sum=0.0;
    for(int i=0; i<n; i++){
        sum+=cp(*tab[i], *tab[(i+1)%n]); //%n is for last triangle
    }
    return abs(sum)/2.0;
}


void GpsFramework::calculContourExterieur(){
    if(m_list.size()>10){
        m_contour.clear();
        // Find the leftmost point
        std::vector<GGAFrame_ptr> points;
        for(auto p: m_list){
            points.push_back(p);
        }
        
        int l = 0;
        int n = points.size();
        for (int i = 1; i < n; i++)
            if (points[i]->m_x < points[l]->m_x)
                l = i;
        
        // Start from leftmost point, keep moving counterclockwise
        // until reach the start point again.  This loop runs O(h)
        // times where h is number of points in result or output.
        int p = l, q;
        do
        {
            // Add current point to result
            m_contour.push_back(points[p]);
            
            // Search for a point 'q' such that orientation(p, x,
            // q) is counterclockwise for all points 'x'. The idea
            // is to keep track of last visited most counterclock-
            // wise point in q. If any point 'i' is more counterclock-
            // wise than q, then update q.
            q = (p+1)%n;
            for (int i = 0; i < n; i++)
            {
                // If i is more counterclockwise than current q, then
                // update q
                if (orientation(*points[p], *points[i], *points[q]) == 2)
                    q = i;
            }
            
            // Now q is the most counterclockwise with respect to p
            // Set p as q for next iteration, so that q is added to
            // result 'hull'
            p = q;
            
        } while (p != l);  // Whil
        m_contour.push_back(m_contour[0]);
        m_surface_exterieur = polygonArea(m_contour)/10000.0;
        m_surface_exterieur_h = m_surface_exterieur/(m_list.front()->m_timeHour - m_pointA.m_timeHour);
    }
}
