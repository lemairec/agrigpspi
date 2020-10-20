#include "gps_framework.hpp"
#include "logging.hpp"
#include "config/config.hpp"
#include "environnement.hpp"

#include <QDateTime>
#include <math.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <sstream>
std::ofstream gpslogFile;
std::ofstream logFile;

bool rmc = true;

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
    
    std::string file2 = ProjectSourceBin + "/alog_framework.ubx";
    INFO(file2);
    logFile.open(file2, std::ios::out);
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
    m_fileModule.initOrLoad(m_config);
    
    m_distance_cap_vitesse = 3;
    m_lineAB.m_pointA.m_latitude = m_config.m_a_lat;
    m_lineAB.m_pointA.m_longitude = m_config.m_a_lon;
    m_lineAB.m_pointB.m_latitude = m_config.m_b_lat;
    m_lineAB.m_pointB.m_longitude = m_config.m_b_lon;
    gpslogFile << "[config]\n";
    setAB();
    m_reloadConfig = true;
    
    m_pilot_lookahead_d = m_config.m_pilot_lookahead_d;
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
    m_gpsModule.setXY(m_lineAB.m_pointA);
    m_gpsModule.setXY(m_lineAB.m_pointB);
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
    DEBUG("begin");
    m_lastGGAFrame = GGAFrame(f);
    
    if(!rmc){
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
            
            
            calculDeplacement();
            m_distance = distance(*frame);
            
            calculAngleCorrection();
            
            m_pilotModule.run(m_angle_correction, m_time_last_point, m_vitesse);
            
            //m_distanceAB = moyDistance(m_distanceAB);
            //m_deplacementAngle = moyDeplacement(m_deplacementAngle);
            
            calculSurface();
            
            calculDraw(frame);
        }
       
        if(m_observer){
            m_observer->onNewPoint();
        }
        setNewGpsTime();
    }
    //std::cout << "<trkpt lon=\""<< f.m_longitude << "\" lat=\"" << f.m_latitude << "\"><ele>51.0</ele><time>2010-12-26T17:07:40.421Z</time></trkpt>" << std::endl;
    DEBUG("end");
}

void GpsFramework::setNewGpsTime(){
    auto begin = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = begin - m_last_gps_received;

    double seconds = diff.count()*1000;
    m_last_gps_received = begin;
    
    m_gps_times.push_front(seconds);
    while(m_gps_times.size()>10){
        m_gps_times.pop_back();
    }
    
    double sum = 0;
    for(auto c : m_gps_times){
        sum += c;
    }
    double moy = sum/m_gps_times.size();
    
    
    double sum2 = 0;
    for(auto c : m_gps_times){
        sum2 += (moy-c)*(moy-c);
    }
    double error = std::sqrt(sum2/m_gps_times.size());
    
    m_gps_time_moy = moy;
    m_gps_time_et = error;
}

bool GpsFramework::isGpsConnected(){
    auto begin = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = begin - m_last_gps_received;

    if(diff.count() < 2.0){
        return true;
    } else {
        return false;
    }
}

void GpsFramework::setNewPilotTime(){
    auto begin = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = begin - m_last_pilot_received;

    double seconds = diff.count()*1000;
    m_last_pilot_received = begin;
    
    m_pilot_times.push_front(seconds);
    while(m_pilot_times.size()>10){
        m_pilot_times.pop_back();
    }
    
    double sum = 0;
    for(auto c : m_pilot_times){
        sum += c;
    }
    double moy = sum/m_pilot_times.size();
    
    
    double sum2 = 0;
    for(auto c : m_pilot_times){
        sum2 += (moy-c)*(moy-c);
    }
    double error = std::sqrt(sum2/m_pilot_times.size());
    
    m_pilot_time_moy = moy;
    m_pilot_time_et = error;
}

bool GpsFramework::isPilotConnected(){
    auto begin = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = begin - m_last_pilot_received;

    if(diff.count() < 2.0){
        return true;
    } else {
        return false;
    }
}



void GpsFramework::onRMCFrame(RMCFrame_ptr f){
    //return;
    DEBUG("begin");
    
    if(rmc){
        m_lastRMCFrame = f;
        if(true){
            if(m_gpsModule.m_latitudeRef == 0){
                setRef(f->m_latitude, f->m_longitude);
                return;
            }
            
            m_list.push_front(f);
            if(m_list.size()>100){
                m_list.pop_back();
            };
            
            
            calculDeplacement();
            m_distance = distance(*f);
            //m_deplacementAngle = f->m_cap_rad;
            m_vitesse = f->m_vitesse_kmh;
            
            
            calculAngleCorrection();
            m_pilotModule.run(m_angle_correction, m_time_last_point, m_vitesse);

            calculSurface();
            
            DEBUG("draw");
            calculDraw(f);
        }
       
        setNewGpsTime();
        DEBUG("end");
    }
    
    //std::cout << "<trkpt lon=\""<< f.m_longitude << "\" lat=\"" << f.m_latitude << "\"><ele>51.0</ele><time>2010-12-26T17:07:40.421Z</time></trkpt>" << std::endl;
}

void GpsFramework::onFrame(const std::string &frame){
    gpslogFile << frame << "\n";
}


double GpsFramework::distance(GpsPoint & gpsPoint){
    if(m_lineAB.m_pointA.m_x!=0 && m_lineAB.m_pointB.m_x!=0){
        //INFO("-ym "<< gpsPoint.m_y << " xm " << gpsPoint.m_x << " " << gpsPoint.m_y - m_a * gpsPoint.m_x - m_b);
        //INFO("-m_a " << m_a << " m_b " << m_b << " " << " m_c " << m_c << " m_sqrt_m_a_m_b " << m_sqrt_m_a_m_b);
        //INFO("-res1 " << (m_a * gpsPoint.m_x + m_b * gpsPoint.m_y + m_c));
        
        double dist = (m_lineAB.m_a * gpsPoint.m_x + m_lineAB.m_b * gpsPoint.m_y + m_lineAB.m_c)/m_lineAB.m_sqrt_m_a_m_b;
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
    }else if(m_lineAB.m_pointA.m_x!=0){
        //double dist = distanceBetween(m_pointA, gpsPoint);
        double dx = m_lineAB.m_pointA.m_x - gpsPoint.m_x;
        double dy = m_lineAB.m_pointA.m_y - gpsPoint.m_y;
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
        m_lineAB.m_pointA = *(*m_list.begin());
    }
    if(m_observer){
        m_observer->onNewPoint();
    }
    setRef(m_lineAB.m_pointA.m_latitude, m_lineAB.m_pointA.m_longitude);
    
    gpslogFile << "[savePointA]\n";
    INFO(m_lineAB.m_pointA.m_time << " " << m_lineAB.m_pointA.m_latitude << " " << m_lineAB.m_pointA.m_longitude);
    clearSurface();
}

void GpsFramework::savePointB(){
    if(!m_list.empty()){
        m_lineAB.m_pointB = *(*m_list.begin());
    }
    
    INFO(m_lineAB.m_pointB.m_time << " " << m_lineAB.m_pointB.m_latitude << " " << m_lineAB.m_pointB.m_longitude);
    if(m_lineAB.m_pointA.m_isOk!=0 && m_lineAB.m_pointB.m_isOk!=0){
        setAB();
    }
    if(m_observer){
        m_observer->onNewPoint();
    }
    gpslogFile << "[savePointB]\n";
}

void GpsFramework::setAB(){
    m_config.m_a_lat = m_lineAB.m_pointA.m_latitude;
    m_config.m_a_lon = m_lineAB.m_pointA.m_longitude;
    m_config.m_b_lat = m_lineAB.m_pointB.m_latitude;
    m_config.m_b_lon = m_lineAB.m_pointB.m_longitude;
    m_config.save();
    
    //m_pointA.m_x = 1; m_pointA.m_y = 1;
    setRef((m_lineAB.m_pointA.m_latitude + m_lineAB.m_pointB.m_latitude)/2, (m_lineAB.m_pointA.m_longitude + m_lineAB.m_pointB.m_longitude)/2);
    m_lineAB.m_ab_x = m_lineAB.m_pointB.m_x - m_lineAB.m_pointA.m_x;
    m_lineAB.m_ab_y = m_lineAB.m_pointB.m_y - m_lineAB.m_pointA.m_y;
    
    
    m_lineAB.m_a = -(m_lineAB.m_pointB.m_y - m_lineAB.m_pointA.m_y);
    m_lineAB.m_b = m_lineAB.m_pointB.m_x - m_lineAB.m_pointA.m_x;
    m_lineAB.m_c = -m_lineAB.m_a * m_lineAB.m_pointA.m_x - m_lineAB.m_b *  m_lineAB.m_pointA.m_y;
    m_lineAB.m_sqrt_m_a_m_b = sqrt(m_lineAB.m_a*m_lineAB.m_a + m_lineAB.m_b*m_lineAB.m_b);
    
    if(m_lineAB.m_ab_y != 0 && m_lineAB.m_ab_x != 0){
        m_lineAB.m_angleAB = atan(m_lineAB.m_ab_x/m_lineAB.m_ab_y);
    } else {
        m_lineAB.m_angleAB = 0;
    }
    INFO("yb  " << std::fixed << m_lineAB.m_pointB.m_y << " ya " << m_lineAB.m_pointA.m_y << " xb " << m_lineAB.m_pointB.m_x << " xa " << m_lineAB.m_pointA.m_x);
    INFO(m_lineAB.m_a << "*x + " << m_lineAB.m_b << "*y + " << m_lineAB.m_c << " = 0; " << m_lineAB.m_sqrt_m_a_m_b);
}

/*void GpsFramework::calculDeplacement(){
    if(m_list.size() > 3){
        GpsPoint_ptr point1 = m_list.front();
        GpsPoint_ptr point2 = NULL;
        GpsPoint_ptr point3 = NULL;
        int i = 0;
        for(auto point : m_list){
            if(point3 == NULL){
                point2 = point;
                
                double x = point1->m_x - point2->m_x;
                double y = point1->m_y - point2->m_y;
                
                double d = x*x+y*y;
                if(d>(m_distance_cap_vitesse*m_distance_cap_vitesse)){
                    point3 = point;
                }
            } else {
                point3 = point;
                
                double x = point2->m_x - point3->m_x;
                double y = point2->m_y - point3->m_y;
                
                double d = x*x+y*y;
                if(d>(m_distance_cap_vitesse*m_distance_cap_vitesse)){
                    break;
                }
            }
            //point2 = NULL;
            ++i;
        }
        if(point2!=NULL){
            double x = point1->m_x - point2->m_x;
            double y = point1->m_y - point2->m_y;
            
            m_deplacementX = x;
            m_deplacementY = y;
            
            if(m_deplacementY != 0){
                double temp = atan(m_deplacementX/m_deplacementY);
                if(m_deplacementY>0){
                    m_deplacementAngle = temp;
                } else {
                    m_deplacementAngle = temp+3.14;
                }
                //int perc = temp/m_deplacementAngle*100;
                //INFO(perc << " " << temp/3.14*180 << " " << m_deplacementAngle/3.14*180);
            }
            
            if(m_ab_x != 0 || m_ab_y != 0){
                double det = m_a*m_deplacementY - m_b*m_deplacementX;
                //m_deplacementAngle = m_deplacementAngle+3.14;
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
        
        
        int i2 = 0;
        GpsPoint_ptr poin1 = point1 ,poin2 = point2,poin3 = point3;
        
        if(point3){
        
            double num = (poin1->m_x - poin3->m_x)*(poin2->m_x - poin3->m_x) + (poin1->m_y - poin3->m_y)*(poin2->m_y - poin3->m_y);
            
            double den1 = std::sqrt((poin1->m_x - poin3->m_x)*(poin1->m_x - poin3->m_x)+(poin1->m_y - poin3->m_y)*(poin1->m_y - poin3->m_y));
            double den2 = std::sqrt((poin2->m_x - poin3->m_x)*(poin2->m_x - poin3->m_x)+(poin2->m_y - poin3->m_y)*(poin2->m_y - poin3->m_y));
            
            double cosa = num/(den1*den2);
            double a = acos(cosa);
            double a2 = a/3.14*180;
            
            //INFO(a2);
        }
        //INFO(deplacementTime << " " << vitesse);
    }
    //calculContourExterieur();
}*/

void GpsFramework::calculDeplacement(){
    if(m_list.size() > 3){
        GpsPoint_ptr point1 = m_list.front();
              
        
        
        GpsPoint_ptr point2 = NULL;
        int i = 0;
        for(auto point : m_list){
            point2 = point;
            
            double x = point1->m_x - point2->m_x;
            double y = point1->m_y - point2->m_y;
            
            double d = x*x+y*y;
            if(d>(m_distance_cap_vitesse*m_distance_cap_vitesse)){
                break;
            }
            
            //point2 = NULL;
            ++i;
        }
        if(point2!=NULL){
            double x = point1->m_x - point2->m_x;
            double y = point1->m_y - point2->m_y;
            
            m_deplacementX = x;
            m_deplacementY = y;
            
            if(m_deplacementY != 0){
                double temp = atan(m_deplacementX/m_deplacementY);
                if(m_deplacementY>0){
                    m_deplacementAngle = temp;
                } else {
                    m_deplacementAngle = temp+3.14;
                }
                //int perc = temp/m_deplacementAngle*100;
                //INFO(perc << " " << temp/3.14*180 << " " << m_deplacementAngle/3.14*180);
            }
            
            if(m_lineAB.m_ab_x != 0 || m_lineAB.m_ab_y != 0){
                double det = m_lineAB.m_a*m_deplacementY - m_lineAB.m_b*m_deplacementX;
                //m_deplacementAngle = m_deplacementAngle+3.14;
                m_sensAB = (det < 0);
            }
            
            
            m_distance_last_point = sqrt(m_deplacementX*m_deplacementX + m_deplacementY*m_deplacementY);
            m_time_last_point = point1->m_timeHour - point2->m_timeHour;
            
            m_tracteur.m_x_essieu_avant = point1->m_x + sin(m_deplacementAngle)*m_tracteur.m_antenne_essieu_avant;
            m_tracteur.m_y_essieu_avant = point1->m_y + cos(m_deplacementAngle)*m_tracteur.m_antenne_essieu_avant;
            
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

void GpsFramework::calculDraw(GpsPoint_ptr p){
    if(m_listSurfaceToDraw.size()==0){
        SurfaceToDraw_ptr s(new SurfaceToDraw());
        m_listSurfaceToDraw.push_front(s);
        s->m_points.push_front(p);
        s->m_points.push_front(p);
        s->m_lastPoint = p;
        s->m_lastPointOk = p;
        return;
    }
    if(m_pauseDraw){
        return;
    }
    
    auto s = m_listSurfaceToDraw.front();
    
    if(s->m_lastPoint != s->m_lastPointOk){
        s->m_points.pop_front();
    }
    s->m_lastPoint = p;
    if(s->m_lastPointOk == NULL){
        s->m_lastPointOk = s->m_lastPoint;
    };
    double x = s->m_lastPointOk->m_x - s->m_lastPoint->m_x;
    double y = s->m_lastPointOk->m_y - s->m_lastPoint->m_y;
    double dist = x*x + y*y;
    if(dist > m_distance_cap_vitesse*m_distance_cap_vitesse){
        s->m_lastPointOk = p;
    }
    s->m_points.push_front(p);
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
    std::ifstream infile(m_config.m_file_gps);
    if(!infile.is_open()){
        INFO("can not open " << m_config.m_file_gps);
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
        m_config.m_input_gps = "none";
    } else {
        m_list.clear();
        m_listSurfaceToDraw.clear();
        clearSurface();
    }
    //exportHtml();
}


void GpsFramework::test(){
    m_gpsModule.readFrame("$GNGGA,110138.80,4902.71554,N,00324.04388,E,1,07,1.94,46.6,M,46.3,M,,*71");
    GpsPoint_ptr f = m_list.front();
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
    m_surface_h = 0;
    //INFO("****");
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
    //naif
    //m_angle_correction = atan(m_distanceAB/m_algo_lookahead_d);
   
    //follow karott
    double angleABDeplacement = m_lineAB.m_angleAB - m_deplacementAngle;
    if(angleABDeplacement>3.14/2){
        angleABDeplacement = angleABDeplacement-3.14;
    }
    if(angleABDeplacement < -3.14/2){
        angleABDeplacement = angleABDeplacement+3.14;
    }
    //INFO(angleABDeplacement/3.14*180);
    
    m_angle_correction = atan(m_distanceAB/m_pilot_lookahead_d)+angleABDeplacement;
    //INFO(m_angle_correction);
    //m_angle_correction = ;
    
    double angle_max = 0.5;
    if(m_angle_correction < -angle_max){
        m_angle_correction = -angle_max;
    }
    if(m_angle_correction > angle_max){
        m_angle_correction = angle_max;
    }
    //m_angle_correction = -3.14/6;
    
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
        std::vector<GpsPoint_ptr> points;
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
        m_surface_exterieur_h = m_surface_exterieur/(m_list.front()->m_timeHour - m_lineAB.m_pointA.m_timeHour);
    }
}


void GpsFramework::setVolantEngaged(bool value){
    m_pilotModule.clear();
    if(value){
        gpslogFile << "[engage]\n";
        m_pilotModule.engage();
        m_pauseDraw = false;
    } else {
        gpslogFile << "[desengage]\n";
        m_pilotModule.desengage();
        m_pauseDraw = true;
    }
}

bool GpsFramework::getVolantEngaged(){
    return m_pilotModule.m_engaged;
}

void GpsFramework::addLog(const std::string &s, bool time2){
    if(time2){
        time_t     now = time(0);
        struct tm  tstruct;
        char       buf[80];
        tstruct = *localtime(&now);
        // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
        // for more information about date/time format
        strftime(buf, sizeof(buf), "%M:%S ", &tstruct);

        std::ostringstream strs3;
        strs3 << buf << s;
        
        m_listLog.push_front(strs3.str());
        logFile << '\n' <<  strs3.str();
    } else {
        std::ostringstream strs3;
        strs3 << "  " << s;
        m_listLog.push_front(strs3.str());
        logFile << '\n' << strs3.str();
    }
    while(m_listLog.size()>20){
        m_listLog.pop_back();
    }
}


