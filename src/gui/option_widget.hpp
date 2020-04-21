#pragma once

#include "base_widget.hpp"

class OptionWidget : public BaseWidget {
    ButtonGui * m_button_close = NULL;
    
    ButtonGui * m_button_p1 = NULL;
    ButtonGui * m_button_p2 = NULL;
    ButtonGui * m_button_p3 = NULL;
    ButtonGui * m_button_p4 = NULL;
    
    int m_page = 1;
    
    //page1
    ButtonGui * m_button_p1add1;
    ButtonGui * m_button_p1add2;
    ButtonGui * m_button_p1add3;
    ButtonGui * m_button_p1minus1;
    ButtonGui * m_button_p1minus2;
    ButtonGui * m_button_p1minus3;
    
    ButtonGui * m_button_sens;
    ButtonGui * m_button_debug;
    
    
    std::list<ButtonGui *> m_buttonp2Serials;
    std::list<ButtonGui *> m_buttonp2Baurates;
    
    //page2
    ButtonGui * m_button_p3add1;
    ButtonGui * m_button_p3minus1;
    
    ButtonGui * m_button_p3test1;
    ButtonGui * m_button_p3test2;
    ButtonGui * m_button_p3test3;
    
    //page3
    ValueGui * m_kp;
    ValueGui * m_ki;
    ValueGui * m_kd;
    ValueGui * m_kvitesse;
    
    std::list<ButtonGui *> m_buttonp3Serials;
    std::list<ButtonGui *> m_buttonp3Baurates;
    
    void addSerials();
    
public:
    OptionWidget();
    
    virtual void draw();
    virtual void drawPage1();
    virtual void drawPage2();
    virtual void drawPage3();
    virtual void drawPage4();
    
    virtual void onMouse(double x, double y);
    virtual void onMousePage1(double x, double y);
    virtual void onMousePage2(double x, double y);
    virtual void onMousePage3(double x, double y);
    virtual void onMousePage4(double x, double y);
    
    void open();
    
};
