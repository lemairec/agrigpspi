#ifndef IMU_MODULE_H
#define IMU_MODULE_H
#include <string>
#include <memory>
#include <deque>


class ImuModule {
public:
    ImuModule();

    std::deque<int> m_list;
    
    double m_roll_x_deg = 0;
    double m_pitch_y_deg = 0;
    double m_yaw_z_deg = 0;
    
    void print(int i);
    void remove(int i);
    void run();
    void addIChar(int c);
};

#endif //GPS_H
