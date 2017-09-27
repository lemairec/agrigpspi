#include "gps_module.hpp"
#include "logging.hpp"
#include "environnement.hpp"

#include "gps_framework.hpp"

#include <math.h>

GpsModule::GpsModule(){
    init();
}

void GpsModule::init(){
    resetBuffer();
}

void GpsModule::onGGAFrame(GGAFrame * ggaFrame){
    
}

void GpsModule::onGGAFrame(const std::string & s){
    GpsFramework::Instance().onGGAFrame(s);
    //INFO(s);
}

void GpsModule::readFrame(const std::string & frame){
    for(char c : frame){
        readChar(c);
    }
    readChar('\n');
}

void GpsModule::readChar(char c){
    std::cout << c;
    if(c == '$'){
        //INFO("readChar");
    } else if(c == '\n'){
        parseBuffer();
        resetBuffer();
    } else {
        this->m_buffer[m_bufferIndLast] = c;
        m_bufferIndLast++;
    }
}


/**
 * Parsing
 **/

//$GPGGA,114608.00,4905.46094,N,00332.09303,E,2,07,1.46,87.8,M,46.3,M,,0000*6B



void GpsModule::resetBuffer(){
    m_bufferIndLast = 0;
    //for(size_t i = 0; i < 200; ++i){
    //  m_buffer[i] = ' ';
    //}
}


void GpsModule::error(){
    WARN("error");
}

void GpsModule::debug(){
    /*Serial.print(m_tempInd);
     Serial.print(" ");
     Serial.print(m_buffer[m_tempInd-1]);
     Serial.print(m_buffer[m_tempInd]);
     Serial.print(m_buffer[m_tempInd+1]);
     Serial.print("\n");*/
}



void GpsModule::parseBuffer(){
    m_tempInd = 0;
#ifdef GPS_DEBUG
    printBuffer();
#endif
    //return true;
    if(m_buffer[0] == 'G'){
        if(m_buffer[1] == 'N' || m_buffer[1] == 'P'){
            if(m_buffer[2] == 'G' && m_buffer[3] == 'G' && m_buffer[4] == 'A'){
                return parseGGA();
            }
        }
    } else if (m_buffer[0] == 'S' && m_buffer[1] == 'P'){
        if(m_buffer[2] == 'A'){
            GpsFramework::Instance().savePointA();
        } else if(m_buffer[2] == 'B'){
            GpsFramework::Instance().savePointB();
        }
    }
}

void GpsModule::parseGGA(){
    std::string s = "";
    for(size_t i =0; i < m_bufferIndLast; ++i){
        s += m_buffer[i];
    }
    onGGAFrame(s);
    
    readUntilCommat();
    m_lastGGAEvent.m_time = readDouble();
    m_lastGGAEvent.m_latitude = readDeg();
    readUntilCommat();
    m_lastGGAEvent.m_longitude = readDeg();
    readUntilCommat();
    m_lastGGAEvent.m_fix = readInt();
    m_lastGGAEvent.m_nbrSat = readInt();
    m_lastGGAEvent.m_precision = readDouble();
    m_lastGGAEvent.m_altitude = readDouble();
    
    m_lastGGAEvent.m_timeHour = getTimeHour(m_lastGGAEvent.m_time);
    setXY(m_lastGGAEvent);
    INFO("******** GGA");
    GpsFramework::Instance().onGGAFrame(m_lastGGAEvent);
}

bool GpsModule::isEqual(const char * c, size_t size){
    size_t j = m_tempInd;
    for(size_t i = 0; i < size; ++i){
        if(c[i] == m_buffer[j]){
            ++j;
        } else {
            //  INFO(c[i] << " " << m_buffer[j] << " " << i);
            return false;
        }
    }
    return true;
}

void GpsModule::readUntilCommat(){
    while(m_tempInd < m_bufferIndLast){
        if(m_buffer[m_tempInd] == ','){
            ++m_tempInd;
            return;
        }
        ++m_tempInd;
    }
    error();
}

int GpsModule::getOneInt(){
    char c = m_buffer[m_tempInd];
    m_tempInd++;
    return getIntWithChar(c);
}

int GpsModule::getIntWithChar(char c){
    if(c =='0'){
        return 0;
    } else if(c =='1'){
        return 1;
    } else if(c =='2'){
        return 2;
    } else if(c =='3'){
        return 3;
    } else if(c =='4'){
        return 4;
    } else if(c =='5'){
        return 5;
    } else if(c =='6'){
        return 6;
    } else if(c =='7'){
        return 7;
    } else if(c =='8'){
        return 8;
    } else if(c =='9'){
        return 9;
    } else {
        error();
        return 0;
    }
}

int GpsModule::readInt(){
    int res = 0;
    while(m_tempInd < m_bufferIndLast){
        char c = m_buffer[m_tempInd];
        if(c == ','){
            ++m_tempInd;
            return res;
        } else {
            res = res*10 + getIntWithChar(c);
            ++m_tempInd;
        }
    }
    return 0;
}

double GpsModule::readDouble(){
    double res = 0;
    double virgule_part = 1;
    bool virgule = false;
    while(m_tempInd < m_bufferIndLast){
        char c = m_buffer[m_tempInd];
        int number = 0;
        if(c == ','){
            ++m_tempInd;
            return res;
        } else if(c =='.'){
            virgule = true;
        } else {
            number = getIntWithChar(c);
        }
        if(!virgule){
            res = res * 10 + number;
        } else {
            res = res + number * virgule_part;
            virgule_part = virgule_part * 0.1;
        }
        ++m_tempInd;
    }
    error();
    return 0.0;
}

double GpsModule::readDeg()
{
    double d = readDouble();
    int h = d/100;
    double minu = (d-(h*100));
    return h + minu/60.0;
}


double GpsModule::getTimeHour(double d)
{
    int h = d/10000;
    int minu = (d/100-(h*100));
    double s = d - h*10000.0 - minu*100.0;
    double res = h + minu/60.0 + s/3600.0;
    //INFO(d << " " << h << ":" << minu << ":" << s << " " << res);
    return res;
}

#define DEG2RAD(a)   ((a) / (180.0 / M_PI))
#define RAD2DEG(a)   ((a) * (180.0 / M_PI))

/**
 * TRANSFORME LAMBERT
 **/


//Algo 01
double __YGLatitudeISOFromLatitude(double lat, double e)
{
    return log(tan(M_PI_4+lat/2)*pow((1-e*sin(lat))/(1+e*sin(lat)),e/2));
}

double __YGLatitudeISO(double lat, double e)
{
    return log(tan(M_PI_4 + lat/2)*pow((1-e*sin(lat))/(1+e*sin(lat)),e/2));
}

void __YGCoordinatesTransform(GpsPoint & gpsPoint, double e, double n, double c, double lambda_c, double x_s, double y_s)
{
    double lon = DEG2RAD(gpsPoint.m_longitude);
    double lat = DEG2RAD(gpsPoint.m_latitude);
    double latiso = __YGLatitudeISO(lat,e);
    gpsPoint.m_x = x_s + c*exp(-n*latiso)*sin(n*(lon-lambda_c));
    gpsPoint.m_y = y_s - c*exp(-n*latiso)*cos(n*(lon-lambda_c));
    INFO(std::fixed << c*exp(-n*latiso) <<"  " << cos(n*(lon-lambda_c)) << "  " << gpsPoint.m_y);
}

static double lambert_n[6] = {0.7604059656, 0.7289686274, 0.6959127966, 0.6712679322, 0.7289686274, 0.7256077650};
static double lambert_c[6] = {11603796.98, 11745793.39, 11947992.52, 12136281.99, 11745793.39, 11754255.426};
static double lambert_xs[6]= {600000.0, 600000.0, 600000.0, 234.358, 600000.0, 700000.0};
static double lambert_ys[6]= {5657616.674, 6199695.768, 6791905.085, 7239161.542, 8199695.768, 12655612.050};
#define E_CLARK_IGN 0.08248325676
#define LON_MERID_GREENWICH 0.04079234433

#define LAMBERT_I 0
#define LAMBERT_II 1
#define LAMBERT_III 2
#define LAMBERT_IV 3
#define LAMBERT_II_E 4
#define LAMBERT_93 5
#define LAMBERT LAMBERT_I


/**
 * Cordonnees spheriques
 *
 * http://geodesie.ign.fr/contenu/fichiers/documentation/pedagogiques/TransformationsCoordonneesGeodesiques.pdf
 **/

double a = 6378249.2;
double b = 6356515;
double e_2 = (a*a-b*b)/(a*a);

double LAT_REF = 49.375;
double LON_REF = 4.222;
double temp_a = a * cos(DEG2RAD(LAT_REF));

void __SetXYSpherique(GpsPoint & gpsPoint){
    double lon = DEG2RAD(gpsPoint.m_longitude - LON_REF);
    double lat = DEG2RAD(gpsPoint.m_latitude - LAT_REF);
    gpsPoint.m_x = temp_a * lon;
    gpsPoint.m_y = a * lat;
}

/**
 * Mercator
 *
 * http://wiki.openstreetmap.org/wiki/Mercator
 **/
#define EARTH_RADIUS 6378137
double lat2y_m(double lat) { return log(tan( DEG2RAD(lat) / 2 + M_PI/4 )) * EARTH_RADIUS; }
double lon2x_m(double lon) { return          DEG2RAD(lon)                 * EARTH_RADIUS; }

void __MercatorCoordinatesTransform(GpsPoint & gpsPoint)
{
    gpsPoint.m_x = lon2x_m(gpsPoint.m_longitude);
    gpsPoint.m_y = lat2y_m(gpsPoint.m_latitude);
    
}


void GpsModule::setXY(GpsPoint & gpsPoint){
    //__YGCoordinatesTransform(gpsPoint, E_CLARK_IGN, lambert_n[LAMBERT], lambert_c[LAMBERT], LON_MERID_GREENWICH, 0.0, 0.0);
    __SetXYSpherique(gpsPoint);
}


