#include "imu_module.hpp"
#include "logging.hpp"
ImuModule::ImuModule(){
    
}

void ImuModule::remove(int s){
    for(int i= 0; i < s; ++i){
        m_list.pop_front();
    }
}

void ImuModule::print(int s){
    std::cout << "     ";
    for(int i= 0; i < s; ++i){
        std::cout << std::hex << m_list[i] << " ";
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
        int c = m_list[1];
        if(c ==  0x51){
            print(11);
            /*unsigned char axl = m_list[2];
            unsigned char axh = m_list[3];
            unsigned char ayl = m_list[4];
            unsigned char ayh = m_list[5];
            unsigned char azl = m_list[6];
            unsigned char azh = m_list[7];
            unsigned char tl = m_list[8];
            unsigned char th = m_list[9];
            unsigned char sum = m_list[10];*/
            INFO("51");
            remove(11);
            
        } else if(c ==  0x52){
            print(11);
            /*char wxl = m_list[2];
            char wxh = m_list[3];
            char wyl = m_list[4];
            char wyh = m_list[5];
            char wzl = m_list[6];
            char wzh = m_list[7];
            char tl = m_list[8];
            char th = m_list[9];
            char sum = m_list[10];*/
            INFO("52");
            remove(11);
        } else if(c ==  0x53){
            print(11);
            /*char wxl = m_list[2];
            char wxh = m_list[3];
            char wyl = m_list[4];
            char wyh = m_list[5];
            char wzl = m_list[6];
            char wzh = m_list[7];
            char tl = m_list[8];
            char th = m_list[9];
            char sum = m_list[10];*/
            INFO("53");
            remove(11);
        } else if(c ==  0x54){
            print(11);
            int hxl = m_list[2];
            int hxh = m_list[3];
            int hyl = m_list[4];
            int hyh = m_list[5];
            int hzl = m_list[6];
            int hzh = m_list[7];
            int tl = m_list[8];
            int th = m_list[9];
            int sum = m_list[10];
            
            int axis_x = ((hxh<<8)|hxl);
            m_axis_x = ((double)axis_x)/32768.0*180.0;
            
            int axis_y = ((hyh<<8)|hyl);
            m_axis_y = ((double)axis_y)/32768.0*180.0;
            
            int axis_z = ((hzh<<8)|hzl);
            m_axis_z = ((double)axis_z)/32768.0*180.0;
            
            int sum_ = 0x55 + 0x54 + hxl + hxh + hyl + hyh + hzl + hzh + tl + th;
            INFO("54 " << m_axis_x << " " << m_axis_y  << " " << m_axis_z << std::hex << sum_);
            remove(11);
        } else if(c ==  0x56){
            print(11);
            /*char p0 = m_list[2];
            char p1 = m_list[3];
            char p2 = m_list[4];
            char p3 = m_list[5];
            char h0 = m_list[6];
            char h1 = m_list[7];
            char h2 = m_list[8];
            char h3 = m_list[9];
            char sum = m_list[10];*/
            INFO("56");
            remove(11);
        }
    }
}

void ImuModule::addIChar(int c){
    m_list.push_back(c);
    run();
}
