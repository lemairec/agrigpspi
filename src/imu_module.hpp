#ifndef IMU_MODULE_H
#define IMU_MODULE_H
#include <string>
#include <memory>
#include <deque>


class ImuModule {
public:
    ImuModule();

    std::deque<unsigned char> m_list;
    
    void remove(int i);
    void run();
    void addUChar(unsigned char c);
};

#endif //GPS_H
