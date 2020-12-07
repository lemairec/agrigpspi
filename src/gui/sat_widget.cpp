#include "sat_widget.hpp"

#include "../gps_framework.hpp"
#include "gps_widget.hpp"


SatWidget::SatWidget(){
    m_imgOk = loadImage("/images/ok.png");
    
}

int x = 0;


void SatWidget::setSize(int width, int height){
    BaseWidget::setSize(width, height);
    
    x = m_width*0.7;
    m_buttonOk.setResize((m_width+x)/2+m_width/12, 0.8*m_height, m_petit_button);
    m_buttonDebug.setResize((m_width+x)/2-m_width/12, 0.8*m_height, m_petit_button);
}


void SatWidget::draw(){
    GpsFramework & f =  GpsFramework::Instance();
    auto last_frame = f.m_lastGGAFrame;
    scene->addRect(x, m_height*0.1, m_width-x, m_height*0.8, m_penBlack, m_brushWhiteAlpha);
    //scene->addRect(m_width*0.2, m_height*0.1, m_width*0.08, m_height*0.8, m_penBlack, m_brushDarkGray);
    
    int x2 = x+30;
    if(m_type == 0){
        {
            QString s = "Satellite";
            drawQText(s, (m_width+x)/2, 0.15*m_height, sizeText_big, true);
        }
        {
            QString s = QString::number(last_frame.m_nbrSat) + " satellites";
            drawQText(s, x2, 0.30*m_height, sizeText_medium, false);
        }
        {
            QString s = QString::number(last_frame.m_fix)+ " ";
            if(last_frame.m_fix == 1){
                s += "GPS";
            } else if(last_frame.m_fix == 2){
                s += "DGPS";
            } else if(last_frame.m_fix == 3){
                s += "PPSFIX";
            } else if(last_frame.m_fix == 4){
                s += "RTK";
            } else if(last_frame.m_fix == 5){
                s += "FRTK";
            } else if(last_frame.m_fix == 9){
                s += "DGPS compas";
            } else {
                s += "invalid";
            }
            drawQText(s, x2, 0.40*m_height, sizeText_medium, false);
        }
        {
            QString s = "hdop " + QString::number(last_frame.m_hdop, 'f', 2);
            drawQText(s, x2, 0.50*m_height, sizeText_medium, false);
        }
        {
            int h = last_frame.m_time/10000;
            int min = (int)(last_frame.m_time/100) - h*100;
            double sec = last_frame.m_time - h*10000 - min*100;
            QString s = QString::number(h) + ":" + QString::number(min) + ":" + QString::number(sec, 'f', 2);
            drawQText(s, x2, 0.60*m_height, sizeText_medium, false);
        }
    } else if (m_type == 1){
        {
            QString s = "Pilot";
            drawQText(s, (m_width+x)/2, 0.15*m_height, sizeText_big, true);
        }
        if(f.m_serialModule.pilotIsOpen()){
            drawText("open", x2, 0.25*m_height, sizeText_medium, false);
        } else {
            drawText("close", x2, 0.25*m_height, sizeText_medium, false);
        }
        {
            drawText("version", x2, 0.35*m_height, sizeText_medium, false);
        }
        {
            drawText(f.m_pilotModule.m_version_guidage, x2, 0.40*m_height, sizeText_medium, false);
        }
        {
            drawText(f.m_pilot_algo_str, x2, 0.50*m_height, sizeText_medium, false);
        }
        
        {
            drawText(f.m_pilot_last_error, x2, 0.60*m_height, sizeText_little, false);
        }
              
        
    } else  if (m_type == 2){
        {
            QString s = "IMU";
            drawQText(s, (m_width+x)/2, 0.15*m_height, sizeText_big, true);
        }
        if(f.m_serialModule.imuIsOpen()){
            drawText("open", x2, 0.25*m_height, sizeText_medium, false);
        } else {
            drawText("close", x2, 0.25*m_height, sizeText_medium, false);
        }
        
        {
            QString s = "roll_x "+QString::number(f.m_imuModule.m_roll_x_deg)+"°\npitch_y "+QString::number(f.m_imuModule.m_pitch_y_deg)+"°\nyaw_z "+QString::number(f.m_imuModule.m_yaw_z_deg)+"°";
            drawQText(s, x2, 0.4*m_height, sizeText_little, false);
        }
        
        {
            QString s = "a_x "+QString::number(f.m_imuModule.m_ax)+"\na_y "+QString::number(f.m_imuModule.m_ay)+"\na_z "+QString::number(f.m_imuModule.m_az)+"";
            drawQText(s, x2, 0.55*m_height, sizeText_little, false);
        }
        {
            QString s = "correction "+QString::number(round(f.m_tracteur.m_correction_lateral_imu*1000)/10.0)+" cm";
            drawQText(s, x2, 0.7*m_height, sizeText_little, false);
        }
    } else  if (m_type == 3) {
        {
            QString s = "Time";
            drawQText(s, (m_width+x)/2, 0.15*m_height, sizeText_big, true);
        }
        
        {
            QString s =  "draw " + QString::number(round(f.m_draw_time.m_moy*10)/10) + " ms " + QString::number(round(f.m_draw_time.m_et*10)/10);
            drawQText(s, x2, 0.3*m_height, sizeText_little, false);
        }
        
        {
            QString s =  "gps " + QString::number(round(f.m_gps_time.m_moy*10)/10) + " ms " + QString::number(round(f.m_gps_time.m_et*10)/10);
            drawQText(s, x2, 0.4*m_height, sizeText_little, false);
        }
        {
            QString s =  "pil " + QString::number(round(f.m_pilot_time.m_moy*10)/10) + " ms " + QString::number(round(f.m_pilot_time.m_et*10)/10);
            drawQText(s, x2, 0.45*m_height, sizeText_little, false);
        }
        {
            QString s =  "imu " + QString::number(round(f.m_imu_time.m_moy*10)/10) + " ms " + QString::number(round(f.m_imu_time.m_et*10)/10);
            drawQText(s, x2, 0.5*m_height, sizeText_little, false);
        }
        {
            QString s =  "virtual " + QString::number(round(f.m_virtual_point.m_moy*10)/10) + " ms " + QString::number(round(f.m_virtual_point.m_et*10)/10);
            drawQText(s, x2, 0.55*m_height, sizeText_little, false);
        }
        
        
    } else  if (m_type == 4) {
        {
            QString s = "Debug";
            drawQText(s, (m_width+x)/2, 0.15*m_height, sizeText_big, true);
        }
        
        {
            if(f.m_line){
                QString s = "lineAB\n\n";
                s += "m_d_e "+QString::number(round(f.m_lineAB.m_d_e*10000)/100.0)+" cm\n";
                s += "m_d_angle "+QString::number(round(f.m_lineAB.m_d_angle/3.14*180*100)/100.0)+" °\n";
                s += "m_d_k "+QString::number(round(f.m_lineAB.m_d_k*100)/100.0)+"\n\n";
                
                s += "m_d_xe (kte) "+QString::number(round(f.m_lineAB.m_d_xe*100)/100.0)+"\n";
                s += "m_d_xthe (kth) "+QString::number(round(f.m_lineAB.m_d_xthe*100)/100.0)+"\n";
                s += "m_d_xk "+QString::number(round(f.m_lineAB.m_d_xk*100)/100.0)+"\n\n";
                
                s += "m_d_res "+QString::number(round(f.m_lineAB.m_d_res*100)/100.0)+"\n";
                
                s += "m_d_delta "+QString::number(round(f.m_lineAB.m_d_delta/3.14*180*100)/100.0)+" °\n\n";
                
                drawQText(s, x2, 0.25*m_height, sizeText_little, false);
            } else {
                QString s = "curveAB\n\n";
                s += "m_d_e "+QString::number(round(f.m_curveAB.m_d_e*10000)/100.0)+" cm\n";
                s += "m_d_angle "+QString::number(round(f.m_curveAB.m_d_angle/3.14*180*100)/100.0)+" °\n";
                s += "m_d_k "+QString::number(round(f.m_curveAB.m_d_k*100)/100.0)+"\n\n";
                
                s += "m_d_xe (kte) "+QString::number(round(f.m_curveAB.m_d_xe*100)/100.0)+"\n";
                s += "m_d_xthe (kth) "+QString::number(round(f.m_curveAB.m_d_xthe*100)/100.0)+"\n";
                s += "m_d_xk "+QString::number(round(f.m_curveAB.m_d_xk*100)/100.0)+"\n\n";
                
                s += "m_d_res "+QString::number(round(f.m_curveAB.m_d_res*100)/100.0)+"\n";
                
                s += "m_d_delta "+QString::number(round(f.m_lineAB.m_d_delta/3.14*180*100)/100.0)+" °\n\n";
                drawQText(s, x2, 0.25   *m_height, sizeText_little, false);
            }
        }
    }
          
    
    
    drawButtonImage(&m_buttonOk, *m_imgOk);
    
    drawButton(&m_buttonDebug);
    
}
void SatWidget::onMouse(int x, int y){
    if(m_buttonOk.isActive(x, y)){
        m_close = true;
    }
    if(m_buttonDebug.isActive(x, y)){
        m_type = (m_type+1)%5;
    }
}
