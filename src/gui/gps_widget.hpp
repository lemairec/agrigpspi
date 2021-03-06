#ifndef GPS_WIDGET_H
#define GPS_WIDGET_H

#include "../gps_framework.hpp"
#include "base_widget.hpp"
#include "option_widget.hpp"
#include "infos_widget.hpp"
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
    
    InfosWidget m_infosWidget;
    
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
    
    QPixmap * m_tracteur_2d;
    QPixmap * m_tracteur_3d;
    
    ButtonGui m_buttonSat;
    ButtonGui m_button3d;
    ButtonGui m_buttonPlus;
    ButtonGui m_buttonMinus;
    ButtonGui m_buttonGuidage;
    ButtonGui m_buttonParcelle;
    ButtonGui m_buttonOption;
    ButtonGui m_buttonErrorOk;
    
    ButtonGui m_buttonChamp;
    ButtonGui m_buttonVolant;
    ButtonGui m_buttonVolantAuto;
    
    bool m_debug = false;
    
    void setPainter(QPainter * s);
    
    void drawLine2(Line_ptr l, QPen & pen);
    void drawCurve(Curve_ptr l, QPen & pen);
    void drawLines();
    void drawLineCurve();
    
    void drawParcelle(bool force = false);
    void drawSurfaceToDraw();
    
    void drawRect3D(double x, double y, double l, double lg);
    void drawTracteur3D();
    void drawTracteur();
    void drawBottom();
    void drawTop();
    void drawError();
    void drawBackground();
    
    void drawDebug();
    void drawDebugEkf();
    void drawVolant(double y, double x);
    
    void draw();
    void draw_force();
    
    double m_zoom;
    
    bool m_vue_3D;
    
    //angle
    double m_xref;
    double m_yref;
    double m_a;
    double m_cosA;
    double m_sinA;
    
    void my_projete(double x, double y, double & x_res, double & y_res);
    void my_projete2(double x, double y, double & x_res, double & y_res);
    void my_projete2_pt(GpsPoint_ptr pt, double & x_res, double & y_res);
    bool must_be_draw(double x, double y);
    
    void setSize(int width, int height);
    
    
    void mouseReleaseEvent ( QMouseEvent * event );
    
    void onMouse(int x, int y);
};

#endif // GPS_WIDGET_H
