#include "gps_widget.hpp"

#include <sstream>
#include <fstream>
#include "../logging.hpp"

#include "../gps_framework.hpp"

#include <math.h>

GpsWidget::GpsWidget(){
    connect(this, SIGNAL(onValueChangeSignal()), this, SLOT(onValueChangeSlot()));
    scene = new QGraphicsScene(this);
    this->setScene(scene);
    m_height = 400;
    m_width = 800;
    scene->setSceneRect(0, 0, m_width, m_height);
    setFixedSize(m_width + 2, m_height + 2);
    
    m_btnA = new QPushButton(this);
    m_btnA->setGeometry(0,0, 50, 50);
    m_btnA->setText("A");
    m_btnB = new QPushButton(this);
    m_btnB->setGeometry(0,50, 50, 50);
    m_btnB->setText("B");
    m_btnZoomUp = new QPushButton(this);
    m_btnZoomUp->setGeometry(0,300, 50, 50);
    m_btnZoomUp->setText("+");
    m_btnZoomDown = new QPushButton(this);
    m_btnZoomDown->setGeometry(0,350, 50, 50);
    m_btnZoomDown->setText("-");
    addButtons();
    m_zoom = 10;
    
    connect(m_btnZoomUp, SIGNAL(clicked()), this, SLOT(onZoomUp()));
    connect(m_btnZoomDown, SIGNAL(clicked()), this, SLOT(onZoomDown()));
    connect(m_btnA, SIGNAL(clicked()), this, SLOT(onBtnA()));
    connect(m_btnB, SIGNAL(clicked()), this, SLOT(onBtnB()));
    m_penBlack = QPen(Qt::black);
    m_penRed = QPen(Qt::red);
    m_penBlue = QPen(Qt::blue);
}

void GpsWidget::onNewPoint(){
    emit onValueChangeSignal();
}

void GpsWidget::drawCourbe(double l){
    scene->addEllipse(m_width/2 - l*m_zoom, m_height/2 - l*m_zoom, l*m_zoom*2, l*m_zoom*2, m_penBlack, m_brushNo);
    
}

void GpsWidget::addligne(double l, double x, double y){
    GpsFramework & f = GpsFramework::Instance();
    double res = l*f.m_b/cos(atan(-f.m_a/f.m_b));
    double x0 = -400/m_zoom + x;
    double y0 = -(f.m_a * x0 + f.m_c + res)/f.m_b;
    x0 = (x0 - x)*m_zoom;
    y0 = (y0 - y)*m_zoom;
    double x1 = 400/m_zoom + x;
    double y1 = -(f.m_a * x1 + f.m_c + res)/f.m_b;
    x1 = (x1 - x)*m_zoom;
    y1 = (y1 - y)*m_zoom;
    scene->addLine(m_width/2 + x0, m_height/2 - y0, m_width/2 + x1, m_height/2 - y1, m_penBlack);
}

void GpsWidget::onValueChangeSlot(){
    int h = 400;
    int w = 800;
    //INFO(w << " " << h);
        static int i = 0;
    QBrush greenBrush(Qt::darkGreen);
    
    
    i += 1;
    scene->clear();
    
    
    scene->addRect(0, 0, w, h, m_penBlack, greenBrush);
    GpsFramework & f = GpsFramework::Instance();
    auto last_frame = (*f.m_list.begin());
    double x = last_frame->m_x;
    double y = last_frame->m_y;
   
    drawCourbe(1);
    drawCourbe(10);
    drawCourbe(100);
    
    
    for(auto frame : f.m_list){
        double x1  = (frame->m_x - x)*m_zoom;
        double y1  = (frame->m_y - y)*m_zoom;
        scene->addEllipse(w/2 + x1, h/2 - y1, 1, 1, m_penBlack, m_brushNo);
    }
    if(f.m_pointA){
        double xA  = (f.m_pointA->m_x - x)*m_zoom;
        double yA  = (f.m_pointA->m_y - y)*m_zoom;
        scene->addEllipse(w/2 + xA, h/2 - yA, 1, 1, m_penRed, m_brushNo);
    }
    if(f.m_pointB){
        double xB  = (f.m_pointB->m_x - x)*m_zoom;
        double yB  = (f.m_pointB->m_y - y)*m_zoom;
        scene->addEllipse(w/2 + xB, h/2 - yB, 1, 1, m_penRed, m_brushNo);
    }
    if(f.m_pointA && f.m_pointB){
        double xA  = (f.m_pointA->m_x - x)*m_zoom;
        double yA  = (f.m_pointA->m_y - y)*m_zoom;
        double xB  = (f.m_pointB->m_x - x)*m_zoom;
        double yB  = (f.m_pointB->m_y - y)*m_zoom;
        
        addligne(0, x, y);
        for(int i = -10; i <= 10; ++i){
            addligne(i*27, x, y);
        }
        scene->addLine(w/2 + xA, h/2 - yA, w/2 + xB, h/2 - yB, m_penRed);
    }
    
    scene->addLine(w/2, h/2, w/2 + f.m_deplacementX*m_zoom, h/2 - f.m_deplacementY*m_zoom, m_penBlue                                                                                                                                                                                                                                                                                                                                 );
    
    std::ostringstream oss;
    oss <<  "nbSt " << last_frame->m_nbrSattelites;
    //INFO(oss.str());
    QString s = QString::fromStdString(oss.str());
    emit setMessageStatusBar(s);
}

void GpsWidget::addButtons(){
    //scene->addWidget(m_btnA);
    //scene->addWidget(m_btnB);
    //scene->addWidget(m_btnZoomUp);
    //scene->addWidget(m_btnZoomDown);
}

void GpsWidget::onZoomUp(){
    m_zoom *= 1.2;
    onValueChangeSlot();
}

void GpsWidget::onZoomDown(){
    m_zoom *= 0.8;
    onValueChangeSlot();
}

void GpsWidget::onBtnA(){
    GpsFramework::Instance().savePointA();
}
void GpsWidget::onBtnB(){
    GpsFramework::Instance().savePointB();
}
