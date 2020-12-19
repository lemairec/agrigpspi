#ifndef BASE_WIDGET_H
#define BASE_WIDGET_H

#include "qt/include_qt.hpp"
#include "../gps_framework.hpp"

const int COLOR_RED = 1;
const int COLOR_GREEN = 2;
const int COLOR_CHECK = 3;

const int COLOR_OTHER = 4;

class KeyPadWidget;
class KeyBoardWidget;

enum SizeText{
    sizeText_little,
    sizeText_medium,
    sizeText_big
};

struct ButtonGui{
    ButtonGui(double x, double y, double rayon, int type = 0);
    ButtonGui(){};
    
    void setResize(int x, int y, int rayon);
    
    bool isActive(int x, int y);
    int m_x = 0;
    int m_y = 0;
    int m_rayon = 0;
    
    int m_type = 0;
    
    std::string m_label = "";
    int m_labelInt = 0;
};

struct ValueGui{
    ValueGui(double x, double y, double rayon, int type, std::string label);
    ValueGui(){};
    
    void setResize(int x, int y, int rayon, std::string label);
    
    int isActive(double x, double y);
    
    double getMultValue(double x, double y);
    double getMultValue2(double x, double y, double value);
    double getIntValue(double x, double y);
    
    double m_x = 0;
    double m_y = 0;
    double m_rayon = 0;
    std::string m_label;
    ButtonGui m_buttonAdd;
    ButtonGui m_buttonMinus;
    
    int m_type = 0;
};

struct ValueGuiKeyPad{
    ValueGuiKeyPad(){};
    
    void setResize(int x, int y);
    
    int isActive(double x, double y);
    
    double m_x = 0;
    double m_y = 0;
    double m_value = 1.0;
    std::string m_label;
    std::string m_unity = "m";
};

struct ValueGuiKeyBoard{
    ValueGuiKeyBoard(){};
    
    void setResize(int x, int y);
    
    int isActive(double x, double y);
    
    double m_x = 0;
    double m_y = 0;
    std::string m_text;
};


struct SelectButtonGui{
    SelectButtonGui(double x, double y, double rayon);
    SelectButtonGui(){};

    void setResize(int x, int y, int rayon);
    
    double m_x = 0;
    double m_y = 0;
    double m_rayon = 0;
    
    std::string m_label;
    std::vector<std::string> m_values;
    std::vector<int> m_values_int;
    int m_selectedValue = 0;
    ButtonGui m_buttonOpen;
    bool m_open = false;
    
    std::vector<ButtonGui *> m_buttons;
    
    void addValue(std::string s);
    void addValueInt(std::string s, int i);
    void clear();
    
    std::string getValueString();
    int getValueInt();
    
    void setValueInt(int i);
    void setValueString(std::string s);
    
    int m_type = 0;
};

class BaseWidget{
public:
    QPainter * m_painter;
    
    QPen m_penBlack;
    QPen m_penGray;
    QPen m_penRed;
    QPen m_penGreen;
    QPen m_penBlue;
    QPen m_penBlue2;
    QPen m_penNo;

    QBrush m_brushNo;
    QBrush m_brushWhite;
    QBrush m_brushWhiteAlpha;
    QBrush m_brushGreenTransparent;
    QBrush m_brushRed;
    QBrush m_brushOrange;
    QBrush m_brushGray;
    QBrush m_brushGreen;
    QBrush m_brushDarkGray;
    QBrush m_brushLightGrayDebug;
    
    QBrush m_brushParcelle;
    
    QBrush m_brushParcelle1;
    QBrush m_brushParcelle2;
    
public:
    virtual void setPainter(QPainter * p){
        m_painter = p;
    }
public:
    int m_gros_button = 10;
    int m_gros_gros_button = 10;
    int m_petit_button = 10;
    
    double m_width;
    double m_height;
    bool m_close = true;
    
    KeyPadWidget * m_key_pad_widget;
    KeyBoardWidget * m_key_board_widget;
    BaseWidget();
    
    
    
    virtual void draw() = 0;
    virtual void onButton(const ButtonGui & button){};
    
    virtual void onMouse(int x, int y){};
    virtual void setSize(int width, int height);
    
    void drawButton(ButtonGui * button, int color = 0);
    void drawButtonLabel(ButtonGui * button, int color = 0);
    void drawButtonImage(ButtonGui * button, QPixmap & pixmap, double scale=1.0);

    void drawSelectButtonGuiOpen(SelectButtonGui * select);
    void drawSelectButtonGuiClose(SelectButtonGui * select);
    int onMouseSelectButton(SelectButtonGui *select, double x, double y);
    
    void drawMyImage(QPixmap & pixmap, int x, int y, double scale, bool center);
    
    void drawLabel(std::string s, double x, double y);
    void drawLabel2(std::string s, double x, double y);

    void drawValueGui(ValueGui * valueGui, double value);
    void drawValueGui2(ValueGui * valueGui, QPixmap * pixmap1, QPixmap * pixmap2, std::string s);
    
    void drawText(const std::string & text, int x, int y, SizeText = sizeText_little, bool center = false, bool white = false);
    void drawQText(const QString & text, int x, int y, SizeText = sizeText_little, bool center = false, bool white = false);
    void drawQTexts(const QString & text, int x, int y, SizeText = sizeText_little, bool center = false, bool white = false);
    

    void drawValueGuiKeyPad(ValueGuiKeyPad * value);
    bool isActiveValueGuiKeyPad(ValueGuiKeyPad * value, int x, int y);
    
    void drawValueGuiKeyBoard(ValueGuiKeyBoard * value);
    bool isActiveValueGuiKeyBoard(ValueGuiKeyBoard * value, int x, int y);

    QPixmap * loadImage(const std::string & s);
};

#endif
