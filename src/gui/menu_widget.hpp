#ifndef MENU_WIDGET_H
#define MENU_WIDGET_H

#include "base_widget.hpp"
#include "machine_widget.hpp"
#include "option_widget.hpp"

class MenuWidget : public BaseWidget {
    QPixmap * m_imgOk;
    QPixmap * m_imgParam;
    QPixmap * m_imgTractor;
    QPixmap * m_imgOutil;
    QPixmap * m_imgStop;
    
    ButtonGui m_buttonOk;
    ButtonGui m_buttonParam;
    ButtonGui m_buttonTractor;
    ButtonGui m_buttonOutil;
    ButtonGui m_buttonStop;
    
    int m_x = 0;
    int m_lg = 0;
public:
    MachineWidget m_machine_widget;
    OutilWidget m_outil_widget;
    OptionWidget m_options_widget;
    
    
    MenuWidget();
    
    void setSize(int width, int height);
    void setScene(QGraphicsScene * s);
    
    virtual void draw();
    virtual void onMouse(int x, int y);
    
};

#endif
