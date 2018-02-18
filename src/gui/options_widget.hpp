#ifndef OPTIONS_WIDGET_HPP
#define OPTIONS_WIDGET_HPP

#include "base_widget.hpp"

class GpsWidget;

class OptionsWidget : public BaseWidget
{
    GpsWidget * m_gpsWidget;
    
    std::list<std::string> m_serials;
public:
    OptionsWidget(GpsWidget * widget);
    virtual void draw();
    virtual void onButton(const Button & button);
    
    void setSize(int width, int height);
    void open();
    void addSerials();
    
    void run(const std::string & str);
};

#endif
