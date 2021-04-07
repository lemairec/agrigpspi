#ifndef LINE_WIDGET_H
#define LINE_WIDGET_H

#include "../base_widget.hpp"

class LineWidget : public BaseWidget {
    QPixmap * m_imgOk;
    QPixmap * m_imgRecentre;
    ButtonGui m_buttonOk;
    
    ButtonGui m_buttonParcelleNew;
    ButtonGui m_buttonParcelleLoad;
    ButtonGui m_buttonRecentre;
    
    int m_x = 0;
    int m_lg = 20;
public:
    LineWidget();
    
    void setSize(int width, int height);
    
    virtual void draw();
    virtual void onMouse(int x, int y);
    
    //void open();
    
};

#endif
