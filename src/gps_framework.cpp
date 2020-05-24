#include "gps_framework.hpp"
#include "logging.hpp"
#include "config/config.hpp"
#include "environnement.hpp"

#include <QDateTime>
#include <math.h>
#include <iostream>
#include <fstream>
#include <sstream>
std::ofstream gpslogFile;


GpsFramework::GpsFramework(){
    QDateTime date = QDateTime::currentDateTime();
    QString s = date.toString("yyyyMMdd_hhmm");
    
    int y = date.date().year();
    if(y>(1011+1011)){
    //    exit(0);
    }
    
    std::string file = ProjectSourceBin + "/gps_" + s.toUtf8().constData() + ".ubx";
    INFO(file);
    gpslogFile.open(file, std::ios::out);
    m_config.load();
}

void GpsFramework::addError(std::string s){
    WARN(s);
    std::ostringstream oss;
    oss << m_messages_errors << "\n" << s;
    m_messages_errors = oss.str();
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
    m_serialModule.initOrLoad(m_config);
     
    m_pointA.m_latitude = m_config.m_a_lat;
    m_pointA.m_longitude = m_config.m_a_lon;
    m_pointA.m_nbrSat = 100;
    m_pointA.m_fix = 100;
    m_pointB.m_latitude = m_config.m_b_lat;
    m_pointB.m_longitude = m_config.m_b_lon;
    m_pointB.m_nbrSat = 100;
    m_pointB.m_fix = 100;
    setAB();
    m_reloadConfig = true;
    
    m_algo = m_config.m_algo;
    m_algofk_lookahead_d = m_config.m_algofk_lookahead_d;
    m_algo_naif_k = m_config.m_algo_naif_k;
    
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
    for(auto l: m_listSurfaceToDraw){
        m_gpsModule.setXY(*l->m_lastPoint);
        m_gpsModule.setXY(*l->m_lastPointOk);
        for(auto l2: l->m_points){
           m_gpsModule.setXY(*l2);
       }
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
        if(m_list.size()>100){
            m_list.pop_back();
        };
        
        calculAngleCorrection();
        if(m_volantEngaged){
            m_pilotModule.run(m_angle_correction, m_time_last_point);
        }
        calculDeplacement();
        
        calculSurface();
        m_distance = distance(*frame);
        
        calculDraw(frame);
    }
   
    if(m_observer){
        m_observer->onNewPoint();
    }
    
    //std::cout << "<trkpt lon=\""<< f.m_longitude << "\" lat=\"" << f.m_latitude << "\"><ele>51.0</ele><time>2010-12-26T17:07:40.421Z</time></trkpt>" << std::endl;
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
        m_distanceAB = dist;
        if(dist < 0.0){
            dist = -dist;
            m_ledAB = round(dist/coeff) + 1;
        } else {
            m_ledAB = -round(dist/coeff) - 1;
        }
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
        m_angleAB = atan(m_ab_x/m_ab_y);
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
            
            double x = point1->m_x - point2->m_x;
            double y = point1->m_y - point2->m_y;
            
            double d = x*x+y*y;
            if(d>(3*3)){
                break;
            }
            point2 = NULL;
            ++i;
        }
        if(point2!=NULL){
            double x = point1->m_x - point2->m_x;
            double y = point1->m_y - point2->m_y;
            
            m_deplacementX = x;
            m_deplacementY = y;
            
            if(m_deplacementX != 0){
                m_deplacementAngle = atan(m_deplacementY/m_deplacementX);
            }
            
            if(m_ab_x != 0 || m_ab_y != 0){
                double det = m_a*m_deplacementY - m_b*m_deplacementX;
                m_sensAB = (det < 0);
            }
            
            m_distance_last_point = sqrt(m_deplacementX*m_deplacementX + m_deplacementY*m_deplacementY);
            m_time_last_point = point1->m_timeHour - point2->m_timeHour;
            if(m_time_last_point > 0){
                m_vitesse = m_distance_last_point/1000.0/m_time_last_point;
                if(m_vitesse >50){
                    INFO("erreur");
                }
            }
        }
        //INFO(deplacementTime << " " << vitesse);
    }
    //calculContourExterieur();
}

void GpsFramework::changeDraw(){
    if(m_pauseDraw == false){
        m_pauseDraw = true;
    } else {
        SurfaceToDraw_ptr p(new SurfaceToDraw());
        m_listSurfaceToDraw.push_front(p);
        m_pauseDraw = false;
    }
}

void GpsFramework::calculDraw(GGAFrame_ptr p){
    if(m_listSurfaceToDraw.size()==0){
        SurfaceToDraw_ptr p(new SurfaceToDraw());
        m_listSurfaceToDraw.push_front(p);
    }
    if(m_pauseDraw){
        return;
    }
    
    auto s = m_listSurfaceToDraw.front();
    
    s->m_lastPoint = p;
    if(s->m_lastPointOk == NULL){
        s->m_lastPointOk = p;
    }
    double x = s->m_lastPointOk->m_x - s->m_lastPoint->m_x;
    double y = s->m_lastPointOk->m_y - s->m_lastPoint->m_y;
    double dist = x*x + y*y;
    if(dist > 1){
        s->m_lastPointOk = p;
        s->m_points.push_front(p);
    }
}

#include <iostream>

using namespace std;


/*void GpsFramework::exportHtml(){
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
}*/

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
        m_listSurfaceToDraw.clear();
        clearSurface();
    }
    //exportHtml();
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
    double l = m_config.m_largeur;
    m_surface = 0;
    for(auto s : m_listSurfaceToDraw){
        if(s->m_points.size()>0){
            auto last_frame = s->m_lastPoint;
            double x1 = last_frame->m_x;
            double y1 = last_frame->m_y;
            for(auto it = s->m_points.begin(); it != s->m_points.end(); ++it){
                auto frame = (*it);
                double x0 = frame->m_x;
                double y0 = frame->m_y;
                double dist = (x0-x1)*(x0-x1) + (y0-y1)*(y0-y1);
                
                double surface = std::sqrt(dist)*m_config.m_largeur/10000.0;
                m_surface += surface;
                
                x1 = x0;
                y1 = y0;
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



// pure pousuite
void GpsFramework::calculAngleCorrection(){
    if(m_algo == ALGO_NAIF){
        m_angle_correction = -m_distanceAB/m_config.m_largeur*m_algo_naif_k;
    } else {
        m_angle_correction = atan(-m_distanceAB/m_algofk_lookahead_d)-(m_angleAB-atan(m_deplacementX/m_deplacementY));
        //m_angle_correction = ;
    }
    //m_angle_correction = -3.14/6;
    
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


void GpsFramework::setVolantEngaged(bool value){
    m_volantEngaged = value;
    m_pilotModule.clear();
    if(value){
        m_pilotModule.engage();
    } else {
        m_pilotModule.desengage();
    }
}

bool GpsFramework::getVolantEngaged(){
    return m_volantEngaged;
}
