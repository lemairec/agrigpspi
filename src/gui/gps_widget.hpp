#ifndef GPS_WIDGET_H
#define GPS_WIDGET_H

#include "../gps_framework.hpp"
#include "base_widget.hpp"
#include "options_widget.hpp"

class GpsWidget : public BaseWidget {
public:
    GpsWidget();
    
    OptionsWidget m_optionsWidget;
    
    void addButtons();

    int m_width;
    int m_height;
    int m_widthMax;
    int m_heightMax;
    
    QPen m_penBlack;
    QPen m_penRed;
    QPen m_penBlue;
    QPen m_penNo;
    QBrush m_brushNo;
    QBrush m_brushGreen;
    
    void drawCourbe(double l);
    bool addligne(double l, double x, double y, int i);
    void drawLines(double x, double y);
    void drawVitesse();
    void drawSattelite();
    void draw();
    void draw_force();
    
    double m_zoom;
    
    void setSize(int width, int height);
    
    
    void mouseReleaseEvent ( QMouseEvent * event );

    void drawBarreGuidage();
    void onButton(const Button & button);
};

#endif // GPS_WIDGET_H
