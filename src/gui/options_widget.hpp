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
    
    void open();
    void addSerials();
};

#endif
