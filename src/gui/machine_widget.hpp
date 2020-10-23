#ifndef MACHINE_WIDGET_H
#define MACHINE_WIDGET_H

#include "base_widget.hpp"

class MachineWidget : public BaseWidget {
    QPixmap * m_imgOk;
    
    bool m_debug = false;
    int m_x = 0;
    int m_lg = 0;
public:
    MachineWidget();
    
    ButtonGui m_buttonOk;
    ValueGuiKeyPad m_valueA;
    ValueGuiKeyPad m_valueB;
    
    void setSize(int width, int height);
    
    virtual void draw();
    virtual void onMouseInt(int x, int y);
    
};

#endif
