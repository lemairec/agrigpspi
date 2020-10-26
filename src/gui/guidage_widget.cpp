
#include "guidage_widget.hpp"

#include "../gps_framework.hpp"
#include "gps_widget.hpp"


GuidageWidget::GuidageWidget(){
    m_imgOk = loadImage("/images/ok.png");
    m_imgA = loadImage("/images/a.png");
    m_imgB = loadImage("/images/b.png");
    m_imgLigneAB = loadImage("/images/line_ab.png");
    m_imgCurveAB = loadImage("/images/curve_ab.png");
    
    m_close = false;
}


void GuidageWidget::setSize(int width, int height){
    BaseWidget::setSize(width, height);
    
    m_x = 0;
    m_lg = 0.30*m_width;
    m_buttonAB.setResize(m_lg/3, 0.6*m_height, m_petit_button);
    m_buttonLigneCurve.setResize(m_lg/3, 0.4*m_height, m_petit_button);
    m_buttonOk.setResize(m_lg/2.0, 0.8*m_height, m_petit_button);
}


void GuidageWidget::draw(){
    auto last_frame = GpsFramework::Instance().m_lastGGAFrame;
    scene->addRect(m_x, m_height*0.1, m_lg, m_height*0.8, m_penBlack, m_brushWhiteAlpha);
    //scene->addRect(m_width*0.2, m_height*0.1, m_width*0.08, m_height*0.8, m_penBlack, m_brushDarkGray);
    
    int x2 = m_x+30;
    {
        QString s = "nouvelle ligne";
        drawQText(s, m_lg/2, 0.15*m_height, sizeText_big, true);
    }
    
    
    GpsFramework & f = GpsFramework::Instance();
    drawButtonImage(&m_buttonOk, *m_imgOk);
    if(f.m_etat == Etat_Reset){
        drawButtonImage(&m_buttonAB, *m_imgA);
    } else {
        drawButtonImage(&m_buttonAB, *m_imgB);
    }
    if(f.m_line){
        drawButtonImage(&m_buttonLigneCurve, *m_imgLigneAB);
    } else {
        drawButtonImage(&m_buttonLigneCurve, *m_imgCurveAB);
    }
    
    
    
}
void GuidageWidget::onMouseInt(int x, int y){
    if(m_buttonOk.isActive(x, y)){
        m_close = true;
    }
    GpsFramework & f = GpsFramework::Instance();
    
    if(m_buttonAB.isActive(x, y)){
        if(f.m_etat == Etat_Reset){
            f.savePointA();
        } else {
            f.savePointB();
        }
    }

    if(m_buttonLigneCurve.isActive(x, y)){
        f.m_line = !f.m_line;
    }

}

void GuidageWidget::open(){
    GpsFramework & f = GpsFramework::Instance();
    f.setEtat(Etat_Reset);
}
