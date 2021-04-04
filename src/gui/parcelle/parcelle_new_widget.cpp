
#include "parcelle_new_widget.hpp"

#include "../../gps_framework.hpp"
#include "../gps_widget.hpp"


ParcelleNewWidget::ParcelleNewWidget(){
    m_imgOk = loadImage("/images/ok.png");
    m_imgCancel = loadImage("/images/cancel.png");
    m_imgFlag = loadImage("/images/flag.png");
    m_imgRecord = loadImage("/images/record.png");
    m_imgPause = loadImage("/images/pause.png");
}


void ParcelleNewWidget::setSize(int width, int height){
    BaseWidget::setSize(width, height);
    
    m_x = 0;
    m_lg = 0.30*m_width;
    m_name.setResize(m_x + 0.15*m_width, 0.20*m_width);
    m_buttonParcelleStartPause.setResize(m_lg*1/4, 0.5*m_height, m_petit_button);
    m_buttonFlag.setResize(m_lg*1/4, 0.6*m_height, m_petit_button);
    
    m_buttonParcellePoint.setResize(m_lg*1/4, 0.7*m_height, m_petit_button);
    m_buttonOk.setResize(m_lg*2.0/3.0, 0.85*m_height, m_petit_button);
    m_buttonCancel.setResize(m_lg/3.0, 0.85*m_height, m_petit_button);
    
    
}

void ParcelleNewWidget::draw(){
    GpsFramework & f = GpsFramework::Instance();
    m_painter->setPen(m_penBlack);
    m_painter->setBrush(m_brushWhiteAlpha);
    m_painter->drawRect(m_x, m_height*0.1, m_lg, m_height*0.8);
    
    {
        QString s = "nouvelle parcelle";
        drawQText(s, m_lg/2, 0.15*m_height, sizeText_big, true);
    }
    
    {
        QString s = "Nom de la parcelle : ";
        drawQText(s, m_lg/2, 0.25*m_height, sizeText_big, true);
    }
    drawValueGuiKeyBoard(m_name);
    {
        QPixmap * p = m_imgRecord;
        QString s = "Play";
        if(f.m_etat == Etat_ParcelleAdd){
            s = "Pause";
            p = m_imgPause;
        }
        drawQText(s, m_buttonParcelleStartPause.m_x+30, m_buttonParcelleStartPause.m_y, sizeText_big, false);
        drawButtonImage(m_buttonParcelleStartPause, p);
        
    }
    {
        QString s = "Flag";
        drawQText(s, m_buttonFlag.m_x+30, m_buttonFlag.m_y, sizeText_big, false);
        drawButtonImage(m_buttonFlag, m_imgFlag);
        
        
    }
    
    {
        QString s = "Middle";
        if(f.m_parcellePoint == ParcellePointLeft){
            s = "Left";
        } else if(f.m_parcellePoint == ParcellePointRight){
            s = "Right";
        }
        drawQText(s, m_buttonParcellePoint.m_x+30, m_buttonParcellePoint.m_y, sizeText_big, false);
        drawButton(m_buttonParcellePoint);
        
        
    }
    
    
    drawButtonImage(m_buttonOk, m_imgOk);
    drawButtonImage(m_buttonCancel, m_imgCancel);
    
    int x2 = m_width-m_lg;
    
    m_painter->setPen(m_penBlack);
    m_painter->setBrush(m_brushWhiteAlpha);
    m_painter->drawRect(m_width-m_lg, m_height*0.1, m_lg, m_height*0.8);
    {
        f.m_parcelle.calculSurface();
        QString s = "Surface :\n "+QString::number(f.m_parcelle.m_surface_ha)+" ha";
        drawQText(s, x2+30, m_buttonFlag.m_y, sizeText_big, false);
        drawButtonImage(m_buttonFlag, m_imgFlag);
        
        
    }
}
void ParcelleNewWidget::onMouse(int x, int y){
    GpsFramework & f = GpsFramework::Instance();
    if(m_buttonCancel.isActive(x, y)){
        m_close = true;
        f.m_etat = Etat_Reset;
    }
    if(m_buttonOk.isActive(x, y)){
        m_close = true;
        f.m_etat = Etat_Reset;
        f.m_parcelle.m_name = m_name.m_text;
        f.m_parcelle.compute();
        f.m_parcelles.add(f.m_parcelle);
        
    }
    if(m_buttonParcelleStartPause.isActive(x, y)){
        if(f.m_etat == Etat_ParcelleAdd){
            f.m_etat = Etat_ParcellePause;
        } else {
            f.m_etat = Etat_ParcelleAdd;
        }
    }
    if(m_buttonFlag.isActive(x, y)){
        f.m_parcelle.addFlag();
    }
    if(m_buttonParcellePoint.isActive(x, y)){
        int i = ((int)(f.m_parcellePoint) + 1)%3;
        f.m_parcellePoint = (ParcellePoint)i;
    }
    if(isActiveValueGuiKeyBoard(m_name,x,y)){
        m_key_board_widget->m_close = false;
        m_key_board_widget->setValueGuiKeyBoard(&m_name);
    }
}

void ParcelleNewWidget::open(){
    GpsFramework & f = GpsFramework::Instance();
    f.m_etat = Etat_ParcellePause;
}
