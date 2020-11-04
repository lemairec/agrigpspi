#ifndef KEYBOARD_WIDGET_H
#define KEYBOARD_WIDGET_H

#include "../gps_framework.hpp"
#include "base_widget.hpp"

class KeyPadWidget : public BaseWidget {
    int m_x = 0;
    QString m_res;
    double m_lg;
    
    ButtonGui m_button0;
    ButtonGui m_button1;
    ButtonGui m_button2;
    ButtonGui m_button3;
    ButtonGui m_button4;
    ButtonGui m_button5;
    ButtonGui m_button6;
    ButtonGui m_button7;
    ButtonGui m_button8;
    ButtonGui m_button9;
    ButtonGui m_button_point;
    ButtonGui m_button_ok;
    
    void myDrawButton(ButtonGui *, QString s);
    
    ValueGuiKeyPad * m_value = NULL;
public:
    void setSize(int width, int height);
    
    void draw();
    void onMouse(int x, int y);
    
    void setValueGuiKeyPad(ValueGuiKeyPad * value){
        m_value = value;
        m_res = QString();
    }
    
    void addChiffre(QString se);
};


class KeyBoard {
    int m_x = 0;
    QString m_res;
    double m_lg;
    
    ButtonGui m_buttonA;
    ButtonGui m_buttonB;
    ButtonGui m_buttonC;
    ButtonGui m_button0;
    ButtonGui m_button1;
    ButtonGui m_button2;
    ButtonGui m_button3;
    ButtonGui m_button4;
    ButtonGui m_button5;
    ButtonGui m_button6;
    ButtonGui m_button7;
    ButtonGui m_button8;
    ButtonGui m_button9;
    ButtonGui m_button_point;
    ButtonGui m_button_ok;
    
    void myDrawButton(ButtonGui *, QString s);
    
    ValueGuiKeyPad * m_value = NULL;
public:
    void setSize(int width, int height);
    
    void draw();
    void onMouse(int x, int y);
    
    void setValueGuiKeyPad(ValueGuiKeyPad * value){
        m_value = value;
        m_res = QString();
    }
    
    void addChiffre(QString se);
};


#endif
