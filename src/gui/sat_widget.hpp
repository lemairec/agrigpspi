#ifndef SAT_WIDGET_H
#define SAT_WIDGET_H

#include "base_widget.hpp"

class SatWidget : public BaseWidget {
    QPixmap * m_imgOk;
    
    bool m_debug = false;
    int m_x = 0;
public:
    SatWidget();
    
    ButtonGui m_buttonOk;
    ButtonGui m_buttonDebug;
    
    
    void setSize(int width, int height);
    
    virtual void draw();
    virtual void onMouse(int x, int y);
    
};

#endif
