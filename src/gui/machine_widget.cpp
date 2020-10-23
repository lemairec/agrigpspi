#include "machine_widget.hpp"

#include "../gps_framework.hpp"
#include "keyboard_widget.hpp"

MachineWidget::MachineWidget(){
    m_imgOk = loadImage("/images/ok.png");
    
}

void MachineWidget::setSize(int width, int height){
    BaseWidget::setSize(width, height);
    
    m_x = m_width*0.0;
    m_lg = m_width*1;
       
    m_buttonOk.setResize(m_x + m_lg/2, 0.8*m_height, m_petit_button);
    
    m_valueA.setResize(0.4*m_width, 0.5*m_height);
    m_valueB.setResize(0.6*m_width, 0.5*m_height);
//    m_buttonDebug.setResize((m_width+x)/2-m_width/12, 0.8*m_height, m_petit_button);
}

void drawValueGuiTemp(BaseWidget * base, ValueGuiKeyPad * value){
    QString s = QString::number(value->m_value);
    base->scene->addRect(value->m_x-5, value->m_y-10, 60, 20, base->m_penBlack, base->m_brushGreen);
    base->drawQText(s, value->m_x, value->m_y);
    
}

bool isActiveTemp(ValueGuiKeyPad * value, int x, int y){
    if(value->m_x-5 < x && x < value->m_x-5+60 && value->m_y-10 < y && y <value->m_y-10+20){
        return true;
    }
    return false;
    
}

void MachineWidget::draw(){
    auto last_frame = GpsFramework::Instance().m_lastGGAFrame;
    scene->addRect(m_x, m_height*0.1, m_width-m_x, m_height*0.8, m_penBlack, m_brushWhiteAlpha);

    
    {
        QString s = "Tracteur";
        drawQText(s, m_x + m_lg/2, 0.15*m_height, sizeText_big, true);
    }
    
    
    drawButtonImage(&m_buttonOk, *m_imgOk);
    drawValueGuiTemp(this, &m_valueA);
    drawValueGuiTemp(this, &m_valueB);
    
}
void MachineWidget::onMouseInt(int x, int y){
    if(!m_key_pad_widget->m_close){
        return;
    }
    if(m_buttonOk.isActive(x, y)){
        m_close = true;
    }
    
    if(isActiveTemp(&m_valueA,x,y)){
        m_key_pad_widget->m_close = false;
        m_key_pad_widget->setValueGuiKeyPad(&m_valueA);
    }
    if(isActiveTemp(&m_valueB,x,y)){
        m_key_pad_widget->m_close = false;
        m_key_pad_widget->setValueGuiKeyPad(&m_valueB);
    }
        
    
}
