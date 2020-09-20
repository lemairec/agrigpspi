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
      
    ButtonGui * m_button_close = NULL;
    
    ButtonGui * m_button_p1 = NULL;
    ButtonGui * m_button_p2 = NULL;
    ButtonGui * m_button_p3 = NULL;
    ButtonGui * m_button_p4 = NULL;
    ButtonGui * m_button_p5 = NULL;
    ButtonGui * m_button_p6 = NULL;
    
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
    
    
    
    //page 4
    ButtonGui * m_button_sens;
    ButtonGui * m_button_debug;
    
    
    
    
    
    void addSerials();
    
public:
    OptionWidget();
    
    virtual void draw();
    virtual void drawPage1();
    virtual void drawPage2();
    
    virtual void onMouse(double x, double y);
    virtual void onMousePage1(double x, double y);
    virtual void onMousePage2(double x, double y);
    
    //page3
    SelectButtonGui * m_select_pilot_serial;
    SelectButtonGui * m_select_pilot_baudrates;
    SelectButtonGui * m_select_pilot_langage;
    ValueGui * m_vitesse_motor;
    
    void setPage3();
    void onMousePage3(double x, double y);
    void drawPage3();
    
    void setPage4();
    void onMousePage4(double x, double y);
    void drawPage4();
    
    //page5
    ButtonGui * m_button_inverse;
    SelectButtonGui * m_button_select_algo;
    
    ValueGui * m_button_algo_lookahead_d;
    
    SelectButtonGui * m_button_select_algo2;
    
    ValueGui * m_button_algo2_goto_pas_by_tour;
    ValueGui * m_button_algo2_goto_angle_by_tour;
    ValueGui * m_button_algo2_goto_rel_s;
    ValueGui * m_button_algo2_pid_kp;
    ValueGui * m_button_algo2_pid_ki;
    ValueGui * m_button_algo2_pid_kd;
    ValueGui * m_button_algo2_my_k;
    ValueGui * m_button_algo2_my_p;
    
    void setPage5();
    void onMousePage5(double x, double y);
    void drawPage5();
    
    ButtonGui * m_button_p6connect;
    ButtonGui * m_button_p6disable;
    ButtonGui * m_button_p6clearError;
    
    ButtonGui * m_button_p6testLeft;
    ButtonGui * m_button_p6testRight;
    
    ButtonGui * m_button_p6testGoToVLeft;
    ButtonGui * m_button_p6testGoToV0;
    ButtonGui * m_button_p6testGoToVRight;
    
    ButtonGui * m_button_p6testGoToDLeft;
    ButtonGui * m_button_p6testGoToD0;
    ButtonGui * m_button_p6testGoToDRight;
    
    void setPage6();
    void onMousePage6(double x, double y);
    void drawPage6();
    
    void open();
    
};

#endif
