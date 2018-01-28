#include "gps_widget.hpp"

#include <sstream>
#include <fstream>
#include "../logging.hpp"

#include "../gps_framework.hpp"

#include <math.h>
#include <chrono>

#include <QGraphicsTextItem>

#define MY_WIDTH 800
#define MY_HEIGTH 400


GpsWidget::GpsWidget(){
    connect(this, SIGNAL(onValueChangeSignal()), this, SLOT(onValueChangeSlot()));
    scene = new QGraphicsScene(this);
    this->setScene(scene);
    
    m_btnA = new QPushButton(this);
    m_btnB = new QPushButton(this);
    m_btnZoomUp = new QPushButton(this);
    m_btnZoomDown = new QPushButton(this);
    m_btnClose = new QPushButton(this);
    m_btnOptions = new QPushButton(this);
    
    setSize(MY_WIDTH, MY_HEIGTH);
    scene->setSceneRect(0, 0, m_width, m_height);
    setFixedSize(m_width + 2, m_height + 2);
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    
    
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
    
    m_brushGreen = QBrush(QColor(0, 150, 0, 100));
    m_penNo.setColor(QColor(0, 250, 0, 0));
    //m_brushGreen.a
}

void GpsWidget::setSize(int width, int height){
    INFO(width << " " << height);
    m_width = width;
    m_height = height;
    m_widthMax = width/2+50;
    m_heightMax = height/2+50;
    scene->setSceneRect(0, 0, m_width, m_height);
    setFixedSize(m_width + 2, m_height + 2);
    
    m_btnA->setGeometry(0,50, 50, 50);
    m_btnA->setText("A");
    m_btnB->setGeometry(0,100, 50, 50);
    m_btnB->setText("B");
    m_btnZoomUp->setGeometry(10,m_height-90, 40, 40);
    m_btnZoomUp->setText("+");
    m_btnZoomDown->setGeometry(10,m_height-40, 40, 40);
    m_btnZoomDown->setText("-");
    m_btnClose->setGeometry(m_width - 50, 10, 40, 40);
    m_btnClose->setText("x");
    m_btnOptions->setGeometry(m_width - 50, 60, 40, 40);
    m_btnOptions->setText("o");
}
void GpsWidget::resizeEvent(QResizeEvent* event)
{
//    m_height = parentWidget()->parentWidget()->size().height();
//    m_width = parentWidget()->parentWidget()->size().width();
    
    
    
    
}

void GpsWidget::onNewPoint(){
    emit onValueChangeSignal();
}

void GpsWidget::drawCourbe(double l){
    scene->addEllipse(m_width/2 - l*m_zoom, m_height/2 - l*m_zoom, l*m_zoom*2, l*m_zoom*2, m_penBlack, m_brushNo);
    //INFO(m_height);
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
    double x0 = -m_width/m_zoom + x;
    double y0 = -(f.m_a * x0 + f.m_c + res)/f.m_b;
    x0 = (x0 - x)*m_zoom;
    y0 = (y0 - y)*m_zoom;
    double x1 = m_width/m_zoom + x;
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
    auto begin = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = begin - last_update;
    if(diff.count() < 0.2){
   //     INFO(diff.count());
        return;
    }
    //INFO(diff.count());
    
    GpsFramework & f = GpsFramework::Instance();
    m_widthMax = m_width/2+f.m_config.m_largeur*m_zoom/2;
    m_heightMax = m_height/2+f.m_config.m_largeur*m_zoom/2;
    
    int h = m_height;
    int w = m_width;
    //INFO(w << " " << h);
        static int i = 0;
    
    
    i += 1;
    scene->clear();
    
    
    //scene->addRect(0, 0, w, h, m_penBlack, greenBrush);
    auto last_frame = (*f.m_list.begin());
    double x = last_frame->m_x;
    double y = last_frame->m_y;
   
    drawCourbe(1);
    drawCourbe(10);
    drawCourbe(100);
    
    drawLines(x, y);
    
    double x1 = 0;
    double y1 = 0;
    double xA1 = 0, yA1 = 0, xB1 = 0, yB1 = 0;
    
    double l = f.m_config.m_largeur*m_zoom/2;
    int j = 0;
    int init = 0;
    
    for(auto it = f.m_list.begin(); it != f.m_list.end(); ++it){
        auto frame = (*it);
        double x0  = (frame->m_x - x)*m_zoom;
        double y0  = (frame->m_y - y)*m_zoom;
        
        if(x0 < -m_widthMax || x0 > m_widthMax || y0 < -m_heightMax || y0 > m_heightMax ){
            init = 0;
            x1 = x0;
            y1 = y0;
            continue;
        }
        double dist = (x0-x1)*(x0-x1) + (y0-y1)*(y0-y1);
        if(y1 != y0){
            double res = (x1-x0)/(y1-y0);
            double i = 1.0;
            if(y1-y0 > 0){
                i = -1.0;
            }
            double xA = x0+ i*l/sqrt(1+res*res);
            double yA = y0-(xA-x0)*res;
            double xB = x0- i*l/sqrt(1+res*res);
            double yB = y0-(xB-x0)*res;
            
            if(dist < 0.1*l*l){
                //continue;
            }
            if(init != 0){
                QPolygon polygon;
                polygon << QPoint(w/2 + xA, h/2 - yA) << QPoint(w/2 + xB, h/2 - yB) << QPoint(w/2 + xB1, h/2 - yB1)<< QPoint(w/2 + xA1, h/2 - yA1);
                scene->addPolygon(polygon, m_penNo, m_brushGreen);
            } else if(init == 0) {
                double res = (x1-x0)/(y1-y0);
                double i = 1.0;
                if(y1-y0 > 0){
                    i = -1.0;
                }
                double xA1 = x1+ i*l/sqrt(1+res*res);
                double yA1 = y1-(xA1-x1)*res;
                double xB1 = x1- i*l/sqrt(1+res*res);
                double yB1 = y1-(xB1-x1)*res;
                QPolygon polygon;
                polygon << QPoint(w/2 + xA, h/2 - yA) << QPoint(w/2 + xB, h/2 - yB) << QPoint(w/2 + xB1, h/2 - yB1)<< QPoint(w/2 + xA1, h/2 - yA1);
                scene->addPolygon(polygon, m_penNo, m_brushGreen);
            }
            xA1 = xA, yA1 = yA, xB1 = xB, yB1 = yB;
            x1 = x0;
            y1 = y0;
            ++init;
            
            //scene->addEllipse(w/2 + xA, h/2 - yA, 1, 1, m_penRed, m_brushNo);
            //scene->addEllipse(w/2 + xB, h/2 - yB, 1, 1, m_penRed, m_brushNo);
            
            //scene->addEllipse(w/2 + xB, h/2 - yB, 20, 20, m_penRed, m_brushGreen);
            
        }
        scene->addEllipse(w/2 + x0, h/2 - y0, 1, 1, m_penBlack, m_brushNo);
        j ++;
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
    
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> diff2 = begin - end;
    last_update = end;
    
    std::ostringstream oss;
    oss <<  "nbSt " << last_frame->m_nbrSat << ", fix " << last_frame->m_fix << ", AB " << f.m_sensAB << ", v " << round(f.m_vitesse) << "km/h, draw " << diff2.count();
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
