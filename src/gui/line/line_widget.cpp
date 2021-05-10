
#include "line_widget.hpp"

#include "../../gps_framework.hpp"
#include "../gps_widget.hpp"


LineWidget::LineWidget(){
    m_imgOk = loadImage("/images/ok.png");
    m_imgRecentre = loadImage("/images/ab_middle.png");
    m_img_new = loadImage("/images/new.png");
    m_img_load = loadImage("/images/load.png");
}


void LineWidget::setSize(int width, int height){
    BaseWidget::setSize(width, height);
    m_x = 0;
    m_lg = 0.30*m_width;
    m_buttonOk.setResize(m_lg/2.0, 0.8*m_height, m_petit_button);
    
    m_buttonParcelleNew.setResize(m_x+30, 0.25*m_height, m_petit_button);
    m_buttonParcelleLoad.setResize(m_x+30, 0.35*m_height, m_petit_button);
    m_button_delete.setResize(m_x+30, 0.45*m_height, m_petit_button);
    m_buttonRecentre.setResize(m_x+m_lg/2, 0.55*m_height, m_petit_button);
}


void LineWidget::draw(){
    GpsFramework & f = GpsFramework::Instance();
    
    m_painter->setPen(m_penBlack);
    m_painter->setBrush(m_brushWhiteAlpha);
    m_painter->drawRect(m_x, m_height*0.1, m_lg, m_height*0.8);
    
    {
        QString s = "Ligne";
        drawQText(s, m_lg/2, 0.15*m_height, sizeText_big, true);
    }
    
    {
        drawButtonImage(m_buttonParcelleNew, m_img_new);
        QString s = "Nouvelle ligne";
        drawQText(s, m_buttonParcelleNew.m_x + 25, m_buttonParcelleNew.m_y, sizeText_big, false);
    }
    {
        drawButtonImage(m_buttonParcelleLoad, m_img_load);
        QString s = "Charger";
        drawQText(s, m_buttonParcelleLoad.m_x + 25, m_buttonParcelleLoad.m_y, sizeText_big, false);
    }
    {
        drawButton(m_button_delete);
        QString s = "Supprimer";
        drawQText(s, m_button_delete.m_x + 25, m_button_delete.m_y, sizeText_big, false);
    }
    
    {
        std::string s = "Pas de lignes";
        std::string s1 = "selectionnée";
        if(!f.m_lines.m_line_selected.empty()){
            s = "ligne :";
            s1 = f.m_lines.m_line_selected;
        }
        
        drawText(s, m_x + m_lg/2, m_height*0.6, sizeText_big, true);
        drawText(s1, m_x + m_lg/2, m_height*0.65, sizeText_big, true);
    }
    //drawButtonImage(m_buttonRecentre, m_imgRecentre);
    
    drawButtonImage(m_buttonOk, m_imgOk);
    
}
void LineWidget::onMouse(int x, int y){
    if(m_buttonOk.isActive(x, y)){
        m_close = true;
    }
    if(m_buttonParcelleNew.isActive(x, y)){
        GpsWidget * g = GpsWidget::Instance();
        g->m_lineNewWidget.open();
        g->m_lineNewWidget.m_close = false;
        GpsFramework & f = GpsFramework::Instance();
        f.m_config.m_offset_AB = 0;
        f.initOrLoadConfig();
    }
    if(m_buttonParcelleLoad.isActive(x, y)){
        GpsWidget * g = GpsWidget::Instance();
        g->m_lineLoadWidget.open();
        g->m_lineLoadWidget.m_close = false;
        GpsFramework & f = GpsFramework::Instance();
        f.m_config.m_offset_AB = 0;
        f.initOrLoadConfig();
    }
    if(m_button_delete.isActive(x, y)){
        GpsWidget * g = GpsWidget::Instance();
        g->m_lineLoadWidget.open();
        g->m_lineLoadWidget.m_close = false;
        g->m_lineLoadWidget.m_delete = false;
        GpsFramework & f = GpsFramework::Instance();
        f.m_config.m_offset_AB = 0;
        f.initOrLoadConfig();
    }
    /*if(m_buttonRecentre.isActive(x, y)){
        GpsFramework & f = GpsFramework::Instance();
        f.m_config.m_offset_AB -= f.getOffsetAB();
        f.initOrLoadConfig();
    }*/

}
