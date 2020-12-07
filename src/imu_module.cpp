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
        int c = m_list[1];
        if(c ==  0x51){
            //print(11);
            int acc_xl = m_list[2];
            int acc_xh = m_list[3];
            int acc_yl = m_list[4];
            int acc_yh = m_list[5];
            int acc_zl = m_list[6];
            int acc_zh = m_list[7];
            int tl = m_list[8];
            int th = m_list[9];
            int sum = m_list[10];
            
            int sum_ = 0x55 + 0x51 + acc_xl + acc_xh + acc_yl + acc_yh + acc_zl + acc_zh + tl + th;
            if((sum%256) == (sum_%256)){
                int ax = ((acc_xh<<8)|acc_xl);
                m_ax = ((double)ax)/32768.0*16.0*9.8;
                int ay = ((acc_yh<<8)|acc_yl);
                m_ay = ((double)ay)/32768.0*16.0*9.8;
                int az = ((acc_zh<<8)|acc_zl);
                m_az = ((double)az)/32768.0*16.0*9.8;
            } else {
                m_ax = 0.0;
                m_ay = 0.0;
                m_az = 0.0;
            }
            
        } else if(c ==  0x52){
            //print(11);
            /*char wxl = m_list[2];
            char wxh = m_list[3];
            char wyl = m_list[4];
            char wyh = m_list[5];
            char wzl = m_list[6];
            char wzh = m_list[7];
            char tl = m_list[8];
            char th = m_list[9];
            char sum = m_list[10];*/
            //INFO("52");
        } else if(c ==  0x53){
            int wxl = m_list[2];
            int wxh = m_list[3];
            int wyl = m_list[4];
            int wyh = m_list[5];
            int wzl = m_list[6];
            int wzh = m_list[7];
            //int vl = m_list[8];
            //int vh = m_list[9];
            //int sum = m_list[10];
            //int sum_ = 0x55 + 0x54 + wxl + wxh + wyl + wyh + wzl + wzh + vl + vh;
            
            int axis_x = ((wxh<<8)|wxl);
            m_roll_x_deg = ((double)axis_x)/32768.0*180.0;
            if(m_roll_x_deg > 180){
                m_roll_x_deg = m_roll_x_deg-360.0;
            }
            
            int axis_y = ((wyh<<8)|wyl);
            m_pitch_y_deg = ((double)axis_y)/32768.0*180.0;
            if(m_pitch_y_deg > 180){
                m_pitch_y_deg = m_pitch_y_deg-360.0;
            }
            
            int axis_z = ((wzh<<8)|wzl);
            m_yaw_z_deg = ((double)axis_z)/32768.0*180.0;
            if(m_yaw_z_deg > 180){
                m_yaw_z_deg = m_yaw_z_deg-360.0;
            }
            
            if(m_imu_moy>0){
                m_values.push_front(m_pitch_y_deg);
                while(m_values.size()>m_imu_moy){
                    m_values.pop_back();
                }
                
                double sum = 0;
                for(auto c : m_values){
                    sum += c;
                }
                m_moy_corr_deg = sum/m_values.size();
            } else {
                m_moy_corr_deg = m_pitch_y_deg;
            }
            GpsFramework::Instance().m_imu_time.setNewTime();
             
            //print(11);
        } else if(c ==  0x54){
            //magnetic output
            int mag_axl = m_list[2];
            int mag_axh = m_list[3];
            int mag_ayl = m_list[4];
            int mag_ayh = m_list[5];
            int mag_azl = m_list[6];
            int mag_azh = m_list[7];
            //unsigned char tl = m_list[8];
            //unsigned char th = m_list[9];
            //unsigned char sum = m_list[10];
            
            int axis_x = ((mag_axh<<8)|mag_axl);
            m_mag_x = ((double)axis_x);
            int axis_y = ((mag_ayh<<8)|mag_ayl);
            m_mag_y = ((double)axis_y);
            int axis_z = ((mag_azh<<8)|mag_azl);
            m_mag_z = ((double)axis_z);
            
            
        } else if(c ==  0x56){
            //print(11);
            /*char p0 = m_list[2];
            char p1 = m_list[3];
            char p2 = m_list[4];
            char p3 = m_list[5];
            char h0 = m_list[6];
            char h1 = m_list[7];
            char h2 = m_list[8];
            char h3 = m_list[9];
            char sum = m_list[10];*/
            //INFO("56");
        }
        remove(11);
    }
}

void ImuModule::addIChar(int c){
    m_list.push_back(c);
    run();
}
