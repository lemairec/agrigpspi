#include "gps_widget.hpp"

#include <sstream>
#include <fstream>
#include "../logging.hpp"

#include "../gps_framework.hpp"

#include <math.h>
#include <chrono>

#include <QGraphicsTextItem>

#define MY_HEIGTH 380

GpsWidget::GpsWidget(){
    connect(this, SIGNAL(onValueChangeSignal()), this, SLOT(onValueChangeSlot()));
    scene = new QGraphicsScene(this);
    this->setScene(scene);
    m_height = MY_HEIGTH;
    m_width = 800;
    scene->setSceneRect(0, 0, m_width, m_height);
    setFixedSize(m_width + 2, m_height + 2);
    
    m_btnA = new QPushButton(this);
    m_btnA->setGeometry(0,50, 50, 50);
    m_btnA->setText("A");
    m_btnB = new QPushButton(this);
    m_btnB->setGeometry(0,100, 50, 50);
    m_btnB->setText("B");
    m_btnZoomUp = new QPushButton(this);
    m_btnZoomUp->setGeometry(10,MY_HEIGTH-90, 40, 40);
    m_btnZoomUp->setText("+");
    m_btnZoomDown = new QPushButton(this);
    m_btnZoomDown->setGeometry(10,MY_HEIGTH-40, 40, 40);
    m_btnZoomDown->setText("-");
    m_btnClose = new QPushButton(this);
    m_btnClose->setGeometry(m_width - 50, 10, 40, 40);
    m_btnClose->setText("x");
    m_btnOptions = new QPushButton(this);
    m_btnOptions->setGeometry(m_width - 50, 60, 40, 40);
    m_btnOptions->setText("o");
    addButtons();
    m_zoom = 10;
    
    connect(m_btnZoomUp, SIGNAL(clicked()), this, SLOT(onZoomUp()));
    connect(m_btnZoomDown, SIGNAL(clicked()), this, SLOT(onZoomDown()));
    connect(m_btnA, SIGNAL(clicked()), this, SLOT(onBtnA()));
    connect(m_btnB, SIGNAL(clicked()), this, SLOT(onBtnB()));
    connect(m_btnClose, SIGNAL(clicked()), qApp,SLOT(quit()));
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


void GpsWidget::drawLines(double x, double y){
    //addligne(0, x, y);
    GpsFramework & f = GpsFramework::Instance();
    double x0 = x;
    double y0 = y;
    double res = -(y0*f.m_b +(f.m_a * x0 + f.m_c));
    double l = res / (f.m_b/cos(atan(-f.m_a/f.m_b)));
    
    int l0 = round(l/f.m_config.m_largeur)*f.m_config.m_largeur;
    for(int i = -10; i <= 10; ++i){
        addligne(l0 + i*f.m_config.m_largeur, x, y);
    }
    
    //double y0*f.m_b + f.m_a * x0 + f.m_c= - res;
    
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

auto last_update = std::chrono::system_clock::now();

void GpsWidget::drawBarreGuidage(){
    GpsFramework & f = GpsFramework::Instance();
    QBrush grayBrush(Qt::gray);
    QBrush lightGrayBrush(Qt::lightGray);
    QBrush green(Qt::green);
    
    scene->addRect(m_width/2-300, 0, 600, 40, m_penBlack, lightGrayBrush);
    scene->addRect(m_width/2-50, 5, 100, 30, m_penBlack, grayBrush);
    QString s = QString::number(f.m_distanceAB, 'f', 2) + " m";
    auto textItem = scene->addText(s);
    auto mBounds = textItem->boundingRect();
    textItem->setPos(m_width/2 - mBounds.width()/2, 10);
    for(int i = 0; i < 8; ++i){
        scene->addRect(m_width/2 - 80 - 30*i, 10, 20, 20, m_penBlack, grayBrush);
        scene->addRect(m_width/2 + 60 + 30*i, 10, 20, 20, m_penBlack, grayBrush);
    }
    if(f.m_ledAB > 0){
        for(int i = 0; i < std::min(8, f.m_ledAB); ++i){
            scene->addRect(m_width/2 - 80 - 30*i, 10, 20, 20, m_penBlack, green);
        }
    } else {
        for(int i = 0; i < std::min(8, -f.m_ledAB); ++i){
            scene->addRect(m_width/2 + 60 + 30*i, 10, 20, 20, m_penBlack, green);
        }
    }
    
    scene->addRect(0, 0, 80, 40, m_penBlack, lightGrayBrush);
    s = QString::number(f.m_vitesse, 'f', 2) + " km/h";
    textItem = scene->addText(s);
    mBounds = textItem->boundingRect();
    textItem->setPos(40 - mBounds.width()/2, 10);
    
    
}

void GpsWidget::onValueChangeSlot(){
    auto now = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = now - last_update;
    if(diff.count() < 0.2){
        //INFO(diff.count());
        return;
    }
    //INFO(diff.count());
    last_update = now;
    
    int h = 400;
    int w = 800;
    //INFO(w << " " << h);
        static int i = 0;
    QBrush greenBrush(Qt::darkGreen);
    QColor c = Qt::red;
    c.setAlpha(100);
    QBrush testBrush(c);
    
    
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
    
    drawLines(x, y);
    
    for(auto frame : f.m_list){
        double x1  = (frame->m_x - x)*m_zoom;
        double y1  = (frame->m_y - y)*m_zoom;
        scene->addEllipse(w/2 + x1, h/2 - y1, 1, 1, m_penBlack, m_brushNo);
    }
    if(f.m_pointA.m_x!=0){
        double xA  = (f.m_pointA.m_x - x)*m_zoom;
        double yA  = (f.m_pointA.m_y - y)*m_zoom;
        scene->addEllipse(w/2 + xA, h/2 - yA, 1, 1, m_penRed, m_brushNo);
    }
    if(f.m_pointB.m_x!=0){
        double xB  = (f.m_pointB.m_x - x)*m_zoom;
        double yB  = (f.m_pointB.m_y - y)*m_zoom;
        scene->addEllipse(w/2 + xB, h/2 - yB, 1, 1, m_penRed, m_brushNo);
    }
    if(f.m_pointA.m_x!=0 && f.m_pointB.m_x!=0){
        double xA  = (f.m_pointA.m_x - x)*m_zoom;
        double yA  = (f.m_pointA.m_y - y)*m_zoom;
        double xB  = (f.m_pointB.m_x - x)*m_zoom;
        double yB  = (f.m_pointB.m_y - y)*m_zoom;
        
        
        scene->addLine(w/2 + xA, h/2 - yA, w/2 + xB, h/2 - yB, m_penRed);
    }
    
    scene->addLine(w/2, h/2, w/2 - f.m_deplacementX*m_zoom, h/2 + f.m_deplacementY*m_zoom, m_penBlue);
    
    drawBarreGuidage();
    
    std::ostringstream oss;
    oss <<  "nbSt " << last_frame->m_nbrSat << " fix " << last_frame->m_fix << " AB " << f.m_sensAB << " v " << round(f.m_vitesse) << "km/h";
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
