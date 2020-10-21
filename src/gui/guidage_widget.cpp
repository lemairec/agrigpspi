
#include "guidage_widget.hpp"

#include "../gps_framework.hpp"
#include "gps_widget.hpp"


GuidageWidget::GuidageWidget(){
    m_imgOk = loadImage("/images/ok.png");
    m_imgA = loadImage("/images/a.png");
    m_imgB = loadImage("/images/b.png");
    m_imgLigneAB = loadImage("/images/line_ab.png");
    m_imgCurveAB = loadImage("/images/curve_ab.png");
    
}


void GuidageWidget::setSize(int width, int height){
    BaseWidget::setSize(width, height);
    
    m_x = 0;
    m_lg = 0.30*m_width;
    m_buttonA.setResize(m_lg/3, 0.6*m_height, m_petit_button);
    m_buttonB.setResize(2*m_lg/3, 0.6*m_height, m_petit_button);
    m_buttonCurveAB.setResize(m_lg/3, 0.4*m_height, m_petit_button);
    m_buttonLigneAB.setResize(2*m_lg/3, 0.4*m_height, m_petit_button);
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
    
    
    drawButtonImage(&m_buttonOk, *m_imgOk);
    drawButtonImage(&m_buttonA, *m_imgA);
    drawButtonImage(&m_buttonB, *m_imgB);
    drawButtonImage(&m_buttonCurveAB, *m_imgCurveAB);
    drawButtonImage(&m_buttonLigneAB, *m_imgLigneAB);
    
}
void GuidageWidget::onMouseInt(int x, int y){
    if(m_buttonOk.isActive(x, y)){
        m_close = true;
    }
    GpsFramework & f = GpsFramework::Instance();
    
    if(m_buttonA.isActive(x, y)){
        f.savePointA();
    }
    
    if(m_buttonB.isActive(x, y)){
        f.savePointB();
    }

}
