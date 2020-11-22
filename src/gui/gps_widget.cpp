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
#define MY_HEIGHT 400

#define BUTTON_A 0
#define BUTTON_B 1
#define BUTTON_ZOOM_UP 2
#define BUTTON_ZOOM_DOWN 3
#define BUTTON_CLOSE 4
#define BUTTON_OPTION 5

QGraphicsPixmapItem * item;
QPixmap * p;

int l_bottom = 20;

GpsWidget::GpsWidget()
{
    m_zoom = 30;
    
    m_imgOk = loadImage("/images/ok.png");
    m_imgPlus = loadImage("/images/plus.png");
    m_imgMinus = loadImage("/images/minus.png");
    m_imgGuidage = loadImage("/images/guidage.png");
    m_imgParcelle = loadImage("/images/parcelle.png");
    m_imgOption = loadImage("/images/menu.png");
    m_imgSatBlanc = loadImage("/images/sat.png");
    m_imgSatVert = loadImage("/images/sat_vert.png");
    m_imgSatOrange = loadImage("/images/sat_orange.png");
    m_imgSatRouge = loadImage("/images/sat_rouge.png");
    
    m_imgChampBlanc = loadImage("/images/champ_blanc.png");
    m_imgChampVert = loadImage("/images/champ_vert.png");
    m_imgVolantRouge = loadImage("/images/volant_rouge.png");
    m_imgVolantVert = loadImage("/images/volant_vert.png");
    m_imgVolantBlanc = loadImage("/images/volant_blanc.png");
    m_imgVolantAutoGris = loadImage("/images/volant_auto_gris.png");
    m_imgVolantAutoVert = loadImage("/images/volant_auto_vert.png");
    
    m_imgFleche = loadImage("/images/fleche.png");
    
    m_widgets.push_back(&m_satWidget);
    m_widgets.push_back(&m_lineWidget);
    m_widgets.push_back(&m_lineNewWidget);
    m_widgets.push_back(&m_lineLoadWidget);
    m_widgets.push_back(&m_parcelleWidget);
    m_widgets.push_back(&m_parcelleNewWidget);
    m_widgets.push_back(&m_parcelleLoadWidget);
    m_widgets.push_back(&m_menuWidget);
    m_widgets.push_back(&m_key_pad_widget);
    m_widgets.push_back(&m_key_board_widget);
    
    m_menuWidget.m_machine_widget.m_key_pad_widget = &m_key_pad_widget;
    m_menuWidget.m_outil_widget.m_key_pad_widget = &m_key_pad_widget;
    m_parcelleNewWidget.m_key_board_widget = &m_key_board_widget;
    m_lineNewWidget.m_key_board_widget = &m_key_board_widget;
}

void GpsWidget::setSize(int width, int height){
    BaseWidget::setSize(width, height);
    
    m_widthMax = m_width/2-50;
    m_heightMax = m_height/2-50;
    
    for(auto p : m_widgets){
        p->setSize(m_width, m_height);
    }
    /*m_key_pad_widget.setSize(m_width, m_height);
     m_key_board_widget.setSize(m_width, m_height);*/
    
    double temp = 0.05;
    
    m_buttonSat.setResize(m_width-30, 20, m_gros_button);
    
    
    m_buttonPlus.setResize(temp*m_width, 0.60*m_height, m_gros_button);
    m_buttonMinus.setResize(temp*m_width, 0.70*m_height, m_gros_button);
    
    m_buttonOption.setResize(40, m_height-30, m_gros_button);
    m_buttonGuidage.setResize(100, m_height-30, m_gros_button);
    m_buttonParcelle.setResize(160, m_height-30, m_gros_button);
    if(GpsFramework::Instance().m_config.m_menu_vertical){
        m_imgGuidage = loadImage("/images/guidage2.png");
        m_imgParcelle = loadImage("/images/parcelle2.png");
        m_imgOption = loadImage("/images/menu2.png");
        m_buttonOption.setResize(temp*m_width, 0.20*m_height, m_gros_button);
        m_buttonGuidage.setResize(temp*m_width, 0.30*m_height, m_gros_button);
        m_buttonParcelle.setResize(temp*m_width, 0.40*m_height, m_gros_button);
        
    }
    
    m_buttonChamp.setResize((1-temp)*m_width, 0.35*m_height, m_gros_gros_button);
    m_buttonVolant.setResize((1-temp)*m_width, 0.55*m_height, m_gros_gros_button);
    m_buttonVolantAuto.setResize((1-temp)*m_width, 0.65*m_height, m_gros_gros_button);
    
    m_buttonErrorOk.setResize((0.5)*m_width, 0.8*m_height, m_gros_button);
    //    onValueChangeSlot(true);
}

GpsWidget * GpsWidget::Instance(){
    static GpsWidget gf;
    return &gf;
}

void GpsWidget::setScene(QGraphicsScene * s){
    BaseWidget::setScene(s);
    for(auto p : m_widgets){
        p->setScene(s);
    }
    /*m_key_pad_widget.setScene(s);
     m_key_board_widget.setScene(s);*/
}

void GpsWidget::my_projete(double x, double y, double & x_res, double & y_res){
    double x1_temp = (x - m_xref)*m_zoom;
    double y1_temp = (y - m_yref)*m_zoom;
    
    double h00 = m_cosA, h01 = m_sinA;//, h02 = 1;
    double h10 = -m_sinA, h11 = m_cosA;// h12 = 1;
    //double h20 = -m_sinA, h21 = m_cosA, h22 = 1;
    
    x_res = x1_temp*h00 +y1_temp*h01;
    y_res  = x1_temp*h10 +y1_temp*h11;
}

void GpsWidget::my_projete2(double x, double y, double & x_res, double & y_res){
    double x1_temp = (x - m_xref)*m_zoom;
    double y1_temp = (y - m_yref)*m_zoom;
    
    double h00 = m_cosA, h01 = m_sinA;//, h02 = 1;
    double h10 = -m_sinA, h11 = m_cosA;// h12 = 1;
    //double h20 = -m_sinA, h21 = m_cosA, h22 = 1;
    
    x_res = round(m_width/2 + x1_temp*h00 + y1_temp*h01);
    y_res  = round(m_height/2 - x1_temp*h10  - y1_temp*h11);
}

void GpsWidget::my_projete2_pt(GpsPoint_ptr pt, double & x_res, double & y_res){
    my_projete2(pt->m_x, pt->m_y, x_res, y_res);
}

bool GpsWidget::must_be_draw(double x, double y){
    double x_temp = (x - m_xref);
    double y_temp = (y - m_yref);
    double res = x_temp*x_temp+y_temp*y_temp;
    
    double l = m_width/m_zoom*0.9;
    return res<l*l;
}


void GpsWidget::drawCurve(Lines_ptr l, QPen & pen){
    if(!l){
        return;
    }
    double xA = 0, yA = 0;
    GpsPoint_ptr old_point = nullptr;
    for(auto p :l->m_points){
        if(must_be_draw(p->m_x, p->m_y)){
            double xB, yB;
            my_projete2(p->m_x, p->m_y, xB, yB);
            if(m_debug){
                scene->addEllipse(xB-1, yB-1, 2, 2, pen);
            }
            if(xA != 0 && yA != 0){
                scene->addLine(xA, yA, xB, yB, pen);
            }
            xA = xB, yA = yB;
        } else {
            xA = 0, yA = 0;
        }
    }
}

bool GpsWidget::addligne(double l, int i, QPen & pen){
    GpsFramework & f = GpsFramework::Instance();
    
    double a = f.m_lineAB.m_a;
    double b = f.m_lineAB.m_b;
    
    double res = l*b/cos(atan(-a/b));
    double x0 = -m_width/(m_zoom*2) + m_xref;
    double y0 = -(a * x0 + f.m_lineAB.m_c + res)/b;
    
    double x1 = m_width/(m_zoom*2) + m_xref;
    double y1 = -(a * x1 + f.m_lineAB.m_c + res)/b;
    
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
    
    /*QString s = QString::number(i);
    auto textItem = scene->addText(s);
    auto mBounds = textItem->boundingRect();
    textItem->setPos(m_width/2 + (x12+x02)/2 - mBounds.width()/2, m_height/2 - (y12+y02)/2  - mBounds.height()/2);
    //INFO(x1 << " " << x0);*/
    
    
    scene->addLine(m_width/2 + x02, m_height/2 - y02, m_width/2 + x12, m_height/2 - y12, pen);
    
    return true;
}

void GpsWidget::drawLines(){
    GpsFramework & f = GpsFramework::Instance();
    if(f.m_lineAB.m_a == 0 && f.m_lineAB.m_b ==0 && f.m_lineAB.m_c ==0){
        return;
    }
    //addligne(0, x, y);
    double x0 = m_xref;
    double y0 = m_yref;
    double res = -(y0*f.m_lineAB.m_b +(f.m_lineAB.m_a * x0 + f.m_lineAB.m_c));
    double l = res / (f.m_lineAB.m_b/cos(atan(-f.m_lineAB.m_a/f.m_lineAB.m_b)));
    //INFO("l " << l << " x " << x << " res " << res);
    
    
    int i0 = round(l/f.m_config.m_outil_largeur);
    addligne((i0)*f.m_config.m_outil_largeur, -(i0), m_penBlack);
    for(int i = 1; i < 5; ++i){
        if(! addligne((i0 + i)*f.m_config.m_outil_largeur, -(i0 + i), m_penGray)){
            break;
        }
    }
    for(int i = 1; i < 5; ++i){
        if(! addligne((i0 - i)*f.m_config.m_outil_largeur, -(i0 - i), m_penGray)){
            break;
        }
    }
}

void GpsWidget::drawLineCurve(){
    GpsFramework & f = GpsFramework::Instance();
    if(f.m_line){
        if(f.m_lineAB.m_pointA.m_isOk){
            double xA, yA;
            my_projete2(f.m_lineAB.m_pointA.m_x, f.m_lineAB.m_pointA.m_y, xA, yA);
            scene->addEllipse(xA-3, yA-3, 6, 6, m_penRed, m_brushNo);
            if(f.m_lineAB.m_pointB.m_isOk){
                double xB, yB;
                my_projete2(f.m_lineAB.m_pointB.m_x, f.m_lineAB.m_pointB.m_y, xB, yB);
                scene->addEllipse(xB-3, yB-3, 6, 6, m_penRed, m_brushNo);
                
                scene->addLine(xA, yA, xB, yB, m_penRed);
            }
        }
        
        drawLines();
    } else {
        if(f.m_etat == Etat_OK){
            auto list = f.m_curveAB.getCurrentLine();
            drawCurve(list, m_penBlack);
            for(int i = 1; i<3; ++i){
                auto list2 = f.m_curveAB.getCurrentLineRel(i);
                drawCurve(list2, m_penGray);
                auto list3 = f.m_curveAB.getCurrentLineRel(-i);
                drawCurve(list3, m_penGray);
            }
        }
    }
}



void GpsWidget::drawParcelle(bool force){
    GpsFramework & f = GpsFramework::Instance();
    if(force || f.m_parcelle.isInit()){
        QPolygon p;
        for(auto s: f.m_parcelle.m_contour){
            double x, y;
            my_projete2(s->m_x, s->m_y, x, y);
            if(m_debug){
                scene->addEllipse(x-2, y-2, 4, 4, m_penBlack);
            }
            p.push_back(QPoint(x, y));
        }
        if(force){
            scene->addPolygon(p, m_penBlack, m_parcelleBrush);
            for(auto i: f.m_parcelle.m_flag){
                double x, y;
                auto p = f.m_parcelle.m_contour[i];
                my_projete2(p->m_x, p->m_y, x, y);
                scene->addEllipse(x-3, y-3, 6, 6, m_penBlack, m_brushRed);
            }
        } else {
            scene->addPolygon(p, m_penNo, m_parcelleBrush);
        }
        
    }
}

void GpsWidget::drawSurfaceToDraw(){
    GpsFramework & f = GpsFramework::Instance();
    
    for(auto s: f.m_listSurfaceToDraw){
        if(s->m_points.size() > 0 && s->m_lastPoint){
            double xA1 = 0, yA1 = 0, xB1 = 0, yB1 = 0;
            my_projete2(s->m_lastPoint->m_point_left->m_x, s->m_lastPoint->m_point_left->m_y, xA1, yA1);
            my_projete2(s->m_lastPoint->m_point_right->m_x, s->m_lastPoint->m_point_right->m_y, xB1, yB1);
            
            
            int j = 0;
            int init = 1;
            
            for(auto it = s->m_points.begin(); it != s->m_points.end(); ++it){
                auto frame = (*it);
                
                
                if(!must_be_draw(frame->m_point_center->m_x, frame->m_point_center->m_y)){
                    init = 0;
                    //x1 = x0;
                    //y1 = y0;
                    continue;
                } else {
                    double xA = 0, yA = 0, xB = 0, yB = 0;
                    my_projete2(frame->m_point_left->m_x, frame->m_point_left->m_y, xA, yA);
                    my_projete2(frame->m_point_right->m_x, frame->m_point_right->m_y, xB, yB);
                    if(init != 0){
                        QPolygon polygon;
                        polygon << QPoint(xA, yA) << QPoint(xB, yB) << QPoint(xB1, yB1)<< QPoint(xA1, yA1);
                        if(m_debug){
                            scene->addPolygon(polygon, m_penBlack, m_brushGreen);
                        } else {
                            scene->addPolygon(polygon, m_penNo, m_brushGreen);
                        }
                    }
                    init = 1;
                    xA1 = xA, yA1 = yA, xB1 = xB, yB1 = yB;
                    
                }
                
                if(f.m_config.m_debug){
                    //scene->addEllipse(w/2 + x0, h/2 - y0, 2, 2, m_penBlack, m_brushNo);
                }
                j ++;
            }
            //INFO(oss.str());
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
    draw_force();
    DEBUG("END");
}



void GpsWidget::draw_force(){
    DEBUG("BEGIN");
    auto begin = std::chrono::system_clock::now();
    GpsFramework & f = GpsFramework::Instance();
    
    m_widthMax = m_width/2+f.m_config.m_outil_largeur*m_zoom/2;
    m_heightMax = m_height/2+f.m_config.m_outil_largeur*m_zoom/2;
    m_debug = f.m_config.m_debug;
    
    m_a = -f.m_deplacementAngle;
    if(!f.m_config.m_sensDraw){
        m_a = 0;
    }
    m_cosA = cos(m_a);
    m_sinA = sin(m_a);
    
    m_xref = f.m_tracteur.m_x_antenne;
    m_yref = f.m_tracteur.m_y_antenne;
    
    
    scene->clear();
    
    if(f.m_etat == Etat_ParcelleAdd || f.m_etat == Etat_ParcellePause){
        drawTracteur();
        drawParcelle(true);
    } else {
        
        drawParcelle();
        drawLineCurve();
        
        if(f.m_config.m_debug){
            for(auto p: f.m_list){
                double x1, y1;
                my_projete2(p->m_x, p->m_y, x1, y1);
                scene->addEllipse(x1, y1, 1, 1, m_penBlack, m_brushNo);
            }
        }
        drawSurfaceToDraw();
        
        drawContour();
        drawTracteur();
        drawTop();
        drawBottom();
        
        addButtons();
    }
    
    
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> diff2 = end - begin;
    if(f.m_config.m_debug){
        int x = m_width-200;
        int y = m_height-40-90-l_bottom;
        
        QBrush lightGrayBrush(Qt::lightGray);
        scene->addRect(x, y, 200, 90, m_penBlack, lightGrayBrush);
        {
            std::ostringstream oss;
            oss << "draw " << round(diff2.count()*1000) << " ms";
            drawText(oss.str(), x+10, y+10, sizeText_little, false);
        }
        {
            std::ostringstream oss;
            oss << "gps " << round(f.m_gps_time.m_moy) << " ms " << round(f.m_gps_time.m_et);
            drawText(oss.str(), x+10, y+30, sizeText_little, false);
        }
        {
            std::ostringstream oss;
            oss << "pil " << round(f.m_pilot_time.m_moy) << " ms " << round(f.m_pilot_time.m_et);
            drawText(oss.str(), x+10, y+50, sizeText_little, false);
        }
        //drawText(f.m_pilotModule.m_version_guidage, x+10, y+30, 10, false);
        
        drawDebug();
    }
    
    drawError();
    
    for(auto p : m_widgets){
        if(!p->m_close){
            p->draw();
        }
    }
    
   
    DEBUG("END");
    
}

void GpsWidget::drawTracteur(){
    GpsFramework & f = GpsFramework::Instance();
    int alpha = 255;
    if(f.m_config.m_debug){
        alpha = 155;
    }
    
    QColor color = QColor(0,0,200,alpha);
    QColor colorOutil = QColor(60,60,200,alpha);
    
    m_brushTractor = QBrush(color);
    m_brushOutil = QBrush(colorOutil);
    m_penOutil = QPen(colorOutil, 0.3*m_zoom);
    m_penTractorEssieu = QPen(color, 0.15*m_zoom);
    m_penTractorRoue = QPen(color, 0.3*m_zoom);
    
    
    double h = m_height;
    double w = m_width;
    
    double dx = f.m_vitesse*sin(f.m_deplacementAngle);
    double dy = f.m_vitesse*cos(f.m_deplacementAngle);
    
    double l2 = f.m_tracteur.m_antenne_essieu_avant;
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
    
    double l = 1.8*m_zoom/2;
    
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
    
    
    
    if(m_a){
        y = h/2;
        
        double l2 = f.m_tracteur.m_antenne_essieu_arriere*m_zoom;
        
        double y_arriere = y+l2;
        
        //outil
        double l_outil = f.m_tracteur.m_outil_distance;
        double lg_outil = f.m_tracteur.m_outil_largeur;
        scene->addRect(w/2 - lg_outil*0.5*m_zoom, y_arriere + (l_outil-0.5)*m_zoom, lg_outil*m_zoom, 0.5*m_zoom, m_penNo, m_brushOutil);
        scene->addRect(w/2 - 0.1*m_zoom, y_arriere, 0.2*m_zoom, (l_outil)*m_zoom, m_penNo, m_brushOutil);
        
        
        double voie = 1.8*m_zoom;
        double l_roue = 1.7*m_zoom/2;
        scene->addLine(w/2 - voie/2, y_arriere, w/2 + voie/2, y_arriere, m_penTractorEssieu);
        scene->addLine(w/2 - voie/2, y_arriere-l_roue, w/2 - voie/2, y_arriere+l_roue, m_penTractorRoue);
        scene->addLine(w/2 + voie/2, y_arriere-l_roue, w/2 + voie/2, y_arriere+l_roue, m_penTractorRoue);
        //scene->addRect(w/2, y_arriere - voie/2, y-4*l2, l2, 4*l2, m_penBlue);
        
        //scene->addRect(w/2 - voie/2, y_arriere-f.m_tracteur.m_empatement*m_zoom, voie, f.m_tracteur.m_empatement*m_zoom, m_penTractorEssieu);
        
        
        //scene->addRect(w/2 - l2/2, y-4*l2, l2, 4*l2, m_penTractorEssieu, m_brushTractor);
        
        //cabine
        double l_cabine = 1.6*m_zoom;
        scene->addRect(w/2 - l_cabine/2, y_arriere-1.2*m_zoom, l_cabine, l_cabine, m_penNo, m_brushTractor);
        
        //capot
        
        double lg_capot = 1*m_zoom;
        double l_capot = (f.m_tracteur.m_antenne_essieu_avant+0.2)*m_zoom;
        scene->addRect(w/2 - lg_capot/2, y-l_capot, lg_capot, l_capot, m_penNo, m_brushTractor);
        
        double y_direction = y-f.m_tracteur.m_antenne_essieu_avant*m_zoom;
        scene->addLine(w/2 - voie/2, y_direction, w/2 + voie/2, y_direction, m_penTractorEssieu);
        double l_roue2 = 1.1*m_zoom/2;
        
        for(int i = -1; i<=1; i+=2){
            int x=m_width/2+i*voie/2;
            int dx = -l_roue2*sin(f.m_angle_correction);
            int dy = l_roue2*cos(f.m_angle_correction);
            scene->addLine(x-dx, y_direction-dy, x+dx, y_direction+dy, m_penTractorRoue);
        }
        
        
        scene->addEllipse(w/2 - 0.10*m_zoom, h/2 - 0.10*m_zoom, 0.20*m_zoom, 0.20*m_zoom, m_penBlack, m_brushWhite);
        
        
        if(m_debug){
            /*my_projete2_pt(f.m_tracteur.m_pt_outil_arriere, x, y);
            scene->addEllipse(x-3, y-3, 6, 6, m_penRed, m_brushGreen);
            my_projete2_pt(f.m_tracteur.m_pt_outil_arriere_droite, x, y);
            scene->addEllipse(x-2, y-2, 4, 4, m_penRed, m_brushGreen);*/
        }
    } else {
        QPolygon polygon;
        polygon << QPoint(w/2 + xA, h/2 - yA) << QPoint(w/2 + x2, h/2 - y2) << QPoint(w/2 - xA, h/2 + yA)<< QPoint(w/2 + x, h/2 - y);
        scene->addPolygon(polygon, m_penNo, m_brushTractor);
        
        scene->addLine(w/2+xA, h/2-yA, w/2 - xA, h/2 + yA, m_penBlue);
        
        if(f.m_config.m_debug){
            my_projete(m_xref, m_yref, x, y);
            double a = -f.m_angle_correction;
            my_projete(m_xref+dx*cos(a)-dy*sin(a), m_yref+dx*sin(a)+dy*cos(a), x2, y2);
            scene->addLine(w/2+x, h/2-y, w/2 + x2, h/2 - y2, m_penBlue);
        }
        
    }
    if(m_zoom >= 60){
        y = h/2;
        if(f.m_config.m_debug){
            drawVolant(y);
        }
    }
    
    {
        double x_temp, y_temp;
        my_projete(f.m_tracteur.m_x_essieu_avant, f.m_tracteur.m_y_essieu_avant, x_temp, y_temp);
        //INFO(x_temp << " " << y_temp);
        //scene->addEllipse(w/2 + x_temp-5, h/2 - y_temp-5, 10, 10, m_penRed, m_grayBrush);
        
    }
    
}

const int l1 = 15;
void GpsWidget::drawTop(){
    GpsFramework & f = GpsFramework::Instance();
    
    scene->addRect(0, 0, m_width, 40, m_penBlack, m_brushDarkGray);
    scene->addRect(m_width/2-50, 5, 100, 30, m_penBlack, m_grayBrush);
    QString s = QString::number(f.m_distanceAB, 'f', 2) + " m";
    drawQText(s, m_width/2, 20,sizeText_medium, true);
    
    for(int i = 0; i < 8; ++i){
        scene->addRect(m_width/2 - 60 - 10 - l1*i, 10, 10, 20, m_penBlack, m_grayBrush);
        scene->addRect(m_width/2 + 60 + l1*i, 10, 10, 20, m_penBlack, m_grayBrush);
    }
    if(f.m_ledAB > 0){
        for(int i = 0; i < std::min(8, f.m_ledAB); ++i){
            scene->addRect(m_width/2 - 60 - 10 - l1*i, 10, 10, 20, m_penBlack, m_greenBrush);
        }
    } else {
        for(int i = 0; i < std::min(8, -f.m_ledAB); ++i){
            scene->addRect(m_width/2 + 60 + l1*i, 10, 10, 20, m_penBlack, m_greenBrush);
        }
    }
    
    {
        QString s = QString::number(f.m_vitesse, 'f', 1) + " km/h";
        drawQText(s, 7*m_width/8-40, 20,sizeText_medium, true, true);
    }
    
    {
        int current_line = 0;
        if(f.m_line){
            current_line = f.m_lineAB.m_current_line;
        } else {
            current_line = f.m_curveAB.m_i_current;
        }
        QString s = QString::number(f.m_surface, 'f', 2) + " ha    ( "+QString::number(current_line)+" )";
        drawQText(s, 1*m_width/8, 20,sizeText_medium, true, true);
    }
    
    auto last_frame = f.m_lastGGAFrame;
    QPixmap * img = m_imgSatBlanc;
    if(f.isGpsConnected()){
        
        if(last_frame.m_fix == 1 || last_frame.m_fix == 5){
            img = m_imgSatOrange;
        } else if(last_frame.m_fix == 2 || last_frame.m_fix == 4 || last_frame.m_fix == 9){
            img = m_imgSatVert;
        } else {
            img = m_imgSatRouge;
        }
    }
    
    drawButtonImage(&m_buttonSat, *img);
}


void GpsWidget::drawBottom(){
    //GpsFramework & f = GpsFramework::Instance();
    
    //auto last_frame = f.m_lastGGAFrame;
    scene->addRect(0, m_height-40-l_bottom, m_width, 40+l_bottom, m_penBlack, m_brushDarkGray);
    //int y_bottom = m_height-40-l_bottom+12;
    
    
    //scene->addRect(0, m_height-40, 80, 40, m_penBlack, m_brushLightGrayDebug);
    
    /*{
     QString s_vitesse = "hdop : " + QString::number(last_frame.m_hdop, 'f', 1);
     auto textItems_vitesse = scene->addText(s_vitesse);
     auto mBounds = textItems_vitesse->boundingRect();
     textItems_vitesse->setFont(QFont("Latin", 16, 1, false));
     textItems_vitesse->setDefaultTextColor(Qt::white);
     textItems_vitesse->setPos(80 - mBounds.width()/2, y_bottom);
     }*/
    
    
    
    
    /*{
     QString s = QString::fromStdString("licence non commerciale");
     drawQText(s, m_width/2, y_bottom, sizeText_little, true, true);
     }*/
    
    /*{
     QString s = QString::number(f.m_surface_h, 'f', 2) + " ha/h";
     auto textItem = scene->addText(s);
     auto mBounds = textItem->boundingRect();
     textItem->setFont(QFont("Latin", 16, 1, false));
     textItem->setDefaultTextColor(Qt::white);
     textItem->setPos(590 - mBounds.width()/2, y_bottom);
     }
     {
     QString s_vitesse = "cap : " + QString::number(f.m_deplacementAngle/3.14*180, 'f', 1);
     auto textItems_vitesse = scene->addText(s_vitesse);
     auto mBounds = textItems_vitesse->boundingRect();
     textItems_vitesse->setFont(QFont("Latin", 16, 1, false));
     textItems_vitesse->setDefaultTextColor(Qt::white);
     textItems_vitesse->setPos(m_width-135 - mBounds.width()/2, y_bottom);
     }*/
    
    /*if(!f.isGpsConnected()){
     int x = m_width/2;
     int y = m_height/2;
     scene->addRect(x-200, y-30, 400, 60, m_penBlack, m_grayBrush);
     {
     drawText("mauvaise réception GPS", x, y-10, sizeText_big, true);
     }
     }*/
}

void GpsWidget::drawVolant_(double y, double a, double r, double start_angle){
    double angle = a;
    int nbr_tour = 0;
    while(angle > 1.0){
        angle = angle - 1.0;
        nbr_tour++;
    }
    while(angle < -1.0){
        angle = angle + 1.0;
        nbr_tour--;
    }
    double j = 1.1;
    
    QGraphicsEllipseItem* item = new QGraphicsEllipseItem(m_width/2-r, y-r, 2*r, 2*r);
    item->setBrush(m_grayBrush);
    item->setPen(m_penNo);
    item->setStartAngle(90*16 - start_angle*360*16);
    
    item->setSpanAngle(-angle*360*16);
    scene->addItem(item);
    
    j=0.9;
    scene->addEllipse(m_width/2-r*j, y-r*j, 2*r*j, 2*r*j, m_penNo, m_brushWhite);
    
    {
        QString s = QString::number(a, 'f', 2);
        auto textItem = scene->addText(s);
        auto mBounds = textItem->boundingRect();
        
        textItem->setPos(m_width/2-mBounds.width()/2, y-r);
    }
}

void GpsWidget::drawVolant(double y){
    GpsFramework & f = GpsFramework::Instance();
    
    int r=40;
    
    double angle = f.m_pilotModule.m_volant;
    
    double j = 1.2;
    scene->addEllipse(m_width/2-r*j, y-r*j, 2*r*j, 2*r*j, m_penBlack, m_brushWhite);
    
    
    
    QGraphicsEllipseItem* item = new QGraphicsEllipseItem(m_width/2-r, y-r, 2*r, 2*r);
    item->setBrush(m_grayBrush);
    item->setPen(m_penNo);
    item->setStartAngle(90*16-f.m_pilotModule.m_volant0*360*16);
    item->setSpanAngle(-angle*360*16);
    scene->addItem(item);
    
    j=0.8;
    scene->addEllipse(m_width/2-r*j, y-r*j, 2*r*j, 2*r*j, m_penNo, m_brushWhite);
    {
        QString s = QString::number(f.m_pilotModule.m_volantTotal, 'f', 2);
        auto textItem = scene->addText(s);
        auto mBounds = textItem->boundingRect();
        
        textItem->setPos(m_width/2-mBounds.width()/2, y-r-10);
    }
    
    {
        drawVolant_(y, f.m_pilotModule.m_volantMesured, r-10, 0);
        
    }
    
    //double x1 = cos(i*2*3.14)*r;
    //double y1 = sin(i*2*3.14)*r;
    //scene->addRect(m_width/2+x1, y+y1, 2, 2);
    
}

void GpsWidget::drawDebug(){
    GpsFramework & f = GpsFramework::Instance();
    
    if(!f.m_line){
        {
            double x_h;
            double y_h;
            
            my_projete2(f.m_curveAB.x_h, f.m_curveAB.y_h, x_h, y_h);
            scene->addEllipse(x_h-1, y_h-1, 2, 2, m_penRed, m_brushNo);
        }
        
        {
            double x_h_pont;
            double y_h_pont;
            
            my_projete2(f.m_curveAB.x_h_pont, f.m_curveAB.y_h_pont, x_h_pont, y_h_pont);
            scene->addEllipse(x_h_pont-1, y_h_pont-1, 2, 2, m_penGreen, m_brushNo);
        }
        
        {
            double x_h_pont;
            double y_h_pont;
            
            my_projete2(f.m_curveAB.x_h_lookhead, f.m_curveAB.y_h_lookhead, x_h_pont, y_h_pont);
            scene->addEllipse(x_h_pont-1, y_h_pont-1, 2, 2, m_penGreen, m_brushNo);
        }
        
        auto list = f.m_curveAB.getCurrentLine();
        if(list && list->m_points.size()>3){
            {
                double x_h;
                double y_h;
                
                auto p = list->m_points[list->m_curve_i_min];
                my_projete2(p->m_x, p->m_y, x_h, y_h);
                scene->addEllipse(x_h, y_h, 8, 8, m_penBlue, m_brushNo);
            }
            
            {
                double x_h;
                double y_h;
                
                auto p = list->m_points[list->m_curve_i_min2];
                my_projete2(p->m_x, p->m_y, x_h, y_h);
                scene->addEllipse(x_h, y_h, 8, 8, m_penBlue, m_brushNo);
            }
        }
    }
    
    //surface
    {
        /*int x = 0;
         int y = m_height-320;
         
         scene->addRect(x, y, 100, 100, m_penBlack, lightGrayBrush);
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
        
        
        /*scene->addRect(x, y, 100, 120, m_penBlack, m_brushLightGrayDebug);
         
         {
         QString s = QString::number(f.m_angle_correction/3.14*180, 'f', 1) + " deg";
         auto textItem = scene->addText(s);
         textItem->setPos(x, y);
         }
         
         y+=20;
         {
         QString s = "goto " + QString::number(f.m_pilotModule.m_last_goto_tour, 'f', 1) + " tour";
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
         QString s = "V " + QString::number(f.m_pilotModule.m_volant, 'f', 1);
         auto textItem = scene->addText(s);
         textItem->setPos(x, y);
         }
         y+=20;
         {
         QString s = "LR " + QString::number(f.m_pilotModule.m_last_leftright, 'f', 1);
         auto textItem = scene->addText(s);
         textItem->setPos(x, y);
         }*/
        
    }
    
    //zoom
    {
        int x =  100;
        int y = m_height*0.6;
        std::ostringstream out;
        out << "x " << m_zoom;
        drawText(out.str(),x, y, sizeText_medium, false);
    }
    
    //bas gauche
    {
        /*int x = 0;
         int y = m_height-110-l_bottom;
         
         auto last_frame = f.m_lastGGAFrame;
         scene->addRect(x, y, 100, 70, m_penBlack, m_brushLightGrayDebug);*/
        
    }
    
    if(f.m_listLog.size() > 0){
        int l2 = 160;
        {
            int x = m_width-l2;
            int y = 00;
            
            scene->addRect(x, y, l2, m_height-100, m_penBlack, m_brushLightGrayDebug);
            
            int y_temp = m_height - 120;
            for(auto s:f.m_listLog){
                drawText(s,x+10, y_temp, sizeText_little, false);
                y_temp-=15;
            }
        }
    }
}

void GpsWidget::drawError(){
    int w = m_width/2;
    int h = m_height/2;
    
    int x = 50;
    int y = 50;
    
    GpsFramework & f = GpsFramework::Instance();
    if(f.m_config.m_debug){
        if(!f.m_messages_errors.empty()){
            scene->addRect(x, y, w, h, m_penBlack, m_brushLightGrayDebug);
            drawText("Erreurs", x+w/2, y+15, sizeText_big, true);
            drawText(f.m_messages_errors, x+20, y+40, sizeText_little, false);
            m_buttonErrorOk.m_x = (x+w/2);
            m_buttonErrorOk.m_y = (y+h-30);
            drawButtonImage(&m_buttonErrorOk, *m_imgOk);
            // m_pilot_serial_input
        }
    }
}

void GpsWidget::addButtons(){
    drawButtonImage(&m_buttonPlus, *m_imgPlus);
    drawButtonImage(&m_buttonMinus, *m_imgMinus);
    drawButtonImage(&m_buttonGuidage, *m_imgGuidage);
    drawButtonImage(&m_buttonParcelle, *m_imgParcelle);
    drawButtonImage(&m_buttonOption, *m_imgOption);
    
    GpsFramework & f = GpsFramework::Instance();
    if(f.m_pauseDraw){
        drawButtonImage(&m_buttonChamp, *m_imgChampBlanc);
    } else {
        drawButtonImage(&m_buttonChamp, *m_imgChampVert);
    }
    
    if(f.isPilotConnected()){
        if(f.getVolantEngaged()){
            drawButtonImage(&m_buttonVolant, *m_imgVolantVert);
        } else {
            drawButtonImage(&m_buttonVolant, *m_imgVolantBlanc);
        }
        
        
    } else {
        drawButtonImage(&m_buttonVolant, *m_imgVolantRouge);
    }
    if(f.m_config.m_pilot_auto_deactive > 0){
        if(f.m_pilot_auto){
            drawButtonImage(&m_buttonVolantAuto, *m_imgVolantAutoVert);
        } else {
            drawButtonImage(&m_buttonVolantAuto, *m_imgVolantAutoGris);
        }
    }
}


void GpsWidget::onMouse(int x, int y){
    size_t n = m_widgets.size();
    for(size_t i = 0; i < n; ++i){
        auto p = m_widgets[n-i-1];
        if(!p->m_close){
            p->onMouse(x, y);
            return;
        }
    }
    
    /*if(!m_key_pad_widget.m_close){
     m_key_pad_widget.onMouse(x, y);;
     }
     if(!m_key_board_widget.m_close){
     m_key_board_widget.onMouse(x, y);;
     }*/
    
    double x2 = x;
    double y2 = y;
    
    GpsFramework & f = GpsFramework::Instance();
    if(!f.m_messages_errors.empty()){
        if(m_buttonErrorOk.isActive(x2, y2)){
            f.m_messages_errors = "";
        }
    }
    if(m_buttonPlus.isActive(x2, y2)){
        m_zoom *= 1.2;
        m_zoom = std::round(m_zoom*10.0)/10.0;
    } else if(m_buttonMinus.isActive(x2, y2)){
        m_zoom /= 1.2;
        m_zoom = std::round(m_zoom*10.0)/10.0;
    } else if(m_buttonGuidage.isActive(x2, y2)){
        //m_parcelleWidget.open();
        m_lineWidget.m_close = false;
    } else if(m_buttonParcelle.isActive(x2, y2)){
        m_parcelleWidget.m_close = false;
    } else if(m_buttonOption.isActive(x2, y2)){
        m_menuWidget.m_close = false;
    } else if(m_buttonSat.isActive(x2, y2)){
        m_satWidget.m_close = false;
    } else if(m_buttonChamp.isActive(x2, y2)){
        GpsFramework::Instance().changeDraw();
    } else if(m_buttonVolant.isActive(x2, y2)){
        GpsFramework::Instance().setVolantEngaged(!GpsFramework::Instance().getVolantEngaged());
    } else if(f.m_config.m_pilot_auto_deactive > 0 && m_buttonVolantAuto.isActive(x2, y2)){
        GpsFramework::Instance().m_pilot_auto = !GpsFramework::Instance().m_pilot_auto;
    }
    draw();
}

