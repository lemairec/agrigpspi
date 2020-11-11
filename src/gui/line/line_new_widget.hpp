#ifndef LINE_NEW_WIDGET_H
#define LINE_NEW_WIDGET_H

#include "../base_widget.hpp"

class LineNewWidget : public BaseWidget {
    QPixmap * m_imgOk;
    QPixmap * m_imgLigneAB;
    QPixmap * m_imgCurveAB;
    QPixmap * m_imgA;
    QPixmap * m_imgB;
    
    ButtonGui m_buttonOk;
    ButtonGui m_buttonLigneCurve;
    ButtonGui m_buttonAB;
   
    ValueGuiKeyBoard m_name;
    
    int m_x = 0;
    int m_lg = 20;
public:
    LineNewWidget();
    
    void setSize(int width, int height);
    
    virtual void draw();
    virtual void onMouse(int x, int y);
    
    void open();
    
};

#endif
