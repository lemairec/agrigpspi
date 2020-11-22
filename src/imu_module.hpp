#ifndef IMU_MODULE_H
#define IMU_MODULE_H
#include <string>
#include <memory>
#include <deque>


class ImuModule {
public:
    ImuModule();

    std::deque<char> m_list;
    
    void remove(int i);
    void run();
    void addChar(char c);
};

#endif //GPS_H
