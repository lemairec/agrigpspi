#include "keyboard_widget.hpp"

void KeyPadWidget::setSize(int width, int height){
    BaseWidget::setSize(width, height);
    
    m_x = m_width*0.35;
    m_lg = m_width*0.3;
    
    int inter = m_width*0.08;
    int x = m_x+m_lg/2;
    int rayon = m_gros_button;
    
    int y = m_height*0.5- inter;
    m_button7.setResize(x-inter, y, rayon);
    m_button8.setResize(x, y, rayon);
    m_button9.setResize(x+inter, y, rayon);
    
    y = m_height*0.5;
    m_button4.setResize(x-inter, y, rayon);
    m_button5.setResize(x, y, rayon);
    m_button6.setResize(x+inter, y, rayon);
    
    y = m_height*0.5 + inter;
    m_button1.setResize(x-inter, y, rayon);
    m_button2.setResize(x, y, rayon);
    m_button3.setResize(x+inter, y, rayon);
    
    y = m_height*0.5 + 2*inter;
    m_button0.setResize(x-inter, y, rayon);
    m_button_point.setResize(x, y, rayon);
    m_button_ok.setResize(x+inter, y, rayon);
    
}

void KeyPadWidget::myDrawButton(ButtonGui * b, QString s){
    drawButton(b);
    drawQText(s, b->m_x, b->m_y, sizeText_medium, true);
}


void KeyPadWidget::draw(){
    scene->addRect(m_x, m_height*0.1, m_width*0.3, m_height*0.8, m_penBlack, m_brushWhiteAlpha);
    
    QString s = "KeyPad";
    drawQText(s, m_x+m_lg/2, 0.15*m_height, sizeText_big, true);
    
    s = m_res;
    drawQText(s, m_x+m_lg/2, 0.25*m_height, sizeText_big, true);
    
    myDrawButton(&m_button1, "1");
    myDrawButton(&m_button2, "2");
    myDrawButton(&m_button3, "3");
    myDrawButton(&m_button4, "4");
    myDrawButton(&m_button5, "5");
    myDrawButton(&m_button6, "6");
    myDrawButton(&m_button7, "7");
    myDrawButton(&m_button8, "8");
    myDrawButton(&m_button9, "9");
    myDrawButton(&m_button0, "0");
    myDrawButton(&m_button_point, ".");
    myDrawButton(&m_button_ok, "ok");
}

void KeyPadWidget::onMouse(int x, int y){
    if(m_button0.isActive(x, y)){
        addChiffre("0");
    } else if(m_button1.isActive(x, y)){
        addChiffre("1");
    } else if(m_button2.isActive(x, y)){
        addChiffre("2");
    } else if(m_button3.isActive(x, y)){
        addChiffre("3");
    } else if(m_button4.isActive(x, y)){
        addChiffre("4");
    } else if(m_button5.isActive(x, y)){
        addChiffre("5");
    } else if(m_button6.isActive(x, y)){
        addChiffre("6");
    } else if(m_button7.isActive(x, y)){
        addChiffre("7");
    } else if(m_button8.isActive(x, y)){
        addChiffre("8");
    } else if(m_button9.isActive(x, y)){
        addChiffre("9");
    } else if(m_button_point.isActive(x, y)){
        addChiffre(".");
    } else if(m_button_ok.isActive(x, y)){
        double res = m_res.toFloat();
        if(m_value){
            m_value->m_value = res;
        }
        m_close = true;
    }
};

void KeyPadWidget::addChiffre(QString s){
    m_res = m_res + s;
}

