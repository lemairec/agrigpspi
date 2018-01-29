#ifndef OPTIONS_WIDGET_HPP
#define OPTIONS_WIDGET_HPP

#include "include_qt.hpp"

class GpsWidget;

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
public:
    std::list<Button> m_buttons;
    int m_width;
    int m_height;
    bool m_close = true;
    
    virtual void draw(QGraphicsScene * scene) = 0;
    virtual void onButton(const Button & button);
    virtual void onMouse(int x, int y);
    
    void setSize(int width, int height);
    
    void drawText(QGraphicsScene * scene, const std::string & text, int x, int y, int size, bool center);
};

class OptionsWidget : public BaseWidget
{
    GpsWidget * m_gpsWidget;
    
    std::list<std::string> m_serials;
public:
    OptionsWidget(GpsWidget * widget);
    virtual void draw(QGraphicsScene * scene);
    virtual void onButton(const Button & button);
    
    void open();
    void addSerials();
};

#endif
