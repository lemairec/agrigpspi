
#include "line_new_widget.hpp"

#include "../../gps_framework.hpp"
#include "../gps_widget.hpp"


LineNewWidget::LineNewWidget(){
    m_imgOk = loadImage("/images/ok.png");
    m_imgA = loadImage("/images/a.png");
    m_imgB = loadImage("/images/b.png");
    m_imgLigneAB = loadImage("/images/line_ab.png");
    m_imgCurveAB = loadImage("/images/curve_ab.png");
    
    m_close = true;
}


void LineNewWidget::setSize(int width, int height){
    BaseWidget::setSize(width, height);
    
    m_x = 0;
    m_lg = 0.30*m_width;
    
    m_name.setResize(m_x + m_lg/2, 0.35*m_height);
    m_buttonLigneCurve.setResize(m_lg/3, 0.5*m_height, m_petit_button);
    m_buttonAB.setResize(m_lg/3, 0.65*m_height, m_petit_button);
    m_buttonOk.setResize(m_lg/2.0, 0.8*m_height, m_petit_button);
}


void LineNewWidget::draw(){
    m_painter->setPen(m_penBlack);
    m_painter->setBrush(m_brushWhiteAlpha);
    m_painter->drawRect(m_x, m_height*0.1, m_lg, m_height*0.8);
    
    {
        QString s = "nouvelle ligne";
        drawQText(s, m_lg/2, 0.15*m_height, sizeText_big, true);
    }
    drawValueGuiKeyBoard(&m_name);
    
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
void LineNewWidget::onMouse(int x, int y){
    GpsFramework & f = GpsFramework::Instance();
    
    if(m_buttonOk.isActive(x, y)){
        if(m_name.m_text.size() > 2){
            if(f.m_line){
                f.m_lineAB.m_name = m_name.m_text;
                f.m_lines.add(f.m_lineAB);
            } else {
                f.m_curveAB.m_name = m_name.m_text;
                f.m_lines.add(f.m_curveAB);
            }
        }
        
        
        
        m_close = true;
    }
    
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
    
    if(isActiveValueGuiKeyBoard(&m_name,x,y)){
        m_key_board_widget->m_close = false;
        m_key_board_widget->setValueGuiKeyBoard(&m_name);
    }

}

void LineNewWidget::open(){
    GpsFramework & f = GpsFramework::Instance();
    f.setEtat(Etat_Reset);
}
