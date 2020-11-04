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


/*if(m_buttonA.isActive(x, y)){
 addLetter("A");
 } else if(m_buttonB.isActive(x, y)){
 addLetter("B");
 }*/

void KeyBoardWidget::setSize(int width, int height){
    BaseWidget::setSize(width, height);
    
    m_x = m_width*0.05;
    m_lg = m_width*0.9;
    
    int inter = m_width*0.07;
    int rayon = m_gros_button;
    
    int y1 = m_height*0.5 - 2*inter;
    int y = y1+inter;
    int x = m_x+m_lg/2-4.5*inter;
    m_button1.setResize(x, y, rayon);
    m_button2.setResize(x+inter, y, rayon);
    m_button3.setResize(x+2*inter, y, rayon);
    m_button4.setResize(x+3*inter, y, rayon);
    m_button5.setResize(x+4*inter, y, rayon);
    m_button6.setResize(x+5*inter, y, rayon);
    m_button7.setResize(x+6*inter, y, rayon);
    m_button8.setResize(x+7*inter, y, rayon);
    m_button9.setResize(x+8*inter, y, rayon);
    m_button0.setResize(x+9*inter, y, rayon);
    
    y = y1+2*inter;
    x = m_x+m_lg/2-4.5*inter;
    m_buttonA.setResize(x, y, rayon);
    m_buttonZ.setResize(x+inter, y, rayon);
    m_buttonE.setResize(x+2*inter, y, rayon);
    m_buttonR.setResize(x+3*inter, y, rayon);
    m_buttonT.setResize(x+4*inter, y, rayon);
    m_buttonY.setResize(x+5*inter, y, rayon);
    m_buttonU.setResize(x+6*inter, y, rayon);
    m_buttonI.setResize(x+7*inter, y, rayon);
    m_buttonO.setResize(x+8*inter, y, rayon);
    m_buttonP.setResize(x+9*inter, y, rayon);
    
    y = y1+3*inter;
    x = m_x+m_lg/2-4.5*inter;
    m_buttonQ.setResize(x, y, rayon);
    m_buttonS.setResize(x+inter, y, rayon);
    m_buttonD.setResize(x+2*inter, y, rayon);
    m_buttonF.setResize(x+3*inter, y, rayon);
    m_buttonG.setResize(x+4*inter, y, rayon);
    m_buttonH.setResize(x+5*inter, y, rayon);
    m_buttonJ.setResize(x+6*inter, y, rayon);
    m_buttonK.setResize(x+7*inter, y, rayon);
    m_buttonL.setResize(x+8*inter, y, rayon);
    m_buttonM.setResize(x+9*inter, y, rayon);
    
    y = y1+4*inter;
    x = m_x+m_lg/2-4.5*inter;
    m_buttonW.setResize(x, y, rayon);
    m_buttonX.setResize(x+inter, y, rayon);
    m_buttonC.setResize(x+2*inter, y, rayon);
    m_buttonV.setResize(x+3*inter, y, rayon);
    m_buttonB.setResize(x+4*inter, y, rayon);
    m_buttonN.setResize(x+5*inter, y, rayon);
    m_buttonSpace.setResize(x+6*inter, y, rayon);
    
    
    //m_button_point.setResize(x, y, rayon);
    m_button_ok.setResize(x+8*inter, y, rayon);
    
}

KeyBoardWidget::KeyBoardWidget(){
//    m_close = false;
}


void KeyBoardWidget::myDrawButton(ButtonGui * b, QString s){
    drawButton(b);
    drawQText(s, b->m_x, b->m_y, sizeText_medium, true);
}


void KeyBoardWidget::draw(){
    scene->addRect(m_x, m_height*0.1, m_lg, m_height*0.8, m_penBlack, m_brushWhiteAlpha);
    
    QString s = "KeyBoard";
    drawQText(s, m_x+m_lg/2, 0.15*m_height, sizeText_big, true);
    
    s = m_res;
    drawQText(s, m_x+m_lg/2, 0.25*m_height, sizeText_big, true);
    
    myDrawButton(&m_buttonA, "A");
    myDrawButton(&m_buttonB, "B");
    myDrawButton(&m_buttonC, "C");
    myDrawButton(&m_buttonD, "D");
    myDrawButton(&m_buttonE, "E");
    myDrawButton(&m_buttonF, "F");
    myDrawButton(&m_buttonG, "G");
    myDrawButton(&m_buttonH, "H");
    myDrawButton(&m_buttonI, "I");
    myDrawButton(&m_buttonJ, "J");
    myDrawButton(&m_buttonK, "K");
    myDrawButton(&m_buttonL, "L");
    myDrawButton(&m_buttonM, "M");
    myDrawButton(&m_buttonN, "N");
    myDrawButton(&m_buttonO, "O");
    myDrawButton(&m_buttonP, "P");
    myDrawButton(&m_buttonQ, "Q");
    myDrawButton(&m_buttonR, "R");
    myDrawButton(&m_buttonS, "S");
    myDrawButton(&m_buttonT, "T");
    myDrawButton(&m_buttonU, "U");
    myDrawButton(&m_buttonV, "V");
    myDrawButton(&m_buttonW, "W");
    myDrawButton(&m_buttonX, "X");
    myDrawButton(&m_buttonY, "Y");
    myDrawButton(&m_buttonZ, "Z");
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
    myDrawButton(&m_buttonSpace, "_");
    
    myDrawButton(&m_button_ok, "ok");
}

void KeyBoardWidget::addLetter(QString se){
    m_res = m_res + se;
}

void KeyBoardWidget::onMouse(int x, int y){
    if(m_button0.isActive(x, y)){
        addLetter("0");
    } else if(m_button1.isActive(x, y)){
        addLetter("1");
    } else if(m_button2.isActive(x, y)){
        addLetter("2");
    } else if(m_button3.isActive(x, y)){
        addLetter("3");
    } else if(m_button4.isActive(x, y)){
        addLetter("4");
    } else if(m_button5.isActive(x, y)){
        addLetter("5");
    } else if(m_button6.isActive(x, y)){
        addLetter("6");
    } else if(m_button7.isActive(x, y)){
        addLetter("7");
    } else if(m_button8.isActive(x, y)){
        addLetter("8");
    } else if(m_button9.isActive(x, y)){
        addLetter("9");
    } else if(m_buttonA.isActive(x, y)){
        addLetter("A");
    } else if(m_buttonB.isActive(x, y)){
        addLetter("B");
    } else if(m_buttonC.isActive(x, y)){
        addLetter("C");
    } else if(m_buttonD.isActive(x, y)){
        addLetter("D");
    } else if(m_buttonE.isActive(x, y)){
        addLetter("E");
    } else if(m_buttonF.isActive(x, y)){
        addLetter("F");
    } else if(m_buttonG.isActive(x, y)){
        addLetter("G");
    } else if(m_buttonH.isActive(x, y)){
        addLetter("H");
    } else if(m_buttonI.isActive(x, y)){
        addLetter("I");
    } else if(m_buttonJ.isActive(x, y)){
        addLetter("J");
    } else if(m_buttonK.isActive(x, y)){
        addLetter("K");
    } else if(m_buttonL.isActive(x, y)){
        addLetter("L");
    } else if(m_buttonM.isActive(x, y)){
        addLetter("M");
    } else if(m_buttonN.isActive(x, y)){
        addLetter("N");
    } else if(m_buttonO.isActive(x, y)){
        addLetter("O");
    } else if(m_buttonP.isActive(x, y)){
        addLetter("P");
    } else if(m_buttonQ.isActive(x, y)){
        addLetter("Q");
    } else if(m_buttonR.isActive(x, y)){
        addLetter("R");
    } else if(m_buttonS.isActive(x, y)){
        addLetter("S");
    } else if(m_buttonT.isActive(x, y)){
        addLetter("T");
    } else if(m_buttonU.isActive(x, y)){
        addLetter("U");
    } else if(m_buttonV.isActive(x, y)){
        addLetter("V");
    } else if(m_buttonW.isActive(x, y)){
        addLetter("W");
    } else if(m_buttonX.isActive(x, y)){
        addLetter("X");
    } else if(m_buttonY.isActive(x, y)){
        addLetter("Y");
    } else if(m_buttonZ.isActive(x, y)){
        addLetter("Z");
    } else if(m_buttonSpace.isActive(x, y)){
       addLetter("_");
    } else if(m_button_ok.isActive(x, y)){
        if(m_value){
            m_value->m_text = m_res.toUtf8().constData();
        }
        m_close = true;
    }
}
