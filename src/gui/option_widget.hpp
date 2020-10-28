#ifndef OPTION_WIDGET_H
#define OPTION_WIDGET_H

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
    QPixmap * m_imgShutDown;
      
    ButtonGui m_button_close;
    
    ButtonGui m_button_p1;
    ButtonGui m_button_p2;
    ButtonGui m_button_p3;
    ButtonGui m_button_p4;
    ButtonGui m_button_p5;
    ButtonGui m_button_p6;
    
    int m_page = 1;
    
    void addSerials();
    
public:
    OptionWidget();
    
    void setSize(int width, int height);
    
    virtual void draw();
    virtual void onMouse(int x, int y);
    
    
    //page1
    ButtonGui m_button_shudown;
    ButtonGui m_button_p1add1;
    ButtonGui m_button_p1add2;
    ButtonGui m_button_p1minus1;
    ButtonGui m_button_p1minus2;
    
    void resizePage1();
    void onMousePage1(int x, int y);
    void drawPage1();
    
    //page2
    ButtonGui m_button_p3add1;
    ButtonGui m_button_p3minus1;
    SelectButtonGui m_select_gps_serial;
    SelectButtonGui m_select_gps_baudrates;
    void resizePage2();
    void onMousePage2(int x, int y);
    void drawPage2();
    
    //page3
    SelectButtonGui m_select_pilot_serial;
    SelectButtonGui m_select_pilot_baudrates;
    SelectButtonGui m_select_pilot_langage;
    
    ValueGui m_value_gui_lookahead_d;
    
    void resizePage3();
    void onMousePage3(double x, double y);
    void drawPage3();
    
    //page4
    ButtonGui m_button_sens;
    ButtonGui m_button_debug;
    
    void resizePage4();
    void onMousePage4(double x, double y);
    void drawPage4();
    
    //page5
    ButtonGui m_button_encoder_inverse;
    ButtonGui m_button_motor_inverse;
     
    ValueGui m_motor_vitesse_min;
    ValueGui m_motor_vitesse_max;
    ValueGui m_motor_vitesse_agressivite;
    
    ValueGui m_button_algo2_goto_pas_by_tour;
    ValueGui m_button_algo2_goto_angle_by_tour;
    ValueGui m_button_algo2_goto_rel_s;
    
    void resizePage5();
    void onMousePage5(double x, double y);
    void drawPage5();
    
    ButtonGui m_button_p6connect;
    ButtonGui m_button_p6disable;
    ButtonGui m_button_p6clearError;
    
    ButtonGui m_button_p6testGoToVLeft;
    ButtonGui m_button_p6testGoToV0;
    ButtonGui m_button_p6testGoToVRight;
    
    ButtonGui m_button_p6test1;
    
    void resizePage6();
    void onMousePage6(double x, double y);
    void drawPage6();
    
    void open();
    
};

#endif
