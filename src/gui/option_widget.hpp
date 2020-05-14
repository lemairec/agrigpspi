#pragma once

#include "base_widget.hpp"

class OptionWidget : public BaseWidget {
    QPixmap * m_imgClose;
    QPixmap * m_imgPlus;
    QPixmap * m_imgMinus;
    
    QPixmap * m_imgSatBlanc;
    QPixmap * m_imgSatGris;
    QPixmap * m_imgVolantBlanc;
    QPixmap * m_imgVolantGris;
    QPixmap * m_imgOutilBlanc;
    QPixmap * m_imgOutilGris;
    QPixmap * m_imgOptionBlanc;
    QPixmap * m_imgOptionGris;
      
    ButtonGui * m_button_close = NULL;
    
    ButtonGui * m_button_p1 = NULL;
    ButtonGui * m_button_p2 = NULL;
    ButtonGui * m_button_p3 = NULL;
    ButtonGui * m_button_p4 = NULL;
    ButtonGui * m_button_p5 = NULL;
    
    int m_page = 1;
    double y_baurates_title, y_port_title;
    
    //page1
    ButtonGui * m_button_p1add1;
    ButtonGui * m_button_p1add2;
    ButtonGui * m_button_p1minus1;
    ButtonGui * m_button_p1minus2;
    
    std::list<ButtonGui *> m_buttonp2Serials;
    std::list<ButtonGui *> m_buttonp2Baurates;
    
    //page2
    ButtonGui * m_button_p3add1;
    ButtonGui * m_button_p3minus1;
    
    //page3
    std::list<ButtonGui *> m_buttonp3Serials;
    std::list<ButtonGui *> m_buttonp3Baurates;
    
    //page 4
    ButtonGui * m_button_sens;
    ButtonGui * m_button_debug;
    
    
    //page5
    ButtonGui * m_button_algo1;
    ButtonGui * m_button_algo2;
    SelectButtonGui * m_select_button;
    
    ValueGui * m_button_algo1kp;
    ValueGui * m_button_algo1kd;
    
    ValueGui * m_button_algo2k;
    
    ButtonGui * m_button_p5test1;
    ButtonGui * m_button_p5test3;
    
    
    void addSerials();
    
public:
    OptionWidget();
    
    virtual void draw();
    virtual void drawPage1();
    virtual void drawPage2();
    virtual void drawPage3();
    virtual void drawPage4();
    virtual void drawPage5();
    
    virtual void onMouse(double x, double y);
    virtual void onMousePage1(double x, double y);
    virtual void onMousePage2(double x, double y);
    virtual void onMousePage3(double x, double y);
    virtual void onMousePage4(double x, double y);
    virtual void onMousePage5(double x, double y);
    
    void open();
    
};
