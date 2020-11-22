#ifndef IMU_MODULE_H
#define IMU_MODULE_H
#include <string>
#include <memory>
#include <deque>


class ImuModule {
public:
    ImuModule();

    std::deque<int> m_list;
    
    double m_axis_x = 0;
    double m_axis_y = 0;
    double m_axis_z = 0;
    
    void print(int i);
    void remove(int i);
    void run();
    void addIChar(int c);
};

#endif //GPS_H
