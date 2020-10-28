#include "machine_widget.hpp"

#include "../gps_framework.hpp"
#include "keyboard_widget.hpp"

MachineWidget::MachineWidget(){
    m_imgOk = loadImage("/images/ok.png");
    m_imgMachine = loadImage("/images/machine.png");
    

}

void MachineWidget::setSize(int width, int height){
    BaseWidget::setSize(width, height);
    
    m_x = m_width*0.0;
    m_lg = m_width*1;
       
    m_buttonOk.setResize(m_x + m_lg/2, 0.85*m_height, m_petit_button);
    
    m_valueA.setResize(0.25*m_width, 0.75*m_height);
    m_valueB.setResize(0.5*m_width, 0.75*m_height);
    m_valueC.setResize(0.75*m_width, 0.75*m_height);
//    m_buttonDebug.setResize((m_width+x)/2-m_width/12, 0.8*m_height, m_petit_button);
}

void drawValueGuiTemp(BaseWidget * base, ValueGuiKeyPad * value){
    QString s = QString::number(value->m_value);
    base->scene->addRect(value->m_x-40, value->m_y-15, 80, 30, base->m_penBlack, base->m_brushGreen);
    base->drawQText(s, value->m_x, value->m_y, sizeText_medium, true);
    if(value->m_label.size() > 0){
        base->drawText(value->m_label, value->m_x-60, value->m_y, sizeText_medium, true);
    }
    if(value->m_unity.size() > 0){
        base->drawText(value->m_unity, value->m_x+60, value->m_y, sizeText_medium, true);
    }
    
}

bool isActiveTemp(ValueGuiKeyPad * value, int x, int y){
    if(value->m_x-30 < x && x < value->m_x + 30 && value->m_y-15 < y && y <value->m_y+15){
        return true;
    }
    return false;
    
}

void MachineWidget::open(){
    GpsFramework & f = GpsFramework::Instance();
    m_valueA.m_value = f.m_config.m_tracteur_hauteur;
    m_valueA.m_label = "A :";
    m_valueB.m_value = f.m_config.m_tracteur_empatement;
    m_valueB.m_label = "B :";
    m_valueC.m_value = f.m_config.m_tracteur_antenne_pont_arriere;
    m_valueC.m_label = "C :";
    
}

void MachineWidget::draw(){
    //auto last_frame = GpsFramework::Instance().m_lastGGAFrame;
    scene->addRect(m_x, m_height*0.1, m_width-m_x, m_height*0.8, m_penBlack, m_brushWhiteAlpha);

    
    {
        QString s = "Tracteur";
        drawQText(s, m_x + m_lg/2, 0.15*m_height, sizeText_big, true);
    }
    
    
    drawButtonImage(&m_buttonOk, *m_imgOk);
    drawMyImage(*m_imgMachine, 0.5*m_width ,0.42*m_height, 1.5, true);
    
    drawValueGuiTemp(this, &m_valueA);
    drawValueGuiTemp(this, &m_valueB);
    drawValueGuiTemp(this, &m_valueC);
    
}
void MachineWidget::onMouse(int x, int y){
    if(!m_key_pad_widget->m_close){
        return;
    }
    if(m_buttonOk.isActive(x, y)){
        GpsFramework & f = GpsFramework::Instance();
        f.m_config.m_tracteur_hauteur = m_valueA.m_value;
        f.m_config.m_tracteur_empatement = m_valueB.m_value;
        f.m_config.m_tracteur_antenne_pont_arriere = m_valueC.m_value;
        f.initOrLoadConfig();
               
        m_close = true;
    }
    
    if(isActiveTemp(&m_valueA,x,y)){
        m_key_pad_widget->m_close = false;
        m_key_pad_widget->setValueGuiKeyPad(&m_valueA);
    } else if(isActiveTemp(&m_valueB,x,y)){
        m_key_pad_widget->m_close = false;
        m_key_pad_widget->setValueGuiKeyPad(&m_valueB);
    } else if(isActiveTemp(&m_valueC,x,y)){
        m_key_pad_widget->m_close = false;
        m_key_pad_widget->setValueGuiKeyPad(&m_valueC);
    }
        
    
}









OutilWidget::OutilWidget(){
    m_imgOk = loadImage("/images/ok.png");
    m_imgOutil1 = loadImage("/images/outil_1.png");
    m_imgOutil2 = loadImage("/images/outil_2.png");
    

}

void OutilWidget::setSize(int width, int height){
    BaseWidget::setSize(width, height);
    
    m_x = m_width*0.0;
    m_lg = m_width*1;
       
    m_buttonOk.setResize(m_x + m_lg/2, 0.85*m_height, m_petit_button);
    
    m_valueA.setResize(0.3*m_width, 0.8*m_height);
    m_valueB.setResize(0.7*m_width, 0.8*m_height);
//    m_buttonDebug.setResize((m_width+x)/2-m_width/12, 0.8*m_height, m_petit_button);
}


void OutilWidget::open(){
    GpsFramework & f = GpsFramework::Instance();
    m_valueA.m_value = f.m_config.m_outil_distance;
    m_valueA.m_label = "A :";
    m_valueB.m_value = f.m_config.m_outil_largeur;
    m_valueB.m_label = "B :";
    
}

void OutilWidget::draw(){
    //auto last_frame = GpsFramework::Instance().m_lastGGAFrame;
    scene->addRect(m_x, m_height*0.1, m_width-m_x, m_height*0.8, m_penBlack, m_brushWhiteAlpha);

    
    {
        QString s = "Outil";
        drawQText(s, m_x + m_lg/2, 0.15*m_height, sizeText_big, true);
    }
    
    
    drawButtonImage(&m_buttonOk, *m_imgOk);
    drawMyImage(*m_imgOutil1, 0.25*m_width ,0.45*m_height, 0.8, true);
    drawMyImage(*m_imgOutil2, 0.75*m_width ,0.45*m_height, 0.9, true);
    
    drawValueGuiTemp(this, &m_valueA);
    drawValueGuiTemp(this, &m_valueB);
    
}
void OutilWidget::onMouse(int x, int y){
    if(!m_key_pad_widget->m_close){
        return;
    }
    if(m_buttonOk.isActive(x, y)){
        GpsFramework & f = GpsFramework::Instance();
        f.m_config.m_outil_distance = m_valueA.m_value;
        f.m_config.m_outil_largeur = m_valueB.m_value;
        f.initOrLoadConfig();
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





