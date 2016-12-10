#ifndef GPS_MODULE_H
#define GPS_MODULE_H

#include <string>

struct GpsFrame {
    double m_altitude;
    
    double m_latitude = 0.0;
    double m_longitude = 0.0;
    
    double m_x = 0.0;
    double m_y = 0.0;
};

struct GGAFrame : public GpsFrame {
    int m_fix;
    int m_nbrSattelites;
    double m_precision;
    
};

class GpsModule {
public:
    GpsModule();
    
    void readFrame(const std::string & frame);
    void readChar(char c);
    
    void onGGAFrame(GGAFrame * frame);
    void onGGAFrame(const std::string & s);
    
    void setXY(GpsFrame & frame);
private:
    void resetBuffer();
    void parseBuffer();
    void parseGGA();
    
    int m_bufferIndLast = 0;
    char m_buffer[200];
    int m_tempInd = 0;
    
    
    void error();
    void debug();
    
    bool isEqual(const char * c, size_t size);
    void readUntilCommat();
    int getOneInt();
    int getIntWithChar(char c);
    int readInt();
    double readDouble();
    double readDeg();
    
};

#endif // GPS_MODULE_H
