#ifndef GPS_WIDGET_H
#define GPS_WIDGET_H

#include "include_qt.hpp"
#include "../gps_framework.hpp"

#include <QGraphicsView>
#include <QGraphicsScene>

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

    
    
    void addButtons();

    int m_height;
    int m_width;
    QPen m_penBlack;
    QPen m_penRed;
    QPen m_penBlue;
    QBrush m_brushNo;
    void drawCourbe(double l);
    void addligne(double l, double x, double y);
    
    double m_zoom;
signals:
    void onValueChangeSignal();
    void setMessageStatusBar(QString & s);
    
public slots:
    void onValueChangeSlot();
    void onZoomUp();
    void onZoomDown();
    void onBtnA();
    void onBtnB();
};

#endif // GPS_WIDGET_H
