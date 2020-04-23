#ifndef GPS_WIDGET_H
#define GPS_WIDGET_H

#include "../gps_framework.hpp"
#include "base_widget.hpp"
#include "option_widget.hpp"

class GpsWidget : public BaseWidget {
public:
    GpsWidget();
    
    OptionWidget m_optionsWidget;
    
    void addButtons();

    int m_widthMax;
    int m_heightMax;
    
    std::list<Button> m_buttons;
    
    QPen m_penBlack;
    QPen m_penRed;
    QPen m_penBlue;
    QPen m_penBlue2;
    QPen m_penNo;
    QBrush m_brushNo;
    QBrush m_brushGreen;
    QBrush m_brushGreenTractor;
    QBrush m_grayBrush;
    QBrush m_greenBrush;
    QBrush m_lightGrayBrush;
    QBrush m_lightGrayDebugBrush;
    
    QPixmap * m_imgClose;
    QPixmap * m_imgPlus;
    QPixmap * m_imgMinus;
    QPixmap * m_imgA;
    QPixmap * m_imgB;
    QPixmap * m_imgOption;
    QPixmap * m_imgSat;
    QPixmap * m_imgSatVert;
    QPixmap * m_imgSatOrange;
    QPixmap * m_imgSatRouge;
    QPixmap * m_imgFleche;
    
    ButtonGui * m_buttonClose;
    ButtonGui * m_buttonPlus;
    ButtonGui * m_buttonMinus;
    ButtonGui * m_buttonA;
    ButtonGui * m_buttonB;
    ButtonGui * m_buttonOption;
    
    
    void drawCourbe(double l);
    bool addligne(double l, double x, double y, int i);
    void drawLines(double x, double y);
    void drawTracteur();
    void drawBottom();
    void drawDebug();
    void drawContour(double x, double y);
    
    void draw();
    void draw_force();
    
    double m_zoom;
    
    //angle
    double m_a;
    double m_cosA;
    double m_sinA;
   
    //courbe
    double m_la;
    double m_lb;
    double m_lc;
    
    void setSize(int width, int height);
    
    
    void mouseReleaseEvent ( QMouseEvent * event );

    void drawBarreGuidage();
    void onButton(const Button & button);
    
    
    void onMouse(int x, int y);
};

#endif // GPS_WIDGET_H
