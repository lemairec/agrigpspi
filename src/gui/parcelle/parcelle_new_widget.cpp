
#include "parcelle_new_widget.hpp"

#include "../../gps_framework.hpp"
#include "../gps_widget.hpp"


ParcelleNewWidget::ParcelleNewWidget(){
    m_imgOk = loadImage("/images/ok.png");
}


void ParcelleNewWidget::setSize(int width, int height){
    BaseWidget::setSize(width, height);
    
    m_x = 0;
    m_lg = 0.30*m_width;
    m_buttonOk.setResize(m_lg/2.0, 0.8*m_height, m_petit_button);
    
    m_name.setResize(m_x + m_lg*3/4, 0.35*m_height);
    m_buttonParcelleStartPause.setResize(m_lg*1/4, 0.5*m_height, m_petit_button);
    
}

void ParcelleNewWidget::drawParcelle(){
    GpsFramework & f = GpsFramework::Instance();
    Parcelle & parcelle = f.m_parcelle;
    parcelle.compute();
    
    double zoom = m_width*0.6/parcelle.m_bounding_rect_width*0.8;
    double zoom2 = m_height*0.9/parcelle.m_bounding_rect_height*0.8;
    if(zoom2 <zoom){
        zoom = zoom2;
    }
    if (zoom>4){
        zoom = 4;
    }
    
    QPolygon poly;
    for(auto p : parcelle.m_contour){
        double x = -(p->m_x-parcelle.m_center_x)*zoom + m_width*0.7;
        double y = (p->m_y-parcelle.m_center_y)*zoom + m_height/2;
        poly.push_back(QPoint(x, y));
    }
    scene->addPolygon(poly, m_penBlack, m_brushDarkGray);
    
    QString s = QString::number(parcelle.m_surface_ha)+" ha";
    drawQText(s, m_width*0.45, m_height*0.1);
}

/*void GpsWidget::my_projete2(double x, double y, double & x_res, double & y_res){
    double x1_temp = (x - m_xref)*m_zoom;
    double y1_temp = (y - m_yref)*m_zoom;
    
    double h00 = m_cosA, h01 = m_sinA;//, h02 = 1;
    double h10 = -m_sinA, h11 = m_cosA;// h12 = 1;
    //double h20 = -m_sinA, h21 = m_cosA, h22 = 1;
    
    x_res = m_width/2 + x1_temp*h00 + y1_temp*h01;
    y_res  = m_height/2 - x1_temp*h10  - y1_temp*h11;
}*/


void ParcelleNewWidget::draw(){
    GpsFramework & f = GpsFramework::Instance();
    scene->addRect(m_x, 0, m_width, m_height, m_penBlack, m_brushWhite);
    
    scene->addRect(m_width*0.4, m_height*0.05, m_width*0.6-m_height*0.05, m_height*0.9, m_penBlack, m_parcelleBrush);
    drawParcelle();
    //scene->addRect(m_width*0.2, m_height*0.1, m_width*0.08, m_height*0.8, m_penBlack, m_brushDarkGray);
    
    {
        QString s = "Nouvelle Parcelle";
        drawQText(s, m_lg/2, 0.15*m_height, sizeText_big, true);
    }
    
    {
        QString s = "Nom de la parcelle : ";
        drawQText(s, m_x+30, 0.25*m_height, sizeText_big, false);
    }
    drawValueGuiKeyBoard(&m_name);
    {
        QString s = "Play";
        if(f.m_etat == Etat_ParcelleAdd){
            s = "Pause";
        }
        drawQText(s, m_buttonParcelleStartPause.m_x+30, m_buttonParcelleStartPause.m_y, sizeText_big, false);
        drawButton(&m_buttonParcelleStartPause);
        
    }
    
    drawButtonImage(&m_buttonOk, *m_imgOk);
    
}
void ParcelleNewWidget::onMouse(int x, int y){
    GpsFramework & f = GpsFramework::Instance();
    if(m_buttonOk.isActive(x, y)){
        m_close = true;
        f.m_etat = Etat_OK;
        f.m_parcelle.m_name = m_name.m_text;
        f.m_parcelle.compute();
        f.m_parcelles.add(f.m_parcelle);
        
    }
    if(m_buttonParcelleStartPause.isActive(x, y)){
        if(f.m_etat == Etat_ParcelleAdd){
            f.m_etat = Etat_OK;
        } else {
            f.m_etat = Etat_ParcelleAdd;
        }
    }
    if(isActiveValueGuiKeyBoard(&m_name,x,y)){
        m_key_board_widget->m_close = false;
        m_key_board_widget->setValueGuiKeyBoard(&m_name);
    }
}
