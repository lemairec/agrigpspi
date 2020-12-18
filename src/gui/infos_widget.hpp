#ifndef INFOS_WIDGET_H
#define INFOS_WIDGET_H

#include "base_widget.hpp"

class InfosWidget : public BaseWidget {
    QPixmap * m_imgOk;
    
    int m_type = 3;
public:
    InfosWidget();
    
    ButtonGui m_buttonOk;
    ButtonGui m_buttonDebug;
    
    
    void setSize(int width, int height);
    
    virtual void draw();
    virtual void onMouse(int x, int y);
    
};

#endif
