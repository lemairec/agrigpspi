#ifndef INFOS_WIDGET_H
#define INFOS_WIDGET_H

#include "base_widget.hpp"

class InfosWidget : public BaseWidget {
    QPixmap * m_imgOk;
    
    int m_type = 0;
public:
    InfosWidget();
    
    ButtonGui m_buttonOk;
    ButtonGui m_buttonDebug;
    
    
    void setSize(int width, int height);
    
    virtual void draw();
    virtual void onMouse(int x, int y);
    
    ValueGui m_ekf;
    ValueGui m_lk;
    ValueGui m_kth;
    void setSize0(int width, int height);
    void drawPage0();
    virtual void onMouse0(int x, int y);

    
};

#endif
