#include "imu_module.hpp"
#include "logging.hpp"
#include "gps_framework.hpp"

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
    std::cout << std::endl;
}

void ImuModule::run(){
    if(m_list.size() < 20){
        return;
    }
    if(m_list.front() != 0x55){
        INFO("error" << std::hex << m_list[0]);
        m_list.pop_front();
        
    } else {
        int c0 = m_list[0];
        int c1 = m_list[1];
        int c2 = m_list[2];
        int c3 = m_list[3];
        int c4 = m_list[4];
        int c5 = m_list[5];
        int c6 = m_list[6];
        int c7 = m_list[7];
        int c8 = m_list[8];
        int c9 = m_list[9];
        int sum = m_list[10];
        int acc_sum_ = c0 + c1 + c2 + c3 + c4 + c5 + c6 + c7 + c8 + c9;
        
        if((sum%256) == (acc_sum_%256)){
            if(c1 == 0x51){
                //acceleration
                signed short int ax = ((c3<<8)|c2);
                m_ax = ((double)ax)/32768.0*16.0*9.8;
                signed short int ay = ((c5<<8)|c4);
                m_ay = ((double)ay)/32768.0*16.0*9.8;
                signed short int az = ((c7<<8)|c6);
                m_az = ((double)az)/32768.0*16.0*9.8;
            } else if(c1 == 0x52){
                //acceleration angulaire
                signed short int axis_x = ((c3<<8)|c2);
                m_a_v_x = ((double)axis_x)/32768.0*2000.0;

                signed short int axis_y = ((c5<<8)|c4);
                m_a_v_y = ((double)axis_y)/32768.0*2000.0;

                signed short int axis_z = ((c7<<8)|c6);
                m_a_v_z = ((double)axis_z)/32768.0*2000.0;
            } else if(c1 == 0x53){
                //inclinometre
                signed short int axis_x = ((c3<<8)|c2);
                m_roll_x_deg = ((double)axis_x)/32768.0*180.0;
                
                signed short int axis_y = ((c5<<8)|c4);
                m_pitch_y_deg = ((double)axis_y)/32768.0*180.0;
                
                signed short int axis_z = ((c7<<8)|c6);
                m_yaw_z_deg = ((double)axis_z)/32768.0*180.0;
            } else if(c1 == 0x54){
                
                signed short int axis_x = ((c3<<8)|c2);
                m_mag_x = ((double)axis_x);
                
                signed short int axis_y = ((c5<<8)|c4);
                m_mag_y = ((double)axis_y);
                
                signed short int axis_z = ((c7<<8)|c6);
                m_mag_z = ((double)axis_z);
            }
        }

        remove(11);
    }
}

void ImuModule::addIChar(int c){
    m_list.push_back(c);
    run();
}
