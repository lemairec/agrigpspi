
#include "line_widget.hpp"

#include "../../gps_framework.hpp"
#include "../gps_widget.hpp"


LineWidget::LineWidget(){
    m_imgOk = loadImage("/images/ok.png");
}


void LineWidget::setSize(int width, int height){
    BaseWidget::setSize(width, height);
    m_x = 0;
    m_lg = 0.30*m_width;
    m_buttonOk.setResize(m_lg/2.0, 0.8*m_height, m_petit_button);
    
    m_buttonParcelleNew.setResize(m_x+30, 0.25*m_height, m_petit_button);
    m_buttonParcelleLoad.setResize(m_x+30, 0.35*m_height, m_petit_button);
}


void LineWidget::draw(){
    scene->addRect(m_x, m_height*0.1, m_lg, m_height*0.8, m_penBlack, m_brushWhiteAlpha);
    //scene->addRect(m_width*0.2, m_height*0.1, m_width*0.08, m_height*0.8, m_penBlack, m_brushDarkGray);
    
    {
        QString s = "Line";
        drawQText(s, m_lg/2, 0.15*m_height, sizeText_big, true);
    }
    
    {
        drawButton(&m_buttonParcelleNew);
        QString s = "New";
        drawQText(s, m_buttonParcelleNew.m_x + 50, m_buttonParcelleNew.m_y, sizeText_big, true);
    }
    {
        drawButton(&m_buttonParcelleLoad);
        QString s = "Load";
        drawQText(s, m_buttonParcelleLoad.m_x + 50, m_buttonParcelleLoad.m_y, sizeText_big, true);
    }
    
    {
        QString s = "Pas de lines\nselectionne";
        
        drawQText(s, m_x + m_lg/2, m_height*0.5, sizeText_big, true);
    }
    
    
    drawButtonImage(&m_buttonOk, *m_imgOk);
    
}
void LineWidget::onMouse(int x, int y){
    if(m_buttonOk.isActive(x, y)){
        m_close = true;
    }
    if(m_buttonParcelleNew.isActive(x, y)){
        GpsWidget * g = GpsWidget::Instance();
        g->m_lineNewWidget.open();
        g->m_lineNewWidget.m_close = false;
    }
    if(m_buttonParcelleLoad.isActive(x, y)){
        GpsWidget * g = GpsWidget::Instance();
        g->m_lineLoadWidget.open();
        g->m_lineLoadWidget.m_close = false;
    }

}
