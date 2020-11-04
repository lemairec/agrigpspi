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


class KeyBoardWidget : public BaseWidget {
    int m_x = 0;
    QString m_res;
    double m_lg;
    
    ButtonGui m_buttonA;
    ButtonGui m_buttonB;
    ButtonGui m_buttonC;
    ButtonGui m_buttonD;
    ButtonGui m_buttonE;
    ButtonGui m_buttonF;
    ButtonGui m_buttonG;
    ButtonGui m_buttonH;
    ButtonGui m_buttonI;
    ButtonGui m_buttonJ;
    ButtonGui m_buttonK;
    ButtonGui m_buttonL;
    ButtonGui m_buttonM;
    ButtonGui m_buttonN;
    ButtonGui m_buttonO;
    ButtonGui m_buttonP;
    ButtonGui m_buttonQ;
    ButtonGui m_buttonR;
    ButtonGui m_buttonS;
    ButtonGui m_buttonT;
    ButtonGui m_buttonU;
    ButtonGui m_buttonV;
    ButtonGui m_buttonW;
    ButtonGui m_buttonX;
    ButtonGui m_buttonY;
    ButtonGui m_buttonZ;
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
    ButtonGui m_buttonSpace;
    ButtonGui m_button_ok;
    
    void myDrawButton(ButtonGui *, QString s);
    
    ValueGuiKeyBoard * m_value = NULL;
public:
    KeyBoardWidget();
    
    void setSize(int width, int height);
    
    void draw();
    void onMouse(int x, int y);
    
    void setValueGuiKeyBoard(ValueGuiKeyBoard * value){
        m_value = value;
        m_res = QString();
    }
    
    void addLetter(QString se);
};


#endif
