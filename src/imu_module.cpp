#include "imu_module.hpp"
#include "logging.hpp"
ImuModule::ImuModule(){
    
}

void ImuModule::remove(int s){
    for(int i= 0; i < s; ++i){
        m_list.pop_front();
    }
}

void ImuModule::run(){
    if(m_list.size() < 20){
        return;
    }
    if(m_list.front() != 0x55){
        INFO("error" << std::hex << m_list[0]);
        m_list.pop_front();
        
    } else {
        char c = m_list[1];
        if(c ==  0x51){
            /*char axl = m_list[2];
            char axh = m_list[3];
            char ayl = m_list[4];
            char ayh = m_list[5];
            char azl = m_list[6];
            char azh = m_list[7];
            char tl = m_list[8];
            char th = m_list[9];
            char sum = m_list[10];*/
            INFO("51" << std::hex << m_list[0] << std::hex << m_list[1] << std::hex << m_list[2] << std::hex << m_list[3] << std::hex << m_list[4] << std::hex << m_list[5] << std::hex << m_list[6] << std::hex << m_list[7] << std::hex << m_list[8] << std::hex << m_list[9] << std::hex << m_list[10]);
            remove(11);
            
        } else if(c ==  0x52){
            /*char wxl = m_list[2];
            char wxh = m_list[3];
            char wyl = m_list[4];
            char wyh = m_list[5];
            char wzl = m_list[6];
            char wzh = m_list[7];
            char tl = m_list[8];
            char th = m_list[9];
            char sum = m_list[10];*/
            INFO("52" << std::hex << m_list[0] << std::hex << m_list[1] << std::hex << m_list[2] << std::hex << m_list[3] << std::hex << m_list[4] << std::hex << m_list[5] << std::hex << m_list[6] << std::hex << m_list[7] << std::hex << m_list[8] << std::hex << m_list[9] << std::hex << m_list[10]);
            remove(11);
        } else if(c ==  0x53){
            /*char wxl = m_list[2];
            char wxh = m_list[3];
            char wyl = m_list[4];
            char wyh = m_list[5];
            char wzl = m_list[6];
            char wzh = m_list[7];
            char tl = m_list[8];
            char th = m_list[9];
            char sum = m_list[10];*/
            INFO("53" << std::hex << m_list[0] << std::hex << m_list[1] << std::hex << m_list[2] << std::hex << m_list[3] << std::hex << m_list[4] << std::hex << m_list[5] << std::hex << m_list[6] << std::hex << m_list[7] << std::hex << m_list[8] << std::hex << m_list[9] << std::hex << m_list[10]);
            remove(11);
        } else if(c ==  0x54){
            /*char hxl = m_list[2];
            char hxh = m_list[3];
            char hyl = m_list[4];
            char hyh = m_list[5];
            char hzl = m_list[6];
            char hzh = m_list[7];
            char tl = m_list[8];
            char th = m_list[9];
            char sum = m_list[10];*/
            INFO("54" << std::hex << m_list[0] << std::hex << m_list[1] << std::hex << m_list[2] << std::hex << m_list[3] << std::hex << m_list[4] << std::hex << m_list[5] << std::hex << m_list[6] << std::hex << m_list[7] << std::hex << m_list[8] << std::hex << m_list[9] << std::hex << m_list[10]);
            remove(11);
        } else if(c ==  0x56){
            /*char p0 = m_list[2];
            char p1 = m_list[3];
            char p2 = m_list[4];
            char p3 = m_list[5];
            char h0 = m_list[6];
            char h1 = m_list[7];
            char h2 = m_list[8];
            char h3 = m_list[9];
            char sum = m_list[10];*/
            INFO("54" << std::hex << m_list[0] << std::hex << m_list[1] << std::hex << m_list[2] << std::hex << m_list[3] << std::hex << m_list[4] << std::hex << m_list[5] << std::hex << m_list[6] << std::hex << m_list[7] << std::hex << m_list[8] << std::hex << m_list[9] << std::hex << m_list[10]);
            remove(11);
        }
    }
}

void ImuModule::addChar(char c){
    m_list.push_back(c);
    run();
}
