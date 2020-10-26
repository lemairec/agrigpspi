#ifndef GPS_MODULE_H
#define GPS_MODULE_H
#include <string>
#include <memory>

struct GpsPoint {
    double m_altitude;
    
    double m_latitude = 0.0;
    double m_longitude = 0.0;
    
    double m_x = 0.0;
    double m_y = 0.0;
    
    double m_time = 0.0;
    double m_timeHour;
    
    bool m_isOk = false;
    
    double distanceCarre(GpsPoint & p2){
        return (p2.m_x - m_x)*(p2.m_x - m_x) + (p2.m_y - m_y)*(p2.m_y - m_y);
    }
};

struct GGAFrame : public GpsPoint {
    int m_fix;
    int m_nbrSat;
    double m_hdop;

    bool isOk(){
        return m_fix > 0;
    }
};

struct RMCFrame : public GpsPoint {
    double m_vitesse_noeud;
    double m_cap_deg;
    
    double m_vitesse_kmh;
    double m_cap_rad;
};

typedef std::shared_ptr<GGAFrame> GGAFrame_ptr;
typedef std::shared_ptr<RMCFrame> RMCFrame_ptr;
typedef std::shared_ptr<GpsPoint> GpsPoint_ptr;

class GpsModule {
public:
    GpsModule();
    void init();
    
    void printBuffer();
    void onGGAFrame(GGAFrame * ggaFrame);
    void onFrame(const std::string & s);
    
    void readFrame(const std::string & frame);
    void readChar(char c);
    
    void setXY(GpsPoint & gpsPoint);
    void setRef(double latitude, double longitude);
    
    GGAFrame m_lastGGAEvent;
    RMCFrame_ptr m_lastRMCEvent;
    double m_latitudeRef = 0;
private:
    int m_bufferIndLast = 0;
    char m_buffer[200];
    int m_tempInd = 0;
    
    double m_longitudeRef = 0;
    double m_acosLatRef = 0;
    
    void resetBuffer();
    void error();
    void debug();
    
    void parseBuffer();
    void parseGGA();
    void parseRMC();
    
    void SetXYSpherique(GpsPoint & gpsPoint);
    
    bool isEqual(const char * c, size_t size);
    void readUntilCommat();
    int getOneInt();
    int getIntWithChar(char c);
    int readInt();
    double readDouble();
    double readDeg();
    
    double getTimeHour(double d);
};

#endif //GPS_H
