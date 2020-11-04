#ifndef PARCELLE_NEW_WIDGET_H
#define PARCELLE_NEW_WIDGET_H

#include "../base_widget.hpp"

class ParcelleNewWidget : public BaseWidget {
    QPixmap * m_imgOk;
    ButtonGui m_buttonOk;
    
    ButtonGui m_buttonParcelleStart;
    ButtonGui m_buttonParcellePause;
    
    ValueGuiKeyBoard m_name;
    
    int m_x = 0;
    int m_lg = 20;
public:
    ParcelleNewWidget();
    
    void setSize(int width, int height);
    
    virtual void draw();
    virtual void onMouse(int x, int y);
    
    void open();
    
};

#endif
