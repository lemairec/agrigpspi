#ifndef IMU_MODULE_H
#define IMU_MODULE_H
#include <string>
#include <memory>
#include <deque>

#include <list>

class ImuModule {
public:
    ImuModule();

    std::deque<int> m_list;
    
    double m_roll_x_deg = 0;
    double m_pitch_y_deg = 0;
    double m_yaw_z_deg = 0;
    
    double m_ax = 0;
    double m_ay = 0;
    double m_az = 0;
    
    double m_a_v_x = 0;
    double m_a_v_y = 0;
    double m_a_v_z = 0;
    
    double m_mag_x = 0;
    double m_mag_y = 0;
    double m_mag_z = 0;
    
    std::list<int> m_values;
    
    void print(int i);
    void remove(int i);
    void run();
    void addIChar(int c);
};

#endif //GPS_H
