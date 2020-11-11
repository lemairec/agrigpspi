#ifndef GPS_WIDGET_H
#define GPS_WIDGET_H

#include "../gps_framework.hpp"
#include "base_widget.hpp"
#include "option_widget.hpp"
#include "sat_widget.hpp"
#include "machine_widget.hpp"
#include "menu_widget.hpp"
#include "line/line_widget.hpp"
#include "line/line_new_widget.hpp"
#include "line/line_load_widget.hpp"
#include "parcelle/parcelle_widget.hpp"
#include "parcelle/parcelle_new_widget.hpp"
#include "parcelle/parcelle_load_widget.hpp"
#include "keyboard_widget.hpp"

class GpsWidget : public BaseWidget {
    GpsWidget();
public:
    static GpsWidget * Instance();
    
    SatWidget m_satWidget;
    
    ParcelleWidget m_parcelleWidget;
    ParcelleNewWidget m_parcelleNewWidget;
    ParcelleLoadWidget m_parcelleLoadWidget;
    LineWidget m_lineWidget;
    LineNewWidget m_lineNewWidget;
    LineLoadWidget m_lineLoadWidget;
    MenuWidget m_menuWidget;
    std::vector<BaseWidget *> m_widgets;
    
    KeyPadWidget m_key_pad_widget;
    KeyBoardWidget m_key_board_widget;
    
    void addButtons();

    int m_widthMax;
    int m_heightMax;

    QPixmap * m_imgPlus;
    QPixmap * m_imgMinus;
    QPixmap * m_imgGuidage;
    QPixmap * m_imgParcelle;
    QPixmap * m_imgOption;
    QPixmap * m_imgSatBlanc;
    QPixmap * m_imgSatVert;
    QPixmap * m_imgSatOrange;
    QPixmap * m_imgSatRouge;
    QPixmap * m_imgFleche;
    QPixmap * m_imgChampBlanc;
    QPixmap * m_imgChampVert;
    QPixmap * m_imgVolantBlanc;
    QPixmap * m_imgVolantRouge;
    QPixmap * m_imgVolantVert;
    QPixmap * m_imgVolantAutoGris;
    QPixmap * m_imgVolantAutoVert;
    QPixmap * m_imgOk;
    
    ButtonGui m_buttonSat;
    ButtonGui m_buttonPlus;
    ButtonGui m_buttonMinus;
    ButtonGui m_buttonGuidage;
    ButtonGui m_buttonParcelle;
    ButtonGui m_buttonOption;
    ButtonGui m_buttonErrorOk;
    
    ButtonGui m_buttonChamp;
    ButtonGui m_buttonVolant;
    ButtonGui m_buttonVolantAuto;
    
    QBrush m_brushTractor;
    QBrush m_brushOutil;
    QPen m_penOutil;
    QPen m_penTractorEssieu;
    QPen m_penTractorRoue;
    
    bool m_debug = false;
    
    void setScene(QGraphicsScene * s);
    void drawCourbe(double l);
    void drawLine(Lines_ptr l, QPen & pen);
    bool addligne(double l, int i);
    void drawLines();
    void drawTracteur();
    void drawBottom();
    void drawTop();
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
    void my_projete2(double x, double y, double & x_res, double & y_res);
    bool must_be_draw(double x, double y);
    
    //courbe
    double m_la;
    double m_lb;
    double m_lc;
    
    void setSize(int width, int height);
    
    
    void mouseReleaseEvent ( QMouseEvent * event );
    
    void onMouse(int x, int y);
};

#endif // GPS_WIDGET_H
