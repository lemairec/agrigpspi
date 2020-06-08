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
    
    double temp = 0.05;
    m_buttonClose  = new ButtonGui(1-temp, temp, GROS_BUTTON, 0);
    m_buttonA  = new ButtonGui(1-temp, 0.3, GROS_BUTTON, 0);
    m_buttonB  = new ButtonGui(1-temp, 0.4, GROS_BUTTON, 0);
    
    m_buttonPlus  = new ButtonGui(temp, 0.6, GROS_BUTTON, 0);
    m_buttonMinus  = new ButtonGui(temp, 0.7, GROS_BUTTON, 0);
    
    m_buttonOption  = new ButtonGui(temp, temp, GROS_BUTTON, 0);
    m_buttonChamp  = new ButtonGui(temp, 0.3, GROS_BUTTON, 0);
    m_buttonVolant  = new ButtonGui(1-temp, 0.65, GROS_BUTTON, 0);
    
    m_buttonErrorOk  = new ButtonGui(0.5, 0.8, GROS_BUTTON, 0);
    
    m_imgOk = loadImage("/images/ok.png");
    m_imgClose = loadImage("/images/close.png");
    m_imgPlus = loadImage("/images/plus.png");
    m_imgMinus = loadImage("/images/minus.png");
    m_imgA = loadImage("/images/a.png");
    m_imgB = loadImage("/images/b.png");
    m_imgOption = loadImage("/images/option.png");
    m_imgSat = loadImage("/images/sat.png");
    m_imgSatVert = loadImage("/images/sat_vert.png");
    m_imgSatOrange = loadImage("/images/sat_orange.png");
    m_imgSatRouge = loadImage("/images/sat_rouge.png");

    m_imgChampGris = loadImage("/images/champ_gris.png");
    m_imgChampVert = loadImage("/images/champ_vert.png");
    m_imgVolantGris = loadImage("/images/volant_gris.png");
    m_imgVolantVert = loadImage("/images/volant_vert.png");
    
    m_imgFleche = loadImage("/images/fleche.png");


}

void GpsWidget::setSize(int width, int height){
    INFO(width << " " << height);
    m_width = width;
    m_height = height;
    m_widthMax = m_width/2-50;
    m_heightMax = m_height/2-50;

    m_optionsWidget.setSize(m_width, m_height);
    
//    onValueChangeSlot(true);
}


void GpsWidget::drawCourbe(double l){
    scene->addEllipse(m_width/2 - l*m_zoom, m_height/2 - l*m_zoom, l*m_zoom*2, l*m_zoom*2, m_penBlack, m_brushNo);
    //INFO(m_height);
}


void GpsWidget::drawLines(){
    //addligne(0, x, y);
    GpsFramework & f = GpsFramework::Instance();
    double x0 = m_xref;
    double y0 = m_yref;
    double res = -(y0*f.m_b +(f.m_a * x0 + f.m_c));
    double l = res / (f.m_b/cos(atan(-f.m_a/f.m_b)));
    //INFO("l " << l << " x " << x << " res " << res);
    
    
    int i0 = round(l/f.m_config.m_largeur);
    for(int i = 0; i < 100; ++i){
        if(! addligne((i0 + i)*f.m_config.m_largeur, i0 + i)){
            break;
        }
    }
    for(int i = 1; i < 100; ++i){
        if(! addligne((i0 - i)*f.m_config.m_largeur, i0 - i)){
            break;
        }
    }
}

void GpsWidget::my_projete(double x, double y, double & x_res, double & y_res){
    double x1_temp = (x - m_xref)*m_zoom;
    double y1_temp = (y - m_yref)*m_zoom;
    
    x_res = x1_temp*m_cosA +y1_temp*m_sinA;
    y_res  = -x1_temp*m_sinA +y1_temp*m_cosA;
}

bool GpsWidget::addligne(double l, int i){
    GpsFramework & f = GpsFramework::Instance();
    
    double a = f.m_a;
    double b = f.m_b;
    
    double res = l*b/cos(atan(-a/b));
    double x0 = -m_width/(m_zoom*2) + m_xref;
    double y0 = -(a * x0 + f.m_c + res)/b;
    
    double x1 = m_width/(m_zoom*2) + m_xref;
    double y1 = -(a * x1 + f.m_c + res)/b;
    
    double x02;
    double y02;
    double x12;
    double y12;
    
    my_projete(x0, y0, x02, y02);
    my_projete(x1, y1, x12, y12);
    
    if(y02 > m_height/2 && y12 > m_height/2){
        return false;
    }
    
    if(y02 < -m_height/2 && y12 < -m_height/2){
        return false;
    }
    
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
     
    scene->addRect(m_width/2-300, 0, 600, 40, m_penBlack, m_brushDarkGray);
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

void GpsWidget::drawContour(){
    GpsFramework & f = GpsFramework::Instance();
    double x = m_xref;
    double y = m_yref;
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
    DEBUG("BEGIN");
    bool force = false;
    auto begin = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = begin - last_update;
    if(!force && diff.count() < 0.2){
   //     INFO(diff.count());
        return;
    }
    //INFO(diff.count());
    draw_force();
    DEBUG("END");
}



void GpsWidget::draw_force(){
    DEBUG("BEGIN");
    auto begin = std::chrono::system_clock::now();
    GpsFramework & f = GpsFramework::Instance();
    m_widthMax = m_width/2+f.m_config.m_largeur*m_zoom/2;
    m_heightMax = m_height/2+f.m_config.m_largeur*m_zoom/2;
    
    m_la = f.m_a;
    m_lb = f.m_b;
    m_lc = f.m_c;
    
    m_a = -f.m_deplacementAngle; //TODO etonnant!
    if(!f.m_config.m_sensDraw){
        m_a = 0;
    }
    
    if(f.m_list.size() >0){
        auto last_frame = *(f.m_list.begin());
        m_xref = last_frame->m_x;
        m_yref = last_frame->m_y;
    }
    //
    //INFO(f.m_deplacementY << " " << f.m_deplacementX << " " << (a/(2*3.14)*360));
    
    m_cosA = cos(m_a);
    m_sinA = sin(m_a);
    
    double h = m_height;
    double w = m_width;
    //INFO(w << " " << h);
    
    scene->clear();
    
    if(f.m_list.size() >0){
        for(auto s: f.m_listSurfaceToDraw){
            if(s->m_points.size() > 2){
                double x1, y1;
                my_projete(s->m_lastPoint->m_x, s->m_lastPoint->m_y, x1, y1);
                
                double xA1 = 0, yA1 = 0, xB1 = 0, yB1 = 0;
                
                double l = f.m_config.m_largeur*m_zoom/2;
                int j = 0;
                int init = 0;
                
                for(auto it = s->m_points.begin(); it != s->m_points.end(); ++it){
                    auto frame = (*it);
                    
                    double x0, y0;
                    my_projete(frame->m_x, frame->m_y, x0, y0);
                     
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
                    if(f.m_config.m_debug){
                        scene->addEllipse(w/2 + x0, h/2 - y0, 1, 1, m_penBlack, m_brushNo);
                    }
                    j ++;
                }
            //INFO(oss.str());
            }
        }
    }
    drawLines();
    
    
    if(f.m_pointA.m_isOk){
        double xA, yA;
        my_projete(f.m_pointA.m_x, f.m_pointA.m_y, xA, yA);
        scene->addEllipse(w/2 + xA, h/2 - yA, 5, 5, m_penRed, m_brushNo);
    }
    if(f.m_pointB.m_isOk){
        double xB, yB;
        my_projete(f.m_pointB.m_x, f.m_pointB.m_y, xB, yB);
        scene->addEllipse(w/2 + xB, h/2 - yB, 5, 5, m_penRed, m_brushNo);
    }
    if(f.m_pointA.m_isOk && f.m_pointB.m_isOk){
        double xA, yA;
        my_projete(f.m_pointA.m_x, f.m_pointA.m_y, xA, yA);
        double xB, yB;
        my_projete(f.m_pointB.m_x, f.m_pointB.m_y, xB, yB);
        
        
        //INFO("drawLine " << xA << " " << yA << " " <<  xB << " " << yB << " ");
        scene->addLine(w/2 + xA, h/2 - yA, w/2 + xB, h/2 - yB, m_penRed);
    }
    
    //drawCourbe(1);
    //drawCourbe(10);
    //drawCourbe(100);
    
    drawBarreGuidage();
    drawContour();
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
    
    drawError();
    
    last_update = end;
    if(!m_optionsWidget.m_close){
        m_optionsWidget.draw();
    }
    DEBUG("END");
    
}

void GpsWidget::drawTracteur(){
    double h = m_height;
    double w = m_width;
    GpsFramework & f = GpsFramework::Instance();
    
    double dx = f.m_vitesse*sin(f.m_deplacementAngle);
    double dy = f.m_vitesse*cos(f.m_deplacementAngle);
    
    double l2 = 5;
    double res = sqrt(dx*dx+dy*dy);
    dx = dx/res*l2;
    dy = dy/res*l2;
    
    double x;
    double y;
    
    double x2;
    double y2;
    
    //m_angle_correction
    
    my_projete(m_xref+dx, m_yref+dy, x, y);
    my_projete(m_xref+dx/4, m_yref+dy/4, x2, y2);
    
    
    //scene->addLine(w/2, h/2, w/2 + x, h/2 - y, m_penBlue);
    
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
    
    
     
    QPolygon polygon;
    polygon << QPoint(w/2 + xA, h/2 - yA) << QPoint(w/2 + x2, h/2 - y2) << QPoint(w/2 - xA, h/2 + yA)<< QPoint(w/2 + x, h/2 - y);
    scene->addPolygon(polygon, m_penNo, m_brushGreenTractor);
    
    scene->addLine(w/2+xA, h/2-yA, w/2 - xA, h/2 + yA, m_penBlue);
    
    if(f.m_config.m_debug){
        my_projete(m_xref, m_yref, x, y);
        double a = -f.m_angle_correction;
        my_projete(m_xref+dx*cos(a)-dy*sin(a), m_yref+dx*sin(a)+dy*cos(a), x2, y2);
        scene->addLine(w/2+x, h/2-y, w/2 + x2, h/2 - y2, m_penBlue);
    }
    
}

void GpsWidget::drawBottom(){
    GpsFramework & f = GpsFramework::Instance();
    
    scene->addRect(0, m_height-40, m_width, 40, m_penBlack, m_brushDarkGray);
    
    auto last_frame = f.m_lastGGAFrame;
    if(f.isGpsConnected()){
        QPixmap * img;
        if(last_frame.m_fix == 1 || last_frame.m_fix == 5){
           img = m_imgSatOrange;
        } else if(last_frame.m_fix == 2 || last_frame.m_fix == 4){
           img = m_imgSatVert;
        } else {
           img = m_imgSatRouge;
        }

        auto item = new QGraphicsPixmapItem(*img);
        item->setScale(0.4);
        item->setPos(5, m_height-45);
        scene->addItem(item);
    }
    
    {
        QString s_vitesse = "cap : " + QString::number(f.m_deplacementAngle/3.14*180, 'f', 1);
        auto textItems_vitesse = scene->addText(s_vitesse);
        auto mBounds = textItems_vitesse->boundingRect();
        textItems_vitesse->setDefaultTextColor(Qt::white);
        textItems_vitesse->setPos(m_width-135 - mBounds.width()/2, m_height-35);
    }
    
    //scene->addRect(0, m_height-40, 80, 40, m_penBlack, m_brushLightGrayDebug);
    QString s_vitesse = QString::number(f.m_vitesse, 'f', 1) + " km/h";
    auto textItems_vitesse = scene->addText(s_vitesse);
    auto mBounds = textItems_vitesse->boundingRect();
    textItems_vitesse->setDefaultTextColor(Qt::white);
    textItems_vitesse->setPos(m_width-50 - mBounds.width()/2, m_height-35);
    
    {
        QString s_vitesse = "hdop : " + QString::number(last_frame.m_hdop, 'f', 1);
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
        } else if(last_frame.m_fix == 3){
            s = "PPSFIX";
        } else if(last_frame.m_fix == 4){
            s = "RTK";
        } else if(last_frame.m_fix == 5){
            s = "FRTK";
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
        textItems_vitesse->setPos(420 - mBounds.width()/2, m_height-35);
    }
    
    {
        QString s = QString::number(f.m_surface, 'f', 2) + " ha";
        auto textItem = scene->addText(s);
        auto mBounds = textItem->boundingRect();
        textItem->setDefaultTextColor(Qt::white);
        textItem->setPos(580 - mBounds.width()/2, m_height-35);
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
    
    if(last_frame.m_fix < 1){
        int x = m_width/2;
        int y = m_height/2;
        scene->addRect(x-200, y-30, 400, 60, m_penBlack, m_grayBrush);
        {
            drawText("mauvaise réception GPS", x, y-10, 20, true);
        }
    }
}

void GpsWidget::drawDebug(){
    GpsFramework & f = GpsFramework::Instance();
    QBrush lightGrayBrush(Qt::lightGray);
    
    //surface
    {
        int x = 0;
        int y = m_height-320;
        
        /*scene->addRect(x, y, 100, 100, m_penBlack, lightGrayBrush);
        {
            QString s = QString::number(f.m_pilotModule.m_0, 'f', 2) + " m_0";
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
        }*/
        
        scene->addRect(x, y, 100, 120, m_penBlack, lightGrayBrush);
        
        {
            QString s = QString::number(f.m_angle_correction/3.14*180, 'f', 1) + " deg";
            auto textItem = scene->addText(s);
            textItem->setPos(x, y);
        }
        
        y+=20;
        {
            QString s = "m_0 " + QString::number(f.m_pilotModule.m_0/3.14*180, 'f', 1);
            auto textItem = scene->addText(s);
            textItem->setPos(x, y);
        }
        
        y+=15;
        {
            QString s = "goto " + QString::number(f.m_pilotModule.m_last_goto*100, 'f', 1);
            auto textItem = scene->addText(s);
            textItem->setPos(x, y);
        }
        
        y+=20;
        {
            QString s = "H0 " + QString::number(f.m_pilotModule.m_hadrien0, 'f', 1);
            auto textItem = scene->addText(s);
            textItem->setPos(x, y);
        }
        y+=15;
        {
            QString s = "H " + QString::number(f.m_pilotModule.m_hadrienVolant, 'f', 1);
            auto textItem = scene->addText(s);
            textItem->setPos(x, y);
        }
        y+=20;
       {
           QString s = "LR " + QString::number(f.m_pilotModule.m_last_leftright, 'f', 1);
           auto textItem = scene->addText(s);
           textItem->setPos(x, y);
       }
        
    }
    
    //zoom
    {
        int x =  100;
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

        s = QString::number(last_frame.m_fix)+ " ";
        if(last_frame.m_fix == 1){
            s += "GPS";
        } else if(last_frame.m_fix == 2){
            s += "DGPS";
        } else if(last_frame.m_fix == 3){
            s += "PPSFIX";
        } else if(last_frame.m_fix == 4){
            s += "RTK";
        } else if(last_frame.m_fix == 5){
            s += "FRTK";
        } else {
            s += "invalid";
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

void GpsWidget::drawError(){
    GpsFramework & f = GpsFramework::Instance();
    if(f.m_config.m_debug){
        if(!f.m_messages_errors.empty()){
            scene->addRect(50, 50, m_width-100, m_height-100, m_penBlack, m_brushLightGrayDebug);
            drawText("Erreurs", 0.5*m_width, 80, 20, true);
            drawText(f.m_messages_errors, 0.1*m_width, 0.3*m_height, 10, false);
            drawButtonImage(m_buttonErrorOk, *m_imgOk);
           // m_pilot_serial_input
        }
    }
}

void GpsWidget::addButtons(){
    drawButtonImage(m_buttonPlus, *m_imgPlus);
    drawButtonImage(m_buttonMinus, *m_imgMinus);
    drawButtonImage(m_buttonA, *m_imgA);
    drawButtonImage(m_buttonB, *m_imgB);
    drawButtonImage(m_buttonOption, *m_imgOption);
    drawButtonImage(m_buttonClose, *m_imgClose);
    
    GpsFramework & f = GpsFramework::Instance();
    if(f.m_pauseDraw){
        drawButtonImage(m_buttonChamp, *m_imgChampGris);
    } else {
        drawButtonImage(m_buttonChamp, *m_imgChampVert);
    }
    
    if(f.getVolantEngaged()){
        drawButtonImage(m_buttonVolant, *m_imgVolantVert);
    } else {
        drawButtonImage(m_buttonVolant, *m_imgVolantGris);
    }
}


void GpsWidget::onMouse(int x, int y){
    double x2 = x/m_width;
    double y2 = y/m_height;
    
    GpsFramework & f = GpsFramework::Instance();
    if(!f.m_messages_errors.empty()){
        if(m_buttonErrorOk->isActive(x2, y2)){
            f.m_messages_errors = "";
        }
    }
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
    } else if(m_buttonChamp->isActive(x2, y2)){
        GpsFramework::Instance().changeDraw();
    } else if(m_buttonVolant->isActive(x2, y2)){
        GpsFramework::Instance().setVolantEngaged(!GpsFramework::Instance().getVolantEngaged());
    }
    draw();
}

