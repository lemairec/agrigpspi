#include "gps_widget.hpp"

#include <sstream>
#include <fstream>
#include <math.h>
#include <cmath>
#include <chrono>
#include <iomanip>

#include "../logging.hpp"
#include "../gps_framework.hpp"


#define MY_WIDTH 800
#define MY_HEIGTH 400

#define BUTTON_A 0
#define BUTTON_B 1
#define BUTTON_ZOOM_UP 2
#define BUTTON_ZOOM_DOWN 3
#define BUTTON_CLOSE 4
#define BUTTON_OPTION 5

GpsWidget::GpsWidget()
:m_optionsWidget(this)
{
    m_zoom = 10;
    
    
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
    m_widthMax = m_width/2+50;
    m_heightMax = m_height/2+50;
    
    m_buttons.clear();
    
    m_buttons.push_front(Button(50, 100,  BUTTON_A));
    m_buttons.push_front(Button(50, 150, BUTTON_B));
    m_buttons.push_front(Button(50,m_height-110 ,BUTTON_ZOOM_UP));
    m_buttons.push_front(Button(50,m_height-50, BUTTON_ZOOM_DOWN));
    m_buttons.push_front(Button(m_width - 50, 50,BUTTON_CLOSE));
    m_buttons.push_front(Button(m_width - 50, 100,BUTTON_OPTION));
    
    m_optionsWidget.setSize(m_width, m_height);
    
//    onValueChangeSlot(true);
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
    //INFO("l " << l << " x " << x << " res " << res);
    
    int i0 = round(l/f.m_config.m_largeur);
    for(int i = 0; i < 1000; ++i){
        if(! addligne((i0 + i)*f.m_config.m_largeur, x, y, i0 + i)){
            break;
        }
    }
    for(int i = 1; i < 1000; ++i){
        if(! addligne((i0 - i)*f.m_config.m_largeur, x, y, i0 - i)){
            break;
        }
    }
}

bool GpsWidget::addligne(double l, double x, double y, int i){
    GpsFramework & f = GpsFramework::Instance();
    double res = l*f.m_b/cos(atan(-f.m_a/f.m_b));
    double x0 = -m_width/(m_zoom*2) + x;
    double y0 = -(f.m_a * x0 + f.m_c + res)/f.m_b;
    x0 = (x0 - x)*m_zoom;
    y0 = (y0 - y)*m_zoom;
    double x1 = m_width/(m_zoom*2) + x;
    double y1 = -(f.m_a * x1 + f.m_c + res)/f.m_b;
    x1 = (x1 - x)*m_zoom;
    y1 = (y1 - y)*m_zoom;
    
    if(y0 > m_height/2 && y1 > m_height/2){
        return false;
    }
  
    if(y0 < -m_height/2 && y1 < -m_height/2){
        return false;
    }
    
    QString s = QString::number(i);
    auto textItem = scene->addText(s);
    auto mBounds = textItem->boundingRect();
    textItem->setPos(m_width/2 + (x1+x0)/2 - mBounds.width()/2, m_height/2 - (y1+y0)/2  - mBounds.height()/2);
    //INFO(x1 << " " << x0);
    
    
    if(i%10 == 0){
        scene->addLine(m_width/2 + x0, m_height/2 - y0, m_width/2 + x1, m_height/2 - y1, m_penBlue);
    } else {
        scene->addLine(m_width/2 + x0, m_height/2 - y0, m_width/2 + x1, m_height/2 - y1, m_penBlack);
    }
    
    return true;
    
}

auto last_update = std::chrono::system_clock::now();

void GpsWidget::drawBarreGuidage(){
    GpsFramework & f = GpsFramework::Instance();
    QBrush grayBrush(Qt::gray);
    QBrush green(Qt::green);
    QBrush lightGrayBrush(Qt::lightGray);
    
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
}

void GpsWidget::drawVitesse(){
    GpsFramework & f = GpsFramework::Instance();
    QBrush lightGrayBrush(Qt::lightGray);
    
    scene->addRect(0, 0, 80, 40, m_penBlack, lightGrayBrush);
    QString s = QString::number(f.m_vitesse, 'f', 2) + " km/h";
    auto textItem = scene->addText(s);
    auto mBounds = textItem->boundingRect();
    textItem->setPos(40 - mBounds.width()/2, 10);
}

void GpsWidget::drawSurface(){
    GpsFramework & f = GpsFramework::Instance();
    QBrush lightGrayBrush(Qt::lightGray);
    
    scene->addRect(0, m_height/2, 80, 40, m_penBlack, lightGrayBrush);
    QString s = QString::number(f.m_surface, 'f', 2) + " ha";
    auto textItem = scene->addText(s);
    auto mBounds = textItem->boundingRect();
    textItem->setPos(40 - mBounds.width()/2, m_height/2);
    QString s2 = QString::number(f.m_surface_h, 'f', 2) + " ha/h";
    auto textItem2 = scene->addText(s2);
    auto mBounds2 = textItem->boundingRect();
    textItem2->setPos(40 - mBounds2.width()/2, m_height/2+20);
    QString s3 = QString::number(f.m_surface_h2, 'f', 2) + " ha/h";
    auto textItem3 = scene->addText(s3);
    auto mBounds3 = textItem->boundingRect();
    textItem3->setPos(40 - mBounds3.width()/2, m_height/2+40);
}

void GpsWidget::drawSattelite(){
    GpsFramework & f = GpsFramework::Instance();
    QBrush lightGrayBrush(Qt::lightGray);
    auto last_frame = f.m_lastGGAFrame;
    scene->addRect(m_width-100, m_height-90, 100, 90, m_penBlack, lightGrayBrush);
    QString s = QString::number(last_frame.m_nbrSat) + " satellites";
    auto textItem = scene->addText(s);
    textItem->setPos(m_width-100, m_height-90);
    
    if(last_frame.m_fix == 1){
        s = "GPS";
    } else if(last_frame.m_fix == 2){
        s = "DGPS";
    } else {
        s = "invalid";
    }
    textItem = scene->addText(s);
    textItem->setPos(m_width-100, m_height-75);
    
    s = "hdop " + QString::number(last_frame.m_hdop, 'f', 2);
    textItem = scene->addText(s);
    textItem->setPos(m_width-100, m_height-60);

    s = "altitude " + QString::number(last_frame.m_altitude);
    textItem = scene->addText(s);
    textItem->setPos(m_width-100, m_height-40);
    
    int h = last_frame.m_time/10000;
    int min = (int)(last_frame.m_time/100) - h*100;
    double sec = last_frame.m_time - h*10000 - min*100;
    s = QString::number(h) + ":" + QString::number(min) + ":" + QString::number(sec, 'f', 2);
    textItem = scene->addText(s);
    textItem->setPos(m_width-100, m_height-25);
    
    int time = last_frame.m_time;
    //INFO(time);
}

void GpsWidget::drawContour(double x, double y){
    GpsFramework & f = GpsFramework::Instance();
    double last_x = 0, last_y;
    for(auto l : f.m_contour){
        double x1 = m_width/2+(l->m_x-x)*m_zoom;
        double y1 = m_height/2-(l->m_y-y)*m_zoom;
        if(last_x != 0){
            scene->addLine(x1,y1, last_x, last_y, m_penRed);
        }
        last_x = x1; last_y = y1;
    }
    
}


void GpsWidget::draw(){
    //scene = s;
    bool force = false;
    auto begin = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = begin - last_update;
    if(!force && diff.count() < 0.2){
   //     INFO(diff.count());
        return;
    }
    //INFO(diff.count());
    draw_force();
}

void GpsWidget::draw_force(){
    auto begin = std::chrono::system_clock::now();
    GpsFramework & f = GpsFramework::Instance();
    m_widthMax = m_width/2+f.m_config.m_largeur*m_zoom/2;
    m_heightMax = m_height/2+f.m_config.m_largeur*m_zoom/2;
    
    double h = m_height;
    double w = m_width;
    //INFO(w << " " << h);
    
    scene->clear();
    
    double x = 0;
    double y = 0;
    if(f.m_list.size() > 2){
        auto last_frame = (*f.m_list.begin());
        x = last_frame->m_x;
        y = last_frame->m_y;
       
        
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
            if(dist < l*l*0.2){
                continue;
            }
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
        
        scene->addLine(w/2, h/2, w/2 - f.m_deplacementX*m_zoom, h/2 + f.m_deplacementY*m_zoom, m_penBlue);
        
        //INFO(oss.str());
    }
    drawLines(x, y);
    
    
    if(f.m_pointA.isOk()){
        double xA  = (f.m_pointA.m_x - x)*m_zoom;
        double yA  = (f.m_pointA.m_y - y)*m_zoom;
        scene->addEllipse(w/2 + xA, h/2 - yA, 1, 1, m_penRed, m_brushNo);
    }
    if(f.m_pointB.isOk()){
        double xB  = (f.m_pointB.m_x - x)*m_zoom;
        double yB  = (f.m_pointB.m_y - y)*m_zoom;
        scene->addEllipse(w/2 + xB, h/2 - yB, 1, 1, m_penRed, m_brushNo);
    }
    if(f.m_pointA.isOk() && f.m_pointB.isOk()){
        double xA  = (f.m_pointA.m_x - x)*m_zoom;
        double yA  = (f.m_pointA.m_y - y)*m_zoom;
        double xB  = (f.m_pointB.m_x - x)*m_zoom;
        double yB  = (f.m_pointB.m_y - y)*m_zoom;
        
        //INFO("drawLine " << xA << " " << yA << " " <<  xB << " " << yB << " ");
        scene->addLine(w/2 + xA, h/2 - yA, w/2 + xB, h/2 - yB, m_penRed);
    }
    
    drawCourbe(1);
    drawCourbe(10);
    //drawCourbe(100);
    
    drawBarreGuidage();
    drawVitesse();
    drawSurface();
    drawSattelite();
    drawContour(x, y);
    
    addButtons();
    
    QBrush lightGrayBrush(Qt::lightGray);
    scene->addRect(m_width-200, m_height-30, 100, 30, m_penBlack, lightGrayBrush);
    
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> diff2 = end - begin;
    std::ostringstream oss;
    oss << "draw " << diff2.count();
    drawText(oss.str(), m_width-190, m_height-20, 10, false);
    
    last_update = end;
    if(!m_optionsWidget.m_close){
        m_optionsWidget.draw();
    }
    
}

void GpsWidget::addButtons(){
    for(auto button : m_buttons){
        if(button.m_id == BUTTON_A){
            scene->addEllipse(button.m_x-20, button.m_y-20, 40, 40, QPen(QColor(0,0,0)), QBrush(QColor(100, 100, 200)));
            drawText("A", button.m_x, button.m_y-12, 15, true);
        } else if(button.m_id == BUTTON_B){
            scene->addEllipse(button.m_x-20, button.m_y-20, 40, 40, QPen(QColor(0,0,0)), QBrush(QColor(100, 100, 200)));
            drawText("B", button.m_x, button.m_y-12, 15, true);
        } else if(button.m_id == BUTTON_CLOSE){
            scene->addEllipse(button.m_x-20, button.m_y-20, 40, 40, QPen(QColor(0,0,0)), QBrush(QColor(200, 0, 0)));
            drawText("X", button.m_x, button.m_y-12, 15, true);
        } else if(button.m_id == BUTTON_OPTION){
            scene->addEllipse(button.m_x-20, button.m_y-20, 40, 40, QPen(QColor(0,0,0)), QBrush(QColor(0, 200, 0)));
            drawText("O", button.m_x, button.m_y-12, 15, true);
        } else if(button.m_id == BUTTON_ZOOM_UP){
            scene->addEllipse(button.m_x-20, button.m_y-20, 40, 40, QPen(QColor(0,0,0)), QBrush(QColor(100, 100, 100)));
            drawText("+", button.m_x, button.m_y-12, 15, true);
        } else if(button.m_id == BUTTON_ZOOM_DOWN){
            scene->addEllipse(button.m_x-20, button.m_y-20, 40, 40, QPen(QColor(0,0,0)), QBrush(QColor(100, 100, 100)));
            drawText("-", button.m_x, button.m_y-12, 15, true);
        }
    }
    
    std::ostringstream out;
    out << "x " << m_zoom;
    drawText(out.str(), 30, m_height - 95, 15, false);
}

void GpsWidget::onButton(const Button & button){
    if(button.m_id == BUTTON_A){
        GpsFramework::Instance().savePointA();
    } else if(button.m_id == BUTTON_B){
        GpsFramework::Instance().savePointB();
    } else if(button.m_id == BUTTON_CLOSE){
        exit(0);
    } else if(button.m_id == BUTTON_OPTION){
        m_optionsWidget.open();
    } else if(button.m_id == BUTTON_ZOOM_UP){
        m_zoom *= 1.2;
        m_zoom = std::round(m_zoom*10.0)/10.0;
    } else if(button.m_id == BUTTON_ZOOM_DOWN){
        m_zoom /= 1.2;
        m_zoom = std::round(m_zoom*10.0)/10.0;
    }
    draw();
}

