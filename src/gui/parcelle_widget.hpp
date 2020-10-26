#ifndef PARCELLE_WIDGET_H
#define PARCELLE_WIDGET_H

#include "base_widget.hpp"

class ParcelleWidget : public BaseWidget {
    QPixmap * m_imgOk;
    ButtonGui m_buttonOk;
    
    int m_x = 0;
    int m_lg = 20;
public:
    ParcelleWidget();
    
    void setSize(int width, int height);
    
    virtual void draw();
    virtual void onMouseInt(int x, int y);
    
    void open();
    
};

#endif
