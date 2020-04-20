#ifndef BASE_WIDGET_H
#define BASE_WIDGET_H

#include "qt/include_qt.hpp"
#include "../gps_framework.hpp"

struct Button{
    double m_x = 0;
    double m_y = 0;
    int m_id = 0;
    std::string m_desc;
    
    Button(int x, int y, int id)
    :m_x(x), m_y(y), m_id(id)
    {
    }
};

/*class BaseWidget{
protected:
    QGraphicsScene * scene;
public:
    std::list<Button> m_buttons;
    int m_width;
    int m_height;
    bool m_close = true;
    
    void setScene(QGraphicsScene * s){
        scene = s;
    }
    virtual void draw() = 0;
    virtual void onButton(const Button & button);
    virtual void onMouse(int x, int y);
    
    void setSize(int width, int height);
    
    void drawText(const std::string & text, int x, int y, int size, bool center);
};*/

/**
 
 NEW !!!
 
 */

const double PETIT_RAYON = 0.035;
const double GROS_BUTTON = 0.04;
const double PETIT_INTER = PETIT_RAYON*1.2;

const int COLOR_RED = 1;
const int COLOR_GREEN = 2;
const int COLOR_CHECK = 3;

const int COLOR_OTHER = 4;

struct ButtonGui{
    ButtonGui(double x, double y, double rayon, int type);
    
    bool isActive(double x, double y);
    double m_x = 0;
    double m_y = 0;
    double m_rayon = 0;
    
    int m_type = 0;
    
    std::string m_label = "";
};

struct ValueGui{
    ValueGui(double x, double y, double rayon, int type, std::string label);
    
    int isActive(double x, double y);
    
    double getValue(double x, double y);
    double getIntValue(double x, double y);
    
    double m_x = 0;
    double m_y = 0;
    double m_rayon = 0;
    std::string m_label;
    ButtonGui m_buttonAdd;
    ButtonGui m_buttonMinus;
    
    int m_type = 0;
};

class BaseWidget{
protected:
    QGraphicsScene * scene;
public:
    void setScene(QGraphicsScene * s){
        scene = s;
    }
public:
    double m_width;
    double m_height;
    bool m_close = true;
    
    virtual void draw() = 0;
    virtual void onButton(const ButtonGui & button){};
    virtual void onMouse(double x, double y){};
    
    void onMouseInt(double x, double y);
    void setSize(int width, int height);
    
    void drawButton(ButtonGui * button, int color = 0);
    void drawButtonLabel(ButtonGui * button, int color = 0);
    void drawButtonImage(ButtonGui * button, QPixmap & pixmap);

    void drawLabel(std::string s, double x, double y);
    void drawLabel2(std::string s, double x, double y);
     
    void drawText(const std::string & text, double x, double y);
     
    void drawValueGui(ValueGui * valueGui, double value);
    
    void drawText(const std::string & text, int x, int y, int size, bool center);

    QPixmap * loadImage(const std::string & s);
};






#endif
