#ifndef PARCELLE_LOAD_WIDGET_H
#define PARCELLE_LOAD_WIDGET_H

#include "../base_widget.hpp"

class ParcelleLoadWidget : public BaseWidget {
    QPixmap * m_imgOk;
    ButtonGui m_buttonOk;
    QPixmap * m_imgCancel;
    ButtonGui m_buttonCancel;
    
    SelectButtonGui m_selectParcelles;
    
    int m_x = 0;
    int m_lg = 20;
public:
    ParcelleLoadWidget();
    
    void setSize(int width, int height);
    
    virtual void draw();
    virtual void onMouse(int x, int y);
    
    void open();
    
};

#endif
