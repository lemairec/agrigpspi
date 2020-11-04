
#include "parcelle_new_widget.hpp"

#include "../../gps_framework.hpp"
#include "../gps_widget.hpp"


ParcelleNewWidget::ParcelleNewWidget(){
    m_imgOk = loadImage("/images/ok.png");
}


void ParcelleNewWidget::setSize(int width, int height){
    BaseWidget::setSize(width, height);
    
    m_x = 0;
    m_lg = 0.70*m_width;
    m_buttonOk.setResize(m_lg/2.0, 0.8*m_height, m_petit_button);
    
    m_name.setResize(m_x + m_lg*3/4, 0.35*m_height);
    
}


void ParcelleNewWidget::draw(){
    scene->addRect(m_x, m_height*0.1, m_lg, m_height*0.8, m_penBlack, m_brushWhiteAlpha);
    //scene->addRect(m_width*0.2, m_height*0.1, m_width*0.08, m_height*0.8, m_penBlack, m_brushDarkGray);
    
    {
        QString s = "Nouvelle Parcelle";
        drawQText(s, m_lg/2, 0.15*m_height, sizeText_big, true);
    }
    
    {
        QString s = "Nom de la parcelle : ";
        drawQText(s, m_x+30, 0.35*m_height, sizeText_big, false);
    }
    
    drawButtonImage(&m_buttonOk, *m_imgOk);
    drawValueGuiKeyBoard(&m_name);
}
void ParcelleNewWidget::onMouse(int x, int y){
    if(m_buttonOk.isActive(x, y)){
        m_close = true;
    }
    if(isActiveValueGuiKeyBoard(&m_name,x,y)){
        m_key_board_widget->m_close = false;
        m_key_board_widget->setValueGuiKeyBoard(&m_name);
    }
}
