#include "infos_widget.hpp"

#include "../gps_framework.hpp"
#include "gps_widget.hpp"


InfosWidget::InfosWidget(){
    m_imgOk = loadImage("/images/ok.png");
    m_imgRecentre = loadImage("/images/ab_middle.png");
}

int x = 0;


void InfosWidget::setSize(int width, int height){
    BaseWidget::setSize(width, height);
    
    x = m_width*0.7;
    m_buttonOk.setResize(0.8*m_width, 0.8*m_height, m_petit_button);
    m_buttonDebug.setResize(0.9*m_width, 0.8*m_height, m_petit_button);
    
    setSize0(width, height);
    setSize1(width, height);
}


void InfosWidget::draw(){
    GpsFramework & f =  GpsFramework::Instance();
    auto last_frame = f.m_lastGGAFrame;
    m_painter->setPen(m_penBlack);
    m_painter->setBrush(m_brushWhiteAlpha);
    m_painter->drawRect(m_width*0.7, m_height*0.1, m_width*0.3, m_height*0.8);
    
    int x2 = x+30;
    if(m_type == 0){
        drawPage0();
    }else if(m_type == 1){
        drawPage1();
    }else if(m_type == 2){
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
        {
            QString s = "lat " + QString::number(last_frame.m_latitude, 'f', 4);
            drawQText(s, x2, 0.65*m_height, sizeText_medium, false);
        }
        {
            QString s = "long " + QString::number(last_frame.m_longitude, 'f', 4);
            drawQText(s, x2, 0.70*m_height, sizeText_medium, false);
        }
        
    } else if (m_type == 3){
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
              
        
    } else  if (m_type == 4){
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
            drawQTexts(s, x2, 0.4*m_height, sizeText_little, false);
        }
        
        {
            QString s = "a_x "+QString::number(f.m_imuModule.m_ax)+"\na_y "+QString::number(f.m_imuModule.m_ay)+"\na_z "+QString::number(f.m_imuModule.m_az)+"";
            drawQTexts(s, x2, 0.55*m_height, sizeText_little, false);
        }
        {
            QString s = "correction "+QString::number(round(f.m_tracteur.m_correction_lateral_imu*1000)/10.0)+" cm";
            drawQText(s, x2, 0.7*m_height, sizeText_little, false);
        }
    } else  if (m_type == 5) {
        {
            QString s = "Info";
            drawQText(s, (m_width+x)/2, 0.15*m_height, sizeText_big, true);
        }
        
        {
            QString s =  "draw " + QString::number(round(f.m_draw_time.m_moy*10)/10) + " ms " + QString::number(round(f.m_draw_time.m_et*10)/10);
            s = s +  "\n\ngps " + QString::number(round(f.m_gps_time.m_moy*10)/10) + " ms " + QString::number(round(f.m_gps_time.m_et*10)/10);
            s = s +  "\npil " + QString::number(round(f.m_pilot_time.m_moy*10)/10) + " ms " + QString::number(round(f.m_pilot_time.m_et*10)/10);
            s = s +  "\nimu " + QString::number(round(f.m_imu_time.m_moy*10)/10) + " ms " + QString::number(round(f.m_imu_time.m_et*10)/10);
            s = s +  "\nvirtual " + QString::number(round(f.m_virtual_point.m_moy*10)/10) + " ms " + QString::number(round(f.m_virtual_point.m_et*10)/10);
            s = s +  "\n\npendant 5s\ndistance " + QString::number(round(f.m_stat_distance.m_moy*1000)/10) + " cm ";
            s = s +  "\necart " + QString::number(round(f.m_stat_distance.m_moy_carre*1000)/10) + " cm ";
            drawQTexts(s, x2, 0.25*m_height, sizeText_little, false);
        }
        
        
    } else  if (m_type == 6) {
        {
            QString s = "Line";
            drawQText(s, (m_width+x)/2, 0.15*m_height, sizeText_big, true);
        }
        
        {
            if(f.m_line){
                QString s = "lineAB\n\n";
                s += "offset "+QString::number(round(f.m_lineAB.m_offset*10000)/100.0)+" cm\n";
                
                drawQTexts(s, x2, 0.25*m_height, sizeText_little, false);
            } else {
                QString s = "curveAB\n\n";
                s += "offset "+QString::number(round(f.m_lineAB.m_offset*10000)/100.0)+" cm\n";
                
                drawQTexts(s, x2, 0.25   *m_height, sizeText_little, false);
            }
        }
    } else  if (m_type == 7) {
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
                
                drawQTexts(s, x2, 0.25*m_height, sizeText_little, false);
            } else {
                QString s = "curveAB\n\n";
                s += "m_d_e "+QString::number(round(f.m_curveAB.m_d_e*10000)/100.0)+" cm\n";
                s += "m_d_angle "+QString::number(round(f.m_curveAB.m_d_angle/3.14*180*100)/100.0)+" °\n";
                s += "m_d_k "+QString::number(round(f.m_curveAB.m_d_k*100)/100.0)+"\n\n";
                
                s += "m_d_xe (kte) "+QString::number(round(f.m_curveAB.m_d_xe*100)/100.0)+"\n";
                s += "m_d_xthe (kth) "+QString::number(round(f.m_curveAB.m_d_xthe*100)/100.0)+"\n";
                s += "m_d_xk "+QString::number(round(f.m_curveAB.m_d_xk*100)/100.0)+"\n\n";
                
                s += "m_d_res "+QString::number(round(f.m_curveAB.m_d_res*100)/100.0)+"\n";
                
                s += "m_d_delta "+QString::number(round(f.m_curveAB.m_d_delta/3.14*180*100)/100.0)+" °\n\n";
                drawQTexts(s, x2, 0.25   *m_height, sizeText_little, false);
            }
        }
    }
          
    
    
    drawButtonImage(m_buttonOk, m_imgOk);
    
    drawButton(m_buttonDebug);
    
}
void InfosWidget::onMouse(int x, int y){
    if(m_buttonOk.isActive(x, y)){
        m_close = true;
    }
    if(m_buttonDebug.isActive(x, y)){
        m_type = (m_type+1)%7;
    }
    if(m_type == 0){
        onMouse0(x, y);
    } else if(m_type == 1){
        onMouse1(x, y);
    }
}


void InfosWidget::setSize0(int width, int height){
    m_ekf.setResize(m_width*0.8, 0.25*height, m_petit_button, "ekf ");
    m_cap_ekf.setResize(m_width*0.8, 0.45*height, m_petit_button, "ekf ");
    m_cap_d.setResize(m_width*0.8, 0.45*height, m_petit_button, "d ");
    m_lk.setResize(m_width*0.8, 0.65*height, m_petit_button, "lk ");
    m_kth.setResize(m_width*0.8, 0.65*height, m_petit_button, "kth ");
}
void InfosWidget::drawPage0(){
    GpsFramework & f =  GpsFramework::Instance();
    {
        QString s = "Menu Rapide";
        drawQText(s, (m_width+x)/2, 0.15*m_height, sizeText_big, true);
    }
    
    if(f.m_config.m_lissage_gps_mode == LissageGpsMode_Ekf){
        QString s = "GPS ekf";
        drawQText(s, (m_width+x)/2, 0.2*m_height, sizeText_medium, true);
        drawValueGui(m_ekf, f.m_config.m_lissage_gps_ekf*100);
    } else {
        QString s = "pas de lissage";
        drawQText(s, (m_width+x)/2, 0.2*m_height, sizeText_medium, true);
    }
    
    if(f.m_config.m_cap_mode == CapMode_Ekf){
        QString s = "Cap ekf";
        drawQText(s, (m_width+x)/2, 0.4*m_height, sizeText_medium, true);
        drawValueGui(m_cap_ekf, f.m_config.m_cap_ekf*100);
    } else if(f.m_config.m_cap_mode == CapMode_Custom){
        QString s = "Cap custom";
        drawQText(s, (m_width+x)/2, 0.4*m_height, sizeText_medium, true);
        drawValueGui(m_cap_d, f.m_config.m_cap_custom_d);
    }
    
    if(f.m_config.m_pilot_algo == FollowCarrotPontAvant){
        QString s = "follow carrot avant";
        drawQText(s, (m_width+x)/2, 0.6*m_height, sizeText_medium, true);
        drawValueGui(m_lk, f.m_config.m_pilot_lookahead_d);
        
    } else if(f.m_config.m_pilot_algo == FollowCarrotPontArriere){
        QString s = "follow carrot arriere";
        drawQText(s, (m_width+x)/2, 0.6*m_height, sizeText_medium, true);
        drawValueGui(m_lk, f.m_config.m_pilot_lookahead_d);
        
    } else if(f.m_config.m_pilot_algo == RearWheelPosition){
        QString s = "rwp";
        drawQText(s, (m_width+x)/2, 0.6*m_height, sizeText_medium, true);
        drawValueGui(m_kth, f.m_config.m_pilot_rwp_kth);
        QString s2 = "kte "+QString::number(f.m_pilot_rwp_kte);
        drawQText(s2, m_kth.m_x, 0.7*m_height, sizeText_medium, false);
    }
}

void InfosWidget::onMouse0(int x, int y){
    GpsFramework & f = GpsFramework::Instance();
    
    if(f.m_config.m_lissage_gps_mode == LissageGpsMode_Ekf){
        f.m_config.m_lissage_gps_ekf += 0.05*m_ekf.getIntValue(x,y);
    }
    
    if(f.m_config.m_cap_mode == CapMode_Ekf){
        f.m_config.m_cap_ekf += 0.05*m_cap_ekf.getIntValue(x,y);
    }
    if(f.m_config.m_cap_mode == CapMode_Custom){
        f.m_config.m_cap_custom_d += 0.5*m_cap_d.getIntValue(x,y);
    }
    
    f.m_config.m_pilot_lookahead_d += 0.5*m_lk.getIntValue(x,y);
    f.m_config.m_pilot_rwp_kth += 0.1*m_kth.getIntValue(x,y);
    f.m_config.m_pilot_rwp_kte = f.m_config.m_pilot_rwp_kth/2;
    f.initOrLoadConfig();
}


void InfosWidget::setSize1(int width, int height){
    m_buttonRecentre.setResize(m_width*0.8, 0.35*height, m_petit_button);
    m_buttonMoins10.setResize(m_width*0.78, 0.45*height, m_petit_button);
    m_buttonPlus10.setResize(m_width*0.88, 0.45*height, m_petit_button);
}
void InfosWidget::drawPage1(){
    GpsFramework & f = GpsFramework::Instance();
    {
        QString s = "Menu Bineuse";
        drawQText(s, (m_width+x)/2, 0.15*m_height, sizeText_big, true);
    }
    
    {
        QString s = QString::number(-f.m_tracteur.m_antenne_lateral*100) + " cm";
        drawQText(s, (m_width+x)/2, 0.25*m_height, sizeText_big, true);
    }
    
    drawButtonImage(m_buttonRecentre, m_imgRecentre);
    drawButton(m_buttonMoins10);
    {
        QString s = "G";
        drawQText(s, m_buttonMoins10.m_x, m_buttonMoins10.m_y, sizeText_big, true);
    }
    drawButton(m_buttonPlus10);
    {
        QString s = "D";
        drawQText(s, m_buttonPlus10.m_x, m_buttonPlus10.m_y, sizeText_big, true);
    }
}

void InfosWidget::onMouse1(int x, int y){
    GpsFramework & f = GpsFramework::Instance();
    
    if(m_buttonRecentre.isActive(x, y)){
        f.m_config.m_offset_AB -= f.getOffsetAB();
        f.m_tracteur.m_antenne_lateral = 0;
        f.initOrLoadConfig();
    }
    if(m_buttonPlus10.isActive(x, y)){
        f.m_tracteur.m_antenne_lateral -= 0.05;
    }
    if(m_buttonMoins10.isActive(x, y)){
        f.m_tracteur.m_antenne_lateral += 0.05;
    }
    INFO(f.m_tracteur.m_antenne_lateral);

}
