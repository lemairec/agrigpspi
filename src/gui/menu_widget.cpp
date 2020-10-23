#include "menu_widget.hpp"

#include "../gps_framework.hpp"

MenuWidget::MenuWidget(){
    m_imgOk = loadImage("/images/ok.png");
    m_imgStop = loadImage("/images/close.png");
    m_imgOutil = loadImage("/images/outil_gris.png");
    m_imgTractor = loadImage("/images/outil_gris.png");
    m_imgParam = loadImage("/images/option_gris.png");
    
}

void MenuWidget::setScene(QGraphicsScene * s){
    BaseWidget::setScene(s);
    m_machine_widget.setScene(s);
    m_optionsWidget.setScene(scene);
    
}

void MenuWidget::setSize(int width, int height){
    BaseWidget::setSize(width, height);
    m_machine_widget.setSize(width, height);
    m_optionsWidget.setSize(m_width, m_height);
    
    m_x = m_width*0.0;
    m_lg = m_width*1;
    m_buttonOk.setResize(m_x+m_lg/2, 0.7*m_height, m_gros_gros_button);
    
    m_buttonTractor.setResize(m_x+m_lg/5, 0.5*m_height, m_gros_gros_button);
    m_buttonOutil.setResize(m_x+2*m_lg/5, 0.5*m_height, m_gros_gros_button);
    m_buttonParam.setResize(m_x+3*m_lg/5, 0.5*m_height, m_gros_gros_button);
    m_buttonStop.setResize(m_x+4*m_lg/5, 0.5*m_height, m_gros_gros_button);
    
    
}


void MenuWidget::draw(){
    if(!m_machine_widget.m_close){
        m_machine_widget.draw();
    } else if(!m_optionsWidget.m_close){
        m_optionsWidget.draw();
    } else {
    
        auto last_frame = GpsFramework::Instance().m_lastGGAFrame;
        scene->addRect(m_x, m_height*0.15, m_lg, m_height*0.7, m_penBlack, m_brushWhiteAlpha);
        //scene->addRect(m_width*0.2, m_height*0.1, m_width*0.08, m_height*0.8, m_penBlack, m_brushDarkGray);
        
        int x2 = m_x+30;
        {
            QString s = "Menu";
            drawQText(s, m_x + m_lg/2, 0.25*m_height, sizeText_big, true);
        }

        drawButtonImage(&m_buttonOk, *m_imgOk);
        drawButtonImage(&m_buttonTractor, *m_imgTractor);
        drawButtonImage(&m_buttonOutil, *m_imgOutil);
        drawButtonImage(&m_buttonParam, *m_imgParam);
        drawButtonImage(&m_buttonStop, *m_imgStop);
    
    }
}
void MenuWidget::onMouseInt(int x, int y){
    if(!m_machine_widget.m_close){
        m_machine_widget.onMouseInt(x, y);
    } else if(!m_optionsWidget.m_close){
        m_optionsWidget.onMouseInt(x, y);
    } else {
    
        if(m_buttonOk.isActive(x, y)){
            m_close = true;
        } else if(m_buttonStop.isActive(x, y)){
            exit(0);
        } else if(m_buttonTractor.isActive(x, y)){
            m_machine_widget.m_close = false;
        } else if(m_buttonParam.isActive(x, y)){
            m_optionsWidget.open();
            m_optionsWidget.m_close = false;
        }
    }
   
    
}
