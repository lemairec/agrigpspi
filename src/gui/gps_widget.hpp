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
    QPixmap * m_imgChampGris;
    QPixmap * m_imgChampVert;
    QPixmap * m_imgVolantGris;
    QPixmap * m_imgVolantVert;
    QPixmap * m_imgOk;
    
    ButtonGui * m_buttonClose;
    ButtonGui * m_buttonPlus;
    ButtonGui * m_buttonMinus;
    ButtonGui * m_buttonA;
    ButtonGui * m_buttonB;
    ButtonGui * m_buttonOption;
    ButtonGui * m_buttonErrorOk;
    
    ButtonGui * m_buttonChamp;
    ButtonGui * m_buttonVolant;
    
    QBrush m_brushTractor;
    QPen m_penTractorEssieu;
    QPen m_penTractorRoue;
    
    void drawCourbe(double l);
    bool addligne(double l, int i);
    void drawLines();
    void drawTracteur();
    void drawBottom();
    void drawContour();
    void drawError();
    
    void drawDebug();
    void drawVolant(double y);
    void drawVolant_(double y, double a, double r, double start_angle);
    
    void draw();
    void draw_force();
    
    double m_zoom;
    
    
    
    //angle
    double m_xref;
    double m_yref;
    double m_a;
    double m_cosA;
    double m_sinA;
    
    void my_projete(double x, double y, double & x_res, double & y_res);
   
    //courbe
    double m_la;
    double m_lb;
    double m_lc;
    
    void setSize(int width, int height);
    
    
    void mouseReleaseEvent ( QMouseEvent * event );

    void drawBarreGuidage();
    
    void onMouse(int x, int y);
};

#endif // GPS_WIDGET_H
