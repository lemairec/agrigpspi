#ifndef GPS_WIDGET_H
#define GPS_WIDGET_H

#include "include_qt.hpp"
#include "../gps_framework.hpp"
#include "options_widget.hpp"

class GpsWidget : public QGraphicsView, public IGpsObserver {
Q_OBJECT
public:
    GpsWidget();
    void onNewPoint();

    QGraphicsScene * scene;
    QPushButton * m_btnA;
    QPushButton * m_btnB;
    QPushButton * m_btnZoomUp;
    QPushButton * m_btnZoomDown;
    QPushButton * m_btnClose;
    QPushButton * m_btnOptions;

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
    
    double m_zoom;
    
    void setSize(int width, int height);
    void resizeEvent(QResizeEvent* event);

    
    void mouseReleaseEvent ( QMouseEvent * event );
signals:
    void onValueChangeSignal();
    void setMessageStatusBar(QString & s);
   
    
public slots:
    void drawBarreGuidage();
    void onValueChangeSlot(bool force = false);
    void onZoomUp();
    void onZoomDown();
    void onBtnA();
    void onBtnB();
    void onOption();
};

#endif // GPS_WIDGET_H
