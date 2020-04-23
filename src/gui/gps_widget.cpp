#include "gps_widget.hpp"

#include <sstream>
#include <fstream>
#include <math.h>
#include <cmath>
#include <chrono>
#include <iomanip>

#include "../logging.hpp"
#include "../gps_framework.hpp"
#include "environnement.hpp"


#define MY_WIDTH 800
#define MY_HEIGTH 400

#define BUTTON_A 0
#define BUTTON_B 1
#define BUTTON_ZOOM_UP 2
#define BUTTON_ZOOM_DOWN 3
#define BUTTON_CLOSE 4
#define BUTTON_OPTION 5

QGraphicsPixmapItem * item;
QPixmap * p;

GpsWidget::GpsWidget()
:m_optionsWidget()
{
    m_zoom = 10;
    
    
    m_penBlack = QPen(Qt::black);
    m_penRed = QPen(Qt::red);
    m_penBlue = QPen(Qt::blue);
    m_penBlue2 = QPen(Qt::blue, 3);
    
    m_grayBrush = QBrush (QColor(160,160,160));
    
    m_lightGrayBrush = QBrush (QColor(60,60,60));
    m_lightGrayDebugBrush = QBrush (QColor(120,120,120));
    m_greenBrush = QBrush (Qt::green);
    
    m_brushGreen = QBrush(QColor(0, 150, 0, 100));
    m_brushGreenTractor = QBrush(QColor(0, 150, 0));
    m_penNo.setColor(QColor(0, 250, 0, 0));
    //m_brushGreen.a
    
    double temp = 0.05;
    m_buttonClose  = new ButtonGui(1-temp, temp, GROS_BUTTON, 0);
    m_buttonA  = new ButtonGui(1-temp, 0.3, GROS_BUTTON, 0);
    m_buttonB  = new ButtonGui(1-temp, 0.4, GROS_BUTTON, 0);
    
    m_buttonPlus  = new ButtonGui(1-temp, 0.6, GROS_BUTTON, 0);
    m_buttonMinus  = new ButtonGui(1-temp, 0.7, GROS_BUTTON, 0);
    
    m_buttonOption  = new ButtonGui(temp, temp, GROS_BUTTON, 0);
    
    m_imgClose = loadImage("/images/close.png");
    m_imgPlus = loadImage("/images/plus.png");
    m_imgMinus = loadImage("/images/minus.png");
    m_imgA = loadImage("/images/a.png");
    m_imgB = loadImage("/images/b.png");
    m_imgOption = loadImage("/images/option.png");
    m_imgSat = loadImage("/images/sat.png");

    m_imgFleche = loadImage("/images/fleche.png");


}

void GpsWidget::setSize(int width, int height){
    INFO(width << " " << height);
    m_width = width;
    m_height = height;
    m_widthMax = m_width/2-50;
    m_heightMax = m_height/2-50;
    
    m_buttons.clear();
    
    
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
    for(int i = 0; i < 100; ++i){
        if(! addligne((i0 + i)*f.m_config.m_largeur, x, y, i0 + i)){
            break;
        }
    }
    for(int i = 1; i < 100; ++i){
        if(! addligne((i0 - i)*f.m_config.m_largeur, x, y, i0 - i)){
            break;
        }
    }
}

double cosA;
double sinA;
void projete(double x, double y, double & x_res, double & y_res){
    
    x_res = x;
    y_res = y;
    
    x_res = x*cosA +y*sinA;
    y_res  = -x*sinA +y*cosA;
    
}


bool GpsWidget::addligne(double l, double x, double y, int i){
    GpsFramework & f = GpsFramework::Instance();
    
    double a = f.m_a;
    double b = f.m_b;
    
    double res = l*b/cos(atan(-a/b));
    double x0 = -m_width/(m_zoom*2) + x;
    double y0 = -(a * x0 + f.m_c + res)/b;
    x0 = (x0 - x)*m_zoom;
    y0 = (y0 - y)*m_zoom;
    double x1 = m_width/(m_zoom*2) + x;
    double y1 = -(a * x1 + f.m_c + res)/b;
    x1 = (x1 - x)*m_zoom;
    y1 = (y1 - y)*m_zoom;

    if(y0 > m_height/2 && y1 > m_height/2){
        return false;
    }
    
    if(y0 < -m_height/2 && y1 < -m_height/2){
        return false;
    }
    
    double x02;
    double y02;
    double x12;
    double y12;
    
    projete(x0,y0,x02, y02);
    projete(x1,y1,x12, y12);
    
    QString s = QString::number(i);
    auto textItem = scene->addText(s);
    auto mBounds = textItem->boundingRect();
    textItem->setPos(m_width/2 + (x12+x02)/2 - mBounds.width()/2, m_height/2 - (y12+y02)/2  - mBounds.height()/2);
    //INFO(x1 << " " << x0);
    
    
    if(i%10 == 0){
        scene->addLine(m_width/2 + x02, m_height/2 - y02, m_width/2 + x12, m_height/2 - y12, m_penBlue);
    } else {
        scene->addLine(m_width/2 + x02, m_height/2 - y02, m_width/2 + x12, m_height/2 - y12, m_penBlack);
    }
    
    return true;
    
}

auto last_update = std::chrono::system_clock::now();

void GpsWidget::drawBarreGuidage(){
    GpsFramework & f = GpsFramework::Instance();
     
    scene->addRect(m_width/2-300, 0, 600, 40, m_penBlack, m_lightGrayBrush);
    scene->addRect(m_width/2-50, 5, 100, 30, m_penBlack, m_grayBrush);
    QString s = QString::number(f.m_distanceAB, 'f', 2) + " m";
    auto textItem = scene->addText(s);
    auto mBounds = textItem->boundingRect();
    textItem->setPos(m_width/2 - mBounds.width()/2, 10);
    for(int i = 0; i < 8; ++i){
        scene->addRect(m_width/2 - 80 - 30*i, 10, 20, 20, m_penBlack, m_grayBrush);
        scene->addRect(m_width/2 + 60 + 30*i, 10, 20, 20, m_penBlack, m_grayBrush);
    }
    if(f.m_ledAB > 0){
        for(int i = 0; i < std::min(8, f.m_ledAB); ++i){
            scene->addRect(m_width/2 - 80 - 30*i, 10, 20, 20, m_penBlack, m_greenBrush);
        }
    } else {
        for(int i = 0; i < std::min(8, -f.m_ledAB); ++i){
            scene->addRect(m_width/2 + 60 + 30*i, 10, 20, 20, m_penBlack, m_greenBrush);
        }
    }
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
        //return;
    }
    //INFO(diff.count());
    draw_force();
}

void GpsWidget::draw_force(){
    auto begin = std::chrono::system_clock::now();
    GpsFramework & f = GpsFramework::Instance();
    m_widthMax = m_width/2+f.m_config.m_largeur*m_zoom/2;
    m_heightMax = m_height/2+f.m_config.m_largeur*m_zoom/2;
    
    m_la = f.m_a;
    m_lb = f.m_b;
    m_lc = f.m_c;
    
    m_a = f.m_deplacementAngle;
    
    if(f.m_deplacementX>0){
        m_a = m_a-1.57;
    } else {
        m_a = m_a+1.57;
    }
    if(!f.m_config.m_sensDraw){
        m_a = 0;
    }
    //
    //INFO(f.m_deplacementY << " " << f.m_deplacementX << " " << (a/(2*3.14)*360));
    
    m_cosA = cos(m_a);
    m_sinA = sin(m_a);
    
    cosA = m_cosA;
    sinA = m_sinA;
    
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
            double x_temp  = (frame->m_x - x)*m_zoom;
            double y_temp  = (frame->m_y - y)*m_zoom;
            
            double x0;
            double y0;
            
            projete(x_temp, y_temp, x0, y0);
            
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
        
        double xA2;
        double yA2;
        double xB2;
        double yB2;
        
        projete(xA, yA, xA2, yA2);
        projete(xB, yB, xB2, yB2);
        
        
        //INFO("drawLine " << xA << " " << yA << " " <<  xB << " " << yB << " ");
        scene->addLine(w/2 + xA2, h/2 - yA2, w/2 + xB2, h/2 - yB2, m_penRed);
    }
    
    //drawCourbe(1);
    //drawCourbe(10);
    //drawCourbe(100);
    
    drawBarreGuidage();
    drawContour(x, y);
    drawTracteur();
    drawBottom();
    
    addButtons();
    
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> diff2 = end - begin;
    if(f.m_config.m_debug){
        int x = m_width-100;
        int y = m_height-70;
        
        QBrush lightGrayBrush(Qt::lightGray);
        scene->addRect(x, y, 100, 30, m_penBlack, lightGrayBrush);
        std::ostringstream oss;
        oss << "draw " << diff2.count();
        drawText(oss.str(), x+10, y+10, 10, false);
    
        drawDebug();
    }
    
    last_update = end;
    if(!m_optionsWidget.m_close){
        m_optionsWidget.draw();
    }
    
}

void GpsWidget::drawTracteur(){
    double h = m_height;
    double w = m_width;
    GpsFramework & f = GpsFramework::Instance();
    
    double x;
    double y;
    
    projete(f.m_deplacementX*m_zoom, f.m_deplacementY*m_zoom, x, y);
    
    
    scene->addLine(w/2, h/2, w/2 + x, h/2 - y, m_penBlue);
    
    double l = f.m_config.m_largeur*m_zoom/2;
    
    double xA;
    double yA;
    if(y==0){
        xA=0;
        yA=l;
    } else {
        double res = x/y;
        xA = l/(sqrt(1+res*res));
        yA = -xA*res;
    }
    
    
    scene->addLine(w/2+xA, h/2-yA, w/2 - xA, h/2 + yA, m_penBlue2);
    
    
    
    /*auto item = new QGraphicsPixmapItem(*m_imgFleche);
    item->setScale(0.4);
    item->setPos(w/2+10, h/2-50);
     item->setRotation(atan(x/y)/(2*3.14)*360);
     scene->addItem(item);*/
    
}

void GpsWidget::drawBottom(){
    GpsFramework & f = GpsFramework::Instance();
    
    scene->addRect(0, m_height-40, m_width, 40, m_penBlack, m_lightGrayBrush);
    
    auto item = new QGraphicsPixmapItem(*m_imgSat);
    item->setScale(0.4);
    item->setPos(5, m_height-45);
    scene->addItem(item);
    
    
    //scene->addRect(0, m_height-40, 80, 40, m_penBlack, m_lightGrayDebugBrush);
    QString s_vitesse = QString::number(f.m_vitesse, 'f', 2) + " km/h";
    auto textItems_vitesse = scene->addText(s_vitesse);
    auto mBounds = textItems_vitesse->boundingRect();
    textItems_vitesse->setDefaultTextColor(Qt::white);
    textItems_vitesse->setPos(m_width-60 - mBounds.width()/2, m_height-35);
    
    auto last_frame = f.m_lastGGAFrame;
    {
        QString s_vitesse = "hdop : " + QString::number(last_frame.m_hdop, 'f', 2);
        auto textItems_vitesse = scene->addText(s_vitesse);
        auto mBounds = textItems_vitesse->boundingRect();
        textItems_vitesse->setDefaultTextColor(Qt::white);
        textItems_vitesse->setPos(100 - mBounds.width()/2, m_height-35);
    }
    
    {
        QString s_vitesse = "sat : " + QString::number(last_frame.m_nbrSat);
        auto textItems_vitesse = scene->addText(s_vitesse);
        auto mBounds = textItems_vitesse->boundingRect();
        textItems_vitesse->setDefaultTextColor(Qt::white);
        textItems_vitesse->setPos(180 - mBounds.width()/2, m_height-35);
    }
    {
        std::string s;
        if(last_frame.m_fix == 1){
            s = "GPS";
        } else if(last_frame.m_fix == 2){
            s = "DGPS";
        } else {
            s = "invalid";
        }
        QString s_vitesse = QString::fromStdString(s);
        auto textItems_vitesse = scene->addText(s_vitesse);
        auto mBounds = textItems_vitesse->boundingRect();
        textItems_vitesse->setDefaultTextColor(Qt::white);
        textItems_vitesse->setPos(260 - mBounds.width()/2, m_height-35);
    }
    
    {
        QString s_vitesse = QString::fromStdString("licence non commerciale");
        auto textItems_vitesse = scene->addText(s_vitesse);
        auto mBounds = textItems_vitesse->boundingRect();
        textItems_vitesse->setDefaultTextColor(Qt::white);
        textItems_vitesse->setPos(500 - mBounds.width()/2, m_height-35);
    }
    
    /*scene->addRect(m_width/2-50, 5, 100, 30, m_penBlack, m_grayBrush);
    QString s = QString::number(f.m_distanceAB, 'f', 2) + " m";
    auto textItem = scene->addText(s);
    auto mBounds = textItem->boundingRect();
    textItem->setPos(m_width/2 - mBounds.width()/2, 10);
    for(int i = 0; i < 8; ++i){
        scene->addRect(m_width/2 - 80 - 30*i, 10, 20, 20, m_penBlack, m_grayBrush);
        scene->addRect(m_width/2 + 60 + 30*i, 10, 20, 20, m_penBlack, m_grayBrush);
    }
    if(f.m_ledAB > 0){
        for(int i = 0; i < std::min(8, f.m_ledAB); ++i){
            scene->addRect(m_width/2 - 80 - 30*i, 10, 20, 20, m_penBlack, m_greenBrush);
        }
    } else {
        for(int i = 0; i < std::min(8, -f.m_ledAB); ++i){
            scene->addRect(m_width/2 + 60 + 30*i, 10, 20, 20, m_penBlack, m_greenBrush);
        }
    }*/
}

void GpsWidget::drawDebug(){
    GpsFramework & f = GpsFramework::Instance();
    QBrush lightGrayBrush(Qt::lightGray);
    
    //surface
    {
        int x = 0;
        int y = m_height-300;
        
        scene->addRect(x, y, 100, 100, m_penBlack, lightGrayBrush);
        {
            QString s = QString::number(f.m_surface, 'f', 2) + " ha";
            auto textItem = scene->addText(s);
            auto mBounds = textItem->boundingRect();
            textItem->setPos(x, y);
        }
        QString s2 = QString::number(f.m_surface_h, 'f', 2) + " ha/h";
        auto textItem2 = scene->addText(s2);
        auto mBounds2 = textItem2->boundingRect();
        textItem2->setPos(x, y+15);
        {
            QString s = "exterieur";
            auto textItem = scene->addText(s);
            auto mBounds = textItem->boundingRect();
            textItem->setPos(x, y+40);
        }
        {
            QString s = QString::number(f.m_surface_exterieur, 'f', 2) + " ha";
            auto textItem = scene->addText(s);
            auto mBounds = textItem->boundingRect();
            textItem->setPos(x, y+55);
        }
        {
            QString s = QString::number(f.m_surface_exterieur_h, 'f', 2) + " ha/h";
            auto textItem = scene->addText(s);
            auto mBounds = textItem->boundingRect();
            textItem->setPos(x, y+70);
        }
    }
    
    //zoom
    {
        int x =  m_width-100;
        int y = m_height*0.6;
        std::ostringstream out;
        out << "x " << m_zoom;
        drawText(out.str(),x, y, 15, false);
    }
    
    //sattelite
    {
        int x = 0;
        int y = m_height-140;
        
        auto last_frame = f.m_lastGGAFrame;
        scene->addRect(x, y, 100, 100, m_penBlack, lightGrayBrush);
        QString s = QString::number(last_frame.m_nbrSat) + " satellites";
        auto textItem = scene->addText(s);
        textItem->setPos(x, y);

        if(last_frame.m_fix == 1){
         s = "GPS";
        } else if(last_frame.m_fix == 2){
         s = "DGPS";
        } else {
         s = "invalid";
        }
        textItem = scene->addText(s);
        textItem->setPos(x, y+15);

        s = "hdop " + QString::number(last_frame.m_hdop, 'f', 2);
        textItem = scene->addText(s);
        textItem->setPos(x, y+30);

        s = "altitude " + QString::number(last_frame.m_altitude);
        textItem = scene->addText(s);
        textItem->setPos(x, y+50);

        int h = last_frame.m_time/10000;
        int min = (int)(last_frame.m_time/100) - h*100;
        double sec = last_frame.m_time - h*10000 - min*100;
        s = QString::number(h) + ":" + QString::number(min) + ":" + QString::number(sec, 'f', 2);
        textItem = scene->addText(s);
        textItem->setPos(x, y+65);

        int time = last_frame.m_time;
    }
}

void GpsWidget::addButtons(){
    drawButtonImage(m_buttonPlus, *m_imgPlus);
    drawButtonImage(m_buttonMinus, *m_imgMinus);
    drawButtonImage(m_buttonA, *m_imgA);
    drawButtonImage(m_buttonB, *m_imgB);
    drawButtonImage(m_buttonOption, *m_imgOption);
    drawButtonImage(m_buttonClose, *m_imgClose);
}

void GpsWidget::onButton(const Button & button){
    
}

void GpsWidget::onMouse(int x, int y){
    for(auto button : m_buttons){
        if(x > button.m_x - 20  && x < button.m_x + 20 && y > button.m_y - 20  && y < button.m_y + 20){
            onButton(button);
        }
    }
    
    double x2 = x/m_width;
    double y2 = y/m_height;
    if(m_buttonClose->isActive(x2, y2)){
        exit(0);
    } else if(m_buttonPlus->isActive(x2, y2)){
        m_zoom *= 1.2;
        m_zoom = std::round(m_zoom*10.0)/10.0;
    } else if(m_buttonMinus->isActive(x2, y2)){
        m_zoom /= 1.2;
        m_zoom = std::round(m_zoom*10.0)/10.0;
    } else if(m_buttonA->isActive(x2, y2)){
        GpsFramework::Instance().savePointA();
    } else if(m_buttonB->isActive(x2, y2)){
        GpsFramework::Instance().savePointB();
    } else if(m_buttonOption->isActive(x2, y2)){
        m_optionsWidget.open();
        m_optionsWidget.m_close = false;
    }
    draw();
}

