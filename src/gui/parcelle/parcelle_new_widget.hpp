#ifndef PARCELLE_NEW_WIDGET_H
#define PARCELLE_NEW_WIDGET_H

#include "../base_widget.hpp"

class ParcelleNewWidget : public BaseWidget {
    QPixmap * m_imgOk;
    QPixmap * m_imgCancel;
    ButtonGui m_buttonOk;
    ButtonGui m_buttonCancel;
    
    ButtonGui m_buttonParcelleStartPause;
    
    ValueGuiKeyBoard m_name;
    
    int m_x = 0;
    int m_lg = 20;
public:
    ParcelleNewWidget();
    
    void setSize(int width, int height);
    
    void drawParcelle();
    virtual void draw();
    virtual void onMouse(int x, int y);
    
    void open();
    
};

#endif
