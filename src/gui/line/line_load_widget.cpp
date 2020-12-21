#include "line_load_widget.hpp"

#include "../../gps_framework.hpp"
#include "../gps_widget.hpp"


LineLoadWidget::LineLoadWidget(){
    m_imgOk = loadImage("/images/ok.png");
    m_imgCancel = loadImage("/images/cancel.png");
}


void LineLoadWidget::setSize(int width, int height){
    BaseWidget::setSize(width, height);
    
    m_x = 0;
    m_lg = 0.30*m_width;
    m_buttonOk.setResize(2*m_lg/3.0, 0.8*m_height, m_petit_button);
    m_buttonCancel.setResize(m_lg/3.0, 0.8*m_height, m_petit_button);
      
    m_selectLines.setResize(m_x+30, 0.25*m_height, m_petit_button);
    //m_buttonParcelleLoad.setResize(m_x+30, 0.35*m_height, m_petit_button);
    
    GpsFramework & f = GpsFramework::Instance();
    f.m_lines.loadCurveOrLine("curve_CURVE_MINI");
}


void LineLoadWidget::draw(){
    m_painter->setPen(m_penBlack);
    m_painter->setBrush(m_brushWhiteAlpha);
    m_painter->drawRect(m_x, m_height*0.1, m_lg, m_height*0.8);
    
    {
        QString s = "load ligne";
        drawQText(s, m_lg/2, 0.15*m_height, sizeText_big, true);
    }
    
    drawSelectButtonGuiClose(&m_selectLines);
    drawSelectButtonGuiOpen(&m_selectLines);
    
    
    drawButtonImage(&m_buttonOk, *m_imgOk);
    drawButtonImage(&m_buttonCancel, *m_imgCancel);
    
}
void LineLoadWidget::onMouse(int x, int y){
    GpsFramework & f = GpsFramework::Instance();
    if(m_buttonOk.isActive(x, y)){
        f.m_lines.loadCurveOrLine(m_selectLines.getValueString());
        m_close = true;
    }
    if(m_buttonCancel.isActive(x, y)){
        m_close = true;
    }
    onMouseSelectButton(&m_selectLines, x, y);

}

void LineLoadWidget::open(){
    GpsFramework & f = GpsFramework::Instance();
    m_selectLines.clear();
    m_selectLines.addValue("aucune");
    for(auto n : f.m_lines.m_lines_curves){
        m_selectLines.addValue(n);
    }

}
