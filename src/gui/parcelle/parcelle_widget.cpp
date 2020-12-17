
#include "parcelle_widget.hpp"

#include "../../gps_framework.hpp"
#include "../gps_widget.hpp"


ParcelleWidget::ParcelleWidget(){
    m_imgOk = loadImage("/images/ok.png");
}


void ParcelleWidget::setSize(int width, int height){
    BaseWidget::setSize(width, height);
    m_x = 0;
    m_lg = 0.30*m_width;
    m_buttonOk.setResize(m_lg/2.0, 0.8*m_height, m_petit_button);
    
    m_buttonParcelleNew.setResize(m_x+30, 0.25*m_height, m_petit_button);
    m_buttonParcelleLoad.setResize(m_x+30, 0.35*m_height, m_petit_button);
}


void ParcelleWidget::draw(){
    m_painter->setPen(m_penBlack);
    m_painter->setBrush(m_brushWhiteAlpha);
    m_painter->drawRect(m_x, m_height*0.1, m_lg, m_height*0.8);
    
    {
        QString s = "Parcelles";
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
        GpsFramework & f = GpsFramework::Instance();
        QString s = "Pas de parcelle\nselectionne";
        if(f.m_parcelle.isInit()){
            s = QString::fromStdString(f.m_parcelle.m_name) + "\nsurface : \n" + QString::number(f.m_parcelle.m_surface_ha) + " ha";
        }
        drawQTexts(s, m_x + m_lg/2, m_height*0.5, sizeText_big, true);
    }
    
    
    drawButtonImage(&m_buttonOk, *m_imgOk);
    
}
void ParcelleWidget::onMouse(int x, int y){
    if(m_buttonOk.isActive(x, y)){
        m_close = true;
    }
    if(m_buttonParcelleNew.isActive(x, y)){
        GpsWidget * g = GpsWidget::Instance();
        g->m_parcelleNewWidget.open();
        g->m_parcelleNewWidget.m_close = false;
    }
    if(m_buttonParcelleLoad.isActive(x, y)){
        GpsWidget * g = GpsWidget::Instance();
        g->m_parcelleLoadWidget.open();
        g->m_parcelleLoadWidget.m_close = false;
    }

}
