
#include "parcelle_load_widget.hpp"

#include "../../gps_framework.hpp"
#include "../gps_widget.hpp"


ParcelleLoadWidget::ParcelleLoadWidget(){
    m_imgOk = loadImage("/images/ok.png");
    m_imgCancel = loadImage("/images/cancel.png");
}


void ParcelleLoadWidget::setSize(int width, int height){
    BaseWidget::setSize(width, height);
    
    m_x = 0;
    m_lg = 0.30*m_width;
    m_buttonOk.setResize(2*m_lg/3.0, 0.8*m_height, m_petit_button);
    m_buttonCancel.setResize(m_lg/3.0, 0.8*m_height, m_petit_button);
      
    m_selectParcelles.setResize(m_x+30, 0.25*m_height, m_petit_button);
    m_selectLine.setResize(m_x+30, 0.5*m_height, m_petit_button);
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
   
    /*double x_min = 0, y_min, x_max, y_max;
    for(auto p : parcelle.m_contour){
        if(x_min == 0){
            x_min = p->m_x;
            x_max = p->m_x;
            y_min = p->m_y;
            y_max = p->m_y;
        }
        if(x_min > p->m_x){ x_min = p->m_x; }
        if(x_max < p->m_x){ x_max = p->m_x; }
        if(y_min > p->m_y){ y_min = p->m_y; }
        if(y_max < p->m_y){ y_max = p->m_y; }
    }
    INFO(x_min << " " << x_max << " "<< y_min << " " << y_max);
    {
        double x = m_width*0.7 - (x_min-parcelle.m_center_x)*zoom;
        double y = m_height/2 + (y_min-parcelle.m_center_y)*zoom;
        scene->addEllipse(x, y, 3,3, m_penRed);
    }
    {
        double x = m_width*0.7 - (x_max-parcelle.m_center_x)*zoom;
        double y = m_height/2 + (y_max-parcelle.m_center_y)*zoom;
        scene->addEllipse(x, y, 3,3, m_penRed);
    }
    double w = (parcelle.m_bounding_rect_width)*zoom;
    double h = (parcelle.m_bounding_rect_height)*zoom;
    scene->addRect(m_width*0.7-w/2,  m_height/2-h/2, w,h, m_penBlack);
    */
    
    
    for(auto p : parcelle.m_contour){
        double x = m_width*0.7 - (p->m_x-parcelle.m_center_x)*zoom;
        double y = m_height/2 + (p->m_y-parcelle.m_center_y)*zoom;
        poly.push_back(QPoint(x, y));
    }
    
    
    
    
    
    
    scene->addPolygon(poly, m_penNo, m_brushDarkGray);
    
    
    for(auto f : parcelle.m_flag){
        auto p = parcelle.m_contour[f];
        double x = m_width*0.7 - (p->m_x-parcelle.m_center_x)*zoom;
        double y = m_height/2 + (p->m_y-parcelle.m_center_y)*zoom;
        scene->addEllipse(x-3, y-3, 6, 6, m_penBlack, m_brushRed);
    }
    
    if(m_selectLine.m_selectedValue != 0){
        int i = m_selectLine.m_selectedValue-1;
        if(parcelle.m_flag.size()>1){
            int debut = parcelle.m_flag[i%parcelle.m_flag.size()];
            int fin = parcelle.m_flag[(i+1)%parcelle.m_flag.size()];
            
            auto p1 = parcelle.m_contour[debut];
            auto p2 = parcelle.m_contour[fin];
            
            double x1 = m_width*0.7 - (p1->m_x-parcelle.m_center_x)*zoom;
            double y1 = m_height/2 + (p1->m_y-parcelle.m_center_y)*zoom;
            double x2 = m_width*0.7 - (p2->m_x-parcelle.m_center_x)*zoom;
            double y2 = m_height/2 + (p2->m_y-parcelle.m_center_y)*zoom;
            scene->addLine(x1, y1, x2, y2, m_penRed);
            
        }
    }
    
    QString s = QString::number(parcelle.m_surface_ha)+" ha";
    drawQText(s, m_width*0.45, m_height*0.1);
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
        drawQText(s, m_lg/5, 0.15*m_height, sizeText_big, false);
    }
    
    drawSelectButtonGuiClose(&m_selectParcelles);
    
    if(m_parcelleSelected != 0){
        {
            QString s = "Load lines";
            drawQText(s, m_lg/5, 0.4*m_height, sizeText_big, false);
        }
        drawSelectButtonGuiClose(&m_selectLine);
    }
    
    
    
    drawButtonImage(&m_buttonOk, *m_imgOk);
    drawButtonImage(&m_buttonCancel, *m_imgCancel);
    
    drawSelectButtonGuiOpen(&m_selectParcelles);
    if(m_parcelleSelected != 0){
        drawSelectButtonGuiOpen(&m_selectLine);
    }
    
    
}
void ParcelleLoadWidget::onMouse(int x, int y){
    GpsFramework & f = GpsFramework::Instance();
    if(m_buttonOk.isActive(x, y)){
        f.m_parcelle.loadParcelle(m_selectParcelles.getValueString());
        
        
        if(m_selectLine.m_selectedValue != 0){
           int i = m_selectLine.m_selectedValue-1;
           if(f.m_parcelle.m_flag.size()>1){
               int debut = f.m_parcelle.m_flag[i%f.m_parcelle.m_flag.size()];
               int fin = f.m_parcelle.m_flag[(i+1)%f.m_parcelle.m_flag.size()];
               
               auto p1 = f.m_parcelle.m_contour[debut];
               auto p2 = f.m_parcelle.m_contour[fin];
               
               f.m_line = true;
               f.m_lineAB.m_point_origin_A = *p1;
               f.m_lineAB.m_point_origin_B = *p2;
               f.setAB();
           }
        }
        m_close = true;
    }
    if(m_buttonCancel.isActive(x, y)){
        m_close = true;
    }
    onMouseSelectButton(&m_selectParcelles, x, y);
    if(m_parcelleSelected != m_selectParcelles.m_selectedValue){
        m_parcelleSelected = m_selectParcelles.m_selectedValue;
        m_selectLine.clear();
        m_selectLine.addValue("aucune");
        f.m_parcelle.loadParcelle(m_selectParcelles.getValueString());
        for(size_t i=0; i<f.m_parcelle.m_flag.size(); ++i){
            m_selectLine.addValue("line");
        }
    }
    
    if(m_parcelleSelected != 0){
        onMouseSelectButton(&m_selectLine, x, y);
    }
}

void ParcelleLoadWidget::open(){
    GpsFramework & f = GpsFramework::Instance();
    m_selectParcelles.clear();
    m_selectParcelles.addValue("aucune");
    for(auto n : f.m_parcelles.m_parcelles){
        m_selectParcelles.addValue(n);
    }
}
