#ifndef MACHINE_WIDGET_H
#define MACHINE_WIDGET_H

#include "base_widget.hpp"

class MachineWidget : public BaseWidget {
    QPixmap * m_imgOk;
    QPixmap * m_imgMachine;
     
    int m_x = 0;
    int m_lg = 0;
public:
    MachineWidget();
    
    ButtonGui m_buttonOk;
    ValueGuiKeyPad m_valueA;
    ValueGuiKeyPad m_valueB;
    ValueGuiKeyPad m_valueC;
    
    void setSize(int width, int height);
    
    void open();
    void draw();
    void onMouse(int x, int y);
    
};

class OutilWidget : public BaseWidget {
    QPixmap * m_imgOk;
    QPixmap * m_imgOutil1;
    QPixmap * m_imgOutil2;
    
    int m_x = 0;
    int m_lg = 0;
public:
    OutilWidget();
    
    ButtonGui m_buttonOk;
    ValueGuiKeyPad m_valueA;
    ValueGuiKeyPad m_valueB;
    
    void setSize(int width, int height);
    
    void open();
    void draw();
    void onMouse(int x, int y);
    
};

#endif
