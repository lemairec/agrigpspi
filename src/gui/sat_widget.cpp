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
    auto last_frame = GpsFramework::Instance().m_lastGGAFrame;
    scene->addRect(x, m_height*0.1, m_width-x, m_height*0.8, m_penBlack, m_brushWhiteAlpha);
    //scene->addRect(m_width*0.2, m_height*0.1, m_width*0.08, m_height*0.8, m_penBlack, m_brushDarkGray);
    
    int x2 = x+30;
    if(!m_debug){
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
    } else {
        {
            QString s = "Debug";
            drawQText(s, (m_width+x)/2, 0.15*m_height, sizeText_big, true);
        }
        
    }
    
    drawButtonImage(&m_buttonOk, *m_imgOk);
    
    if(m_debug){
       drawButton(&m_buttonDebug, COLOR_CHECK);
    } else {
        drawButton(&m_buttonDebug);
    }
    
}
void SatWidget::onMouseInt(int x, int y){
    if(m_buttonOk.isActive(x, y)){
        m_close = true;
    }
    if(m_buttonDebug.isActive(x, y)){
        m_debug = !m_debug;
    }
}