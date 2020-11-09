
#include "parcelle_load_widget.hpp"

#include "../../gps_framework.hpp"
#include "../gps_widget.hpp"


ParcelleLoadWidget::ParcelleLoadWidget(){
    m_imgOk = loadImage("/images/ok.png");
    m_imgCancel = loadImage("/images/ok.png");
}


void ParcelleLoadWidget::setSize(int width, int height){
    BaseWidget::setSize(width, height);
    
    m_x = 0;
    m_lg = 0.30*m_width;
    m_buttonOk.setResize(2*m_lg/3.0, 0.8*m_height, m_petit_button);
    m_buttonCancel.setResize(m_lg/3.0, 0.8*m_height, m_petit_button);
      
    m_selectParcelles.setResize(m_x+30, 0.25*m_height, m_petit_button);
    //m_buttonParcelleLoad.setResize(m_x+30, 0.35*m_height, m_petit_button);
}


void ParcelleLoadWidget::draw(){
    scene->addRect(m_x, m_height*0.1, m_lg, m_height*0.8, m_penBlack, m_brushWhiteAlpha);
    //scene->addRect(m_width*0.2, m_height*0.1, m_width*0.08, m_height*0.8, m_penBlack, m_brushDarkGray);
    
    {
        QString s = "Load parcelles";
        drawQText(s, m_lg/2, 0.15*m_height, sizeText_big, true);
    }
    
    drawSelectButtonGuiClose(&m_selectParcelles);
    drawSelectButtonGuiOpen(&m_selectParcelles);
    
    
    drawButtonImage(&m_buttonOk, *m_imgOk);
    drawButton(&m_buttonCancel);
    
}
void ParcelleLoadWidget::onMouse(int x, int y){
    GpsFramework & f = GpsFramework::Instance();
    if(m_buttonOk.isActive(x, y)){
        f.m_parcelle.loadParcelle(m_selectParcelles.getValueString());
        m_close = true;
    }
    if(m_buttonCancel.isActive(x, y)){
        m_close = true;
    }
    onMouseSelectButton(&m_selectParcelles, x, y);

}

void ParcelleLoadWidget::open(){
    GpsFramework & f = GpsFramework::Instance();
    m_selectParcelles.clear();
    m_selectParcelles.addValue("aucune");
    for(auto n : f.m_parcelles.m_parcelles){
        m_selectParcelles.addValue(n);
    }

}
