#ifndef GUIDAGE_WIDGET_H
#define GUIDAGE_WIDGET_H

#include "base_widget.hpp"

class GuidageWidget : public BaseWidget {
    QPixmap * m_imgOk;
    QPixmap * m_imgLigneAB;
    QPixmap * m_imgCurveAB;
    QPixmap * m_imgA;
    QPixmap * m_imgB;
    
    ButtonGui m_buttonOk;
    ButtonGui m_buttonLigneCurve;
    ButtonGui m_buttonAB;
     
    int m_x = 0;
    int m_lg = 20;
public:
    GuidageWidget();
    
    void setSize(int width, int height);
    
    virtual void draw();
    virtual void onMouse(int x, int y);
    
    void open();
    
};

#endif
