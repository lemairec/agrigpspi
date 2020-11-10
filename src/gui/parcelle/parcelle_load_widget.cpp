
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


void ParcelleLoadWidget::my_projete(double x, double y, double & x_res, double & y_res){
    /*double x1_temp = (x - m_xref)*m_zoom;
    double y1_temp = (y - m_yref)*m_zoom;
    
    double h00 = m_cosA, h01 = m_sinA;//, h02 = 1;
    double h10 = -m_sinA, h11 = m_cosA;// h12 = 1;
    //double h20 = -m_sinA, h21 = m_cosA, h22 = 1;
    
    x_res = m_width/2 + x1_temp*h00 + y1_temp*h01;
    y_res  = m_height/2 - x1_temp*h10  - y1_temp*h11;*/
}

void ParcelleLoadWidget::drawParcelle(){
    GpsFramework & f = GpsFramework::Instance();
    Parcelle & parcelle = f.m_parcelle;
    
    double zoom = m_width*0.6/parcelle.m_bounding_rect_width*0.8;
    double zoom2 = m_height*0.9/parcelle.m_bounding_rect_height*0.8;
    if(zoom2 <zoom){
        zoom = zoom2;
    }
    
    QPolygon poly;
    for(auto p : parcelle.m_contour){
        double x = (p->m_x-parcelle.m_center_x)*zoom + m_width*0.7;
        double y = (p->m_y-parcelle.m_center_y)*zoom + m_height/2;
        poly.push_back(QPoint(x, y));
    }
    scene->addPolygon(poly, m_penNo, m_brushDarkGray);
    
}

void ParcelleLoadWidget::draw(){
    scene->addRect(m_x, 0, m_width, m_height, m_penBlack, m_brushWhite);
    
    scene->addRect(m_width*0.4, m_height*0.05, m_width*0.6-m_height*0.05, m_height*0.9, m_penBlack, m_parcelleBrush);
    //scene->addRect(m_width*0.2, m_height*0.1, m_width*0.08, m_height*0.8, m_penBlack, m_brushDarkGray);
    
    GpsFramework & f = GpsFramework::Instance();
    f.m_parcelle.loadParcelle(m_selectParcelles.getValueString());
    drawParcelle();
    
    
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
