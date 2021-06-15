#include "line_load_widget.hpp"

#include "../../gps_framework.hpp"
#include "../gps_widget.hpp"


LineLoadWidget::LineLoadWidget(){
    m_imgOk = loadImage("/images/ok.png");
    m_imgCancel = loadImage("/images/cancel.png");
    m_img_up = loadImage("/images/page_droite.png");
    m_img_down = loadImage("/images/page_gauche.png");
}


void LineLoadWidget::setSize(int width, int height){
    BaseWidget::setSize(width, height);
    
    m_x = 0;
    m_lg = 0.30*m_width;
    
    m_buttonPageDown.setResize(m_lg/3.0, 0.75*m_height, m_petit_button);
    m_buttonPageUp.setResize(m_lg*2.0/3.0, 0.75*m_height, m_petit_button);
    m_buttonCancel.setResize(m_lg/2.0, 0.85*m_height, m_petit_button);
      
    //m_buttonParcelleLoad.setResize(m_x+30, 0.35*m_height, m_petit_button);
}


void LineLoadWidget::draw(){
    m_painter->setPen(m_penBlack);
    m_painter->setBrush(m_brushWhiteAlpha);
    m_painter->drawRect(m_x, m_height*0.1, m_lg, m_height*0.8);
    
    {
        QString s = "Charger une ligne";
        if(m_delete){
            s = "Supprimer une ligne";
        }
        drawQText(s, m_lg/2, 0.15*m_height, sizeText_big, true);
    }
    
    for(int i = 0; i < 6; ++i){
        if(i+(m_page*6) < m_buttons.size()){
            auto b = m_buttons[i+(m_page*6)];
            drawButton(*b);
            drawText(b->m_label, b->m_x + m_petit_button*1.5, b->m_y, sizeText_medium, false);
        }
    }
    
    drawButtonImage(m_buttonPageDown, m_img_down);
    {
        QString s = QString::number(m_page+1)+ "/"+ QString::number(m_nbr_page+1);
        drawQText(s, m_lg/2, m_buttonPageDown.m_y, sizeText_big, true);
    }
    drawButtonImage(m_buttonPageUp, m_img_up);
    
    drawButtonImage(m_buttonCancel, m_imgCancel);
    
}
void LineLoadWidget::onMouse(int x, int y){
    GpsFramework & f = GpsFramework::Instance();
    if(m_buttonCancel.isActive(x, y)){
        m_close = true;
        if(m_delete){
            GpsFramework * f2 = NULL;
            f2->changeDraw();
        }
    }
    if(m_buttonPageUp.isActive(x, y)){
        m_page++;
    }
    if(m_buttonPageDown.isActive(x, y)){
        m_page--;
        
    }
    if(m_page < 0){
        m_page = 0;
    }
    if(m_page > m_nbr_page){
        m_page = m_nbr_page ;
    }
    for(int i = 0; i < 6; ++i){
        if(i+(m_page*6) < m_buttons.size()){
            auto b = m_buttons[i+(m_page*6)];
            if(b->isActive(x, y)){
                if(m_delete){
                    f.m_lines.remove(b->m_label);
                } else {
                    f.m_lines.loadCurveOrLine(b->m_label);
                }
                m_close = true;
            }
        }
    }
    //onMouseSelectButton(m_selectLines, x, y);

}

void LineLoadWidget::open(){
    GpsFramework & f = GpsFramework::Instance();
    m_buttons.clear();//toto fuite memoire
    
    double y = 140;
    m_inter = m_petit_button*2.2;
    
    size_t len =  f.m_lines.m_lines_curves.size();
    m_nbr_page = len/6;
    for(size_t i = 0; i < len; ++i){
        std::string n = f.m_lines.m_lines_curves[len-i-1];
        if(i%6 == 0){
            y = 140;
        }
        ButtonGui * b = new ButtonGui();
        b->setResize(20, y, m_petit_button);
        b->m_labelInt = len-i;
        b->m_label = n;
        m_buttons.push_back(b);
        y += m_inter;
    }

}
