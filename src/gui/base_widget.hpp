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

class BaseWidget{
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
};

#endif
