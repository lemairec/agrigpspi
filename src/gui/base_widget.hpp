#ifndef BASE_WIDGET_H
#define BASE_WIDGET_H

#include "qt/include_qt.hpp"
#include "../gps_framework.hpp"

const double PETIT_RAYON = 0.035;
const double GROS_BUTTON = 0.04;
const double PETIT_INTER = PETIT_RAYON*1.2;

const int COLOR_RED = 1;
const int COLOR_GREEN = 2;
const int COLOR_CHECK = 3;

const int COLOR_OTHER = 4;

struct ButtonGui{
    ButtonGui(double x, double y, double rayon, int type = 0);
    
    bool isActive(double x, double y);
    double m_x = 0;
    double m_y = 0;
    double m_rayon = 0;
    
    int m_type = 0;
    
    std::string m_label = "";
    int m_labelInt = 0;
};

struct ValueGui{
    ValueGui(double x, double y, double rayon, int type, std::string label);
    
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

struct SelectButtonGui{
    SelectButtonGui(double x, double y, double rayon);

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
protected:
    QGraphicsScene * scene;
    QPen m_penBlack;
    QPen m_penRed;
    QPen m_penBlue;
    QPen m_penBlue2;
    QPen m_penNo;
    QBrush m_brushNo;
    QBrush m_brushWhite;
    QBrush m_brushWhiteAlpha;
    
    QBrush m_brushGreen;
    QBrush m_brushGreenTractor;
    QBrush m_grayBrush;
    QBrush m_greenBrush;
    QBrush m_brushDarkGray;
    QBrush m_brushLightGrayDebug;
    
public:
    void setScene(QGraphicsScene * s){
        scene = s;
    }
public:
    double m_width;
    double m_height;
    bool m_close = true;
    
    BaseWidget();
    
    virtual void draw() = 0;
    virtual void onButton(const ButtonGui & button){};
    virtual void onMouse(double x, double y){};
    
    void onMouseInt(double x, double y);
    void setSize(int width, int height);
    
    void drawButton(ButtonGui * button, int color = 0);
    void drawButtonLabel(ButtonGui * button, int color = 0);
    void drawButtonImage(ButtonGui * button, QPixmap & pixmap, double scale=1.0);

    void drawSelectButtonGuiOpen(SelectButtonGui * select);
    void drawSelectButtonGuiClose(SelectButtonGui * select);
    int onMouseSelectButton(SelectButtonGui *select, double x, double y);
    
    
    void drawLabel(std::string s, double x, double y);
    void drawLabel2(std::string s, double x, double y);
     
    void drawText(const std::string & text, double x, double y);
     
    void drawValueGui(ValueGui * valueGui, double value);
    void drawValueGui2(ValueGui * valueGui, QPixmap * pixmap1, QPixmap * pixmap2, std::string s);
    
    void drawText(const std::string & text, int x, int y, int size, bool center);

    QPixmap * loadImage(const std::string & s);
};




#endif
