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
    m_tracteur_2d = loadImage("/images/tracteur_2d.png");
    m_tracteur_3d = loadImage("/images/tracteur_3d.png");
    
    //m_widgets.push_back(&m_satWidget);
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
    
    m_vue_3D = false;
}

void GpsWidget::setSize(int width, int height){
    BaseWidget::setSize(width, height);
    
    m_widthMax = m_width/2-50;
    m_heightMax = m_height/2-50;
    
    for(auto p : m_widgets){
        p->setSize(m_width, m_height);
    }
    m_infosWidget.setSize(m_width, m_height);
    /*m_key_pad_widget.setSize(m_width, m_height);
     m_key_board_widget.setSize(m_width, m_height);*/
    
    double temp = 0.05;
    
    m_buttonSat.setResize(m_width-30, 20, m_gros_button);
    
    
    m_buttonPlus.setResize(temp*m_width, 0.60*m_height, m_gros_button);
    m_buttonMinus.setResize(temp*m_width, 0.70*m_height, m_gros_button);
    
    m_buttonOption.setResize(40, m_height-30, m_gros_button);
    m_buttonGuidage.setResize(100, m_height-30, m_gros_button);
    m_buttonParcelle.setResize(160, m_height-30, m_gros_button);
    m_buttonChamp.setResize(m_width - 160, m_height-30, m_gros_gros_button);
    m_buttonVolant.setResize(m_width - 40, m_height-30, m_gros_gros_button);
    m_buttonVolantAuto.setResize(m_width - 100, m_height-30, m_gros_gros_button);
    m_button3d.setResize(m_width/2, m_height/2, m_gros_gros_button);
    
    GpsFramework & f = GpsFramework::Instance();
    if(f.m_config.m_menu_vertical){
        m_imgGuidage = loadImage("/images/guidage2.png");
        m_imgParcelle = loadImage("/images/parcelle2.png");
        m_imgOption = loadImage("/images/menu2.png");
        m_buttonOption.setResize(temp*m_width, 0.20*m_height, m_gros_button);
        m_buttonGuidage.setResize(temp*m_width, 0.30*m_height, m_gros_button);
        m_buttonParcelle.setResize(temp*m_width, 0.40*m_height, m_gros_button);
        m_buttonChamp.setResize((1-temp)*m_width, 0.35*m_height, m_gros_gros_button);
        m_buttonVolant.setResize((1-temp)*m_width, 0.55*m_height, m_gros_gros_button);
        m_buttonVolantAuto.setResize((1-temp)*m_width, 0.65*m_height, m_gros_gros_button);
    }
    
    
    m_buttonErrorOk.setResize((0.5)*m_width, 0.8*m_height, m_gros_button);
    
    //f.m_lines.loadCurveOrLine("line_HADRIEN3");
    //f.loadParcelle("CANON", 0, true, false);  
}

GpsWidget * GpsWidget::Instance(){
    static GpsWidget gf;
    return &gf;
}

void GpsWidget::setPainter(QPainter * p){
    BaseWidget::setPainter(p);
    for(auto p2 : m_widgets){
        p2->setPainter(p);
    }
    m_infosWidget.setPainter(p);
}

int max = 10000;
void GpsWidget::my_projete(double x, double y, double & x_res, double & y_res){
    double x1_temp = (x - m_xref)*m_zoom;
    double y1_temp = (y - m_yref)*m_zoom;
    
    double h00 = m_cosA, h01 = m_sinA;//, h02 = 1;
    double h10 = -m_sinA, h11 = m_cosA;// h12 = 1;
    //double h20 = -m_sinA, h21 = m_cosA, h22 = 1;
    
    double x_temp = (x1_temp*h00 + y1_temp*h01);
    double y_temp  = (x1_temp*h10  + y1_temp*h11);
    
    if(m_vue_3D){
        double a = 1;
        double b = 1.005;
        double res = ((a-1)*x_temp+(b-1)*y_temp+1);
        if(res<0){
            res = 0.0001;
        }
        x_res = a*x_temp/res;
        y_res  = b*y_temp/res;
    } else {
        x_res = x_temp;
        y_res = y_temp;
    }
}

void GpsWidget::my_projete2(double x, double y, double & x_res, double & y_res){
    double x_temp, y_temp;
    my_projete(x,y,x_temp, y_temp);
    
    x_res = m_width/2 + x_temp;
    y_res = m_height*9/16 - y_temp;
}

void GpsWidget::my_projete2_pt(GpsPoint_ptr pt, double & x_res, double & y_res){
    my_projete2(pt->m_x, pt->m_y, x_res, y_res);
}

bool GpsWidget::must_be_draw(double x, double y){
    return true;
    double x_temp = (x - m_xref);
    double y_temp = (y - m_yref);
    double res = x_temp*x_temp+y_temp*y_temp;
    
    double l = m_width/m_zoom*0.9;
    return res<l*l;
}


void GpsWidget::drawCurve(Curve_ptr l, QPen & pen){
    if(!l){
        return;
    }
    m_painter->setPen(pen);
    m_painter->setBrush(m_brushNo);
    
    bool first = true;
    GpsPoint_ptr old_point = nullptr;
    QPainterPath polygonPath;
    for(auto p :l->m_points_to_draw){
        double xB, yB;
        my_projete2(p->m_x, p->m_y, xB, yB);
        if(m_debug){
            m_painter->drawRect(xB-1, yB-1, 2, 2);
        }
        if(!first){
            polygonPath.lineTo(xB, yB);
        } else {
            polygonPath.moveTo(xB, yB);
        }
        first = false;
    }
    m_painter->drawPath(polygonPath);
}

void GpsWidget::drawLine2(Line_ptr l, QPen & pen){
    if(!l){
        return;
    }
    m_painter->setPen(pen);
    m_painter->setBrush(m_brushNo);
    
    GpsPoint_ptr old_point = nullptr;
    
    GpsFramework & f = GpsFramework::Instance();
    
    l->calculProjete2(m_xref, m_yref);
    double xH, yH;
    my_projete2(l->m_proj_x-f.m_lineAB.m_x_ab*0, l->m_proj_y-f.m_lineAB.m_y_ab*0, xH, yH);
    
    
    QPainterPath polygonPath;
    polygonPath.moveTo(xH, yH);
    
    for(int i = 1; i < 10; ++i){
        double xB, yB;
        my_projete2(l->m_proj_x+i*f.m_lineAB.m_x_ab*1, l->m_proj_y+i*f.m_lineAB.m_y_ab*1, xB, yB);
        polygonPath.lineTo(xB, yB);
    }
    for(int i = 1; i < 10; ++i){
        double xB, yB;
        my_projete2(l->m_proj_x+i*f.m_lineAB.m_x_ab*11, l->m_proj_y+i*f.m_lineAB.m_y_ab*11, xB, yB);
        polygonPath.lineTo(xB, yB);
    }
    m_painter->drawPath(polygonPath);
   
    QPainterPath polygonPath2;
    polygonPath2.moveTo(xH, yH);
    for(int i = 1; i < 10; ++i){
        double xB, yB;
        my_projete2(l->m_proj_x-i*f.m_lineAB.m_x_ab*1, l->m_proj_y-i*f.m_lineAB.m_y_ab*1, xB, yB);
        polygonPath2.lineTo(xB, yB);
    }
    for(int i = 1; i < 10; ++i){
        double xB, yB;
        my_projete2(l->m_proj_x-i*f.m_lineAB.m_x_ab*11, l->m_proj_y-i*f.m_lineAB.m_y_ab*11, xB, yB);
        polygonPath2.lineTo(xB, yB);
    }
    m_painter->drawPath(polygonPath2);
}


void GpsWidget::drawLineCurve(){
    GpsFramework & f = GpsFramework::Instance();
    if(f.m_line){
        if(f.m_etat == Etat_OK){
            auto list = f.m_lineAB.getCurrentLine();
            drawLine2(list, m_penBlack);
            for(int i = 1; i<2; ++i){
                auto list2 = f.m_lineAB.getCurrentLineRel(i);
                drawLine2(list2, m_penGray);
                auto list3 = f.m_lineAB.getCurrentLineRel(-i);
                drawLine2(list3, m_penGray);
            }
        }
    } else {
        if(f.m_etat == Etat_OK){
            auto list = f.m_curveAB.getCurrentLine();
            drawCurve(list, m_penBlack);
            for(int i = 1; i<2; ++i){
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
        for(auto s: f.m_parcelle.m_contour_to_draw){
            double x, y;
            my_projete2(s->m_x, s->m_y, x, y);
            if(m_debug){
                //scene->addEllipse(x, y, 1, 1, m_penBlack);
            }
            p.push_back(QPoint(x, y));
        }
        if(force){
            m_painter->setPen(m_penBlack);
            m_painter->setBrush(m_brushParcelle);
            m_painter->drawPolygon(p);
            
            m_painter->setBrush(m_brushRed);
            for(auto i: f.m_parcelle.m_flag){
                double x, y;
                auto p = f.m_parcelle.m_contour[i];
                my_projete2(p->m_x, p->m_y, x, y);
                m_painter->drawEllipse(x-3, y-3, 6, 6);
            }
        } else {
            m_painter->setPen(m_penNo);
            m_painter->setBrush(m_brushParcelle);
            m_painter->drawPolygon(p);
            
        }
        
    }
}

void GpsWidget::drawSurfaceToDraw(){
    GpsFramework & f = GpsFramework::Instance();
    
    m_painter->setBrush(m_brushGreenAlpha);
    m_painter->setPen(m_penNo);
    for(auto s: f.m_listSurfaceToDraw){
        if(s->m_points.size() > 0 && s->m_lastPoint && s->m_lastPoint->m_point_left && s->m_lastPoint->m_point_right){
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
                        QPointF points[4] = {
                            QPointF(xA, yA),
                            QPointF(xB, yB),
                            QPointF(xB1, yB1),
                            QPointF(xA1, yA1)};
                        m_painter->drawPolygon(points, 4);
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

void GpsWidget::draw(){
    //scene = s;
    DEBUG("BEGIN");
    draw_force();
    DEBUG("END");
}

void GpsWidget::drawBackground(){
    double begin_ref_x = round(m_xref/20.0)*20;
    double begin_ref_y = round(m_yref/20.0)*20;
    
    m_painter->setPen(m_penNo);
    
    int nb = -1;
    if(m_zoom > 4){
        nb = 10;
        if(m_zoom > 10){
            nb = 5;
            if(m_zoom > 20){
                nb = 4;
            }
        }
    }
    nb=10;
    for(int j = -nb; j < nb+1; ++j){
        for(int i = -nb; i < nb+1; i+=1){
            if((i+j)%2==0){
                m_painter->setBrush(m_brushBackGround1);
                
            } else {
                m_painter->setBrush(m_brushBackGround2);
                
            }
            double x0 = begin_ref_x+i*10;
            double y0 = begin_ref_y+j*10;
            double x02, y02;
            my_projete2(x0, y0, x02, y02);
            double x1 = begin_ref_x+(i+1)*(10);
            double y1 = begin_ref_y+j*10;
            double x12, y12;
            my_projete2(x1, y1, x12, y12);
            double x3 = begin_ref_x+(i+1)*(10);
            double y3 = begin_ref_y+(j+1)*(10);
            double x32, y32;
            my_projete2(x3, y3, x32, y32);
            double x4 = begin_ref_x+i*(10);
            double y4 = begin_ref_y+(j+1)*(10);
            double x42, y42;
            my_projete2(x4, y4, x42, y42);
            
            QPointF points[4] = {
                QPointF(x02, y02),
                QPointF(x12, y12),
                QPointF(x32, y32),
                QPointF(x42, y42)};
            m_painter->drawPolygon(points, 4);
        }
    }
}

void GpsWidget::draw_force(){
    DEBUG("BEGIN");
    auto begin = std::chrono::system_clock::now();
    GpsFramework & f = GpsFramework::Instance();
    m_debug = f.m_config.m_debug;
    m_vue_3D = f.m_config.m_3d;
    
    m_widthMax = m_width/2+f.m_config.m_outil_largeur*m_zoom/2;
    m_heightMax = m_height/2+f.m_config.m_outil_largeur*m_zoom/2;
    m_debug = f.m_config.m_debug;
    if(f.m_tracteur.m_pt_antenne_corrige){
        m_xref = f.m_tracteur.m_pt_antenne_corrige->m_x;
        m_yref = f.m_tracteur.m_pt_antenne_corrige->m_y;
    }
    
    
    
    
    m_a = -3.14/2+f.m_deplacementAngle;
    if(!f.m_config.m_sensDraw){
        m_a = 0;
    }
    m_cosA = cos(m_a);
    m_sinA = sin(m_a);
    
    
    drawBackground();
    
    //scene->clear();
    
    
    if(f.m_etat == Etat_ParcelleAdd || f.m_etat == Etat_ParcellePause){
        drawParcelle(true);
        drawTracteur();
    } else {
        drawParcelle();
        drawLineCurve();
        
        
        drawSurfaceToDraw();
        
        drawTracteur();
        drawDebugEkf();
        
        if(m_vue_3D){
            m_painter->setBrush(QColor(135,206,235));
            m_painter->setPen(m_penNo);
            QPolygonF points;
            points.append(QPointF(0, 0));
            points.append(QPointF(m_width, 0));
            points.append(QPointF(m_width, m_height*0.25));
            points.append(QPointF(m_width/2, m_height*0.20));
            points.append(QPointF(0, m_height*0.25));
            
            m_painter->drawPolygon(points);
            m_painter->drawRect(0,0, m_width, m_height/6);
        }
        
        drawTop();
        drawBottom();
        
        addButtons();
    }
    
    if(f.m_config.m_debug){
        drawDebug();
    }
    
    if(m_debug && m_zoom >40){
        drawVolant(m_height/2, m_width/2);
    }
    
    
    for(auto p : m_widgets){
        if(!p->m_close){
            p->draw();
        }
    }
    if(!m_infosWidget.m_close){
        m_infosWidget.draw();
    }
    drawError();
    
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> diff2 = end - begin;
    f.m_draw_time.addNewValue(diff2.count()*1000);
    
    
    
    
    DEBUG("END");
    
}

double cosa = 0;
double sina = 0;
void GpsWidget::drawRect3D(double x, double y, double l, double lg){
    
    double x1 = x - sina*lg/2;
    double y1 = y + cosa*lg/2;
    my_projete2(x1, y1, x1, y1);
    double x2 = x + sina*lg/2;;
    double y2 = y - cosa*lg/2;;
    my_projete2(x2, y2, x2, y2);
    double x_outil2 = x + cosa*l;
    double y_outil2 = y + sina*l;
    double x3 = x_outil2 - sina*lg/2;
    double y3 = y_outil2 + cosa*lg/2;
    my_projete2(x3, y3, x3, y3);
    double x4 = x_outil2 + sina*lg/2;;
    double y4 = y_outil2 - cosa*lg/2;;
    my_projete2(x4, y4, x4, y4);
    QPolygonF points;
    points.append(QPointF(x3, y3));
    points.append(QPointF(x4, y4));
    points.append(QPointF(x2, y2));
    points.append(QPointF(x1, y1));
    
    m_painter->drawPolygon(points);
}


void GpsWidget::drawTracteur3D(){
    GpsFramework & f = GpsFramework::Instance();
    
    double x_tracteur = m_width/2, y_tracteur = m_height/2;
    double x_tracteur2 = m_width/2, y_tracteur2 = m_height/2;
    double angle = 0;
    if(f.m_tracteur.m_pt_antenne_corrige){
        my_projete2(f.m_tracteur.m_pt_antenne_corrige->m_x, f.m_tracteur.m_pt_antenne_corrige->m_y, x_tracteur2, y_tracteur2);
        x_tracteur = f.m_tracteur.m_pt_antenne_corrige->m_x;
        y_tracteur = f.m_tracteur.m_pt_antenne_corrige->m_y;
        angle = f.m_deplacementAngle;
    }
    cosa = cos(angle);
    sina = sin(angle);
    
    
    //double l2 = f.m_tracteur.m_antenne_essieu_arriere*m_zoom;
    
    
    //outil
    m_painter->setBrush(m_brushOutil);
    double l_outil2 = f.m_tracteur.m_outil_distance+f.m_tracteur.m_antenne_essieu_arriere;
    double lg_outil = f.m_tracteur.m_outil_largeur;
    double x_outil = x_tracteur - cosa*l_outil2;
    double y_outil = y_tracteur - sina*l_outil2;
    
    drawRect3D(x_outil, y_outil, 0.5, lg_outil);
    drawRect3D(x_outil, y_outil, f.m_tracteur.m_outil_distance, 0.3);
    
    if(!m_debug){
        int h = 8*m_zoom;
        int w = 8*m_zoom;
        m_painter->drawPixmap(x_tracteur2-w/2, y_tracteur2-h/2, w, h, *m_tracteur_3d);
        return;
    }
    
    //roue arriere
    double x_arriere = x_tracteur - cosa*f.m_tracteur.m_antenne_essieu_arriere;
    double y_arriere = y_tracteur - sina*f.m_tracteur.m_antenne_essieu_arriere;
    
    m_painter->setBrush(m_brushTracteur);
    double voie = 1.8;
    double l_roue = 1.7;
    double lg_pneu = 0.45;
    
    drawRect3D(x_arriere - cosa*0.1, y_arriere - sina*0.1, 0.2, voie);
    
    double x_arriere_l = x_arriere - cosa*l_roue/2 - sina*voie/2;
    double y_arriere_l = y_arriere - sina*l_roue/2 + cosa*voie/2;
    double x_arriere_r = x_arriere - cosa*l_roue/2 + sina*voie/2;
    double y_arriere_r = y_arriere - sina*l_roue/2 - cosa*voie/2;
    
    drawRect3D(x_arriere_l, y_arriere_l, l_roue, lg_pneu);
    drawRect3D(x_arriere_r, y_arriere_r, l_roue, lg_pneu);
    
    //roue avant
    l_roue = 1.1;
    lg_pneu = 0.33;
    
    double x_avant = x_tracteur + cosa*f.m_tracteur.m_antenne_essieu_avant;
    double y_avant = y_tracteur + sina*f.m_tracteur.m_antenne_essieu_avant;
    
    m_painter->setBrush(m_brushTracteur);
    drawRect3D(x_avant - cosa*0.1, y_avant - sina*0.1, 0.2, voie);
    
    double x_avant_l = x_avant - cosa*l_roue/2 - sina*voie/2;
    double y_avant_l = y_avant - sina*l_roue/2 + cosa*voie/2;
    double x_avant_r = x_avant - cosa*l_roue/2 + sina*voie/2;
    double y_avant_r = y_avant - sina*l_roue/2 - cosa*voie/2;
    
    drawRect3D(x_avant_l, y_avant_l, l_roue, lg_pneu);
    drawRect3D(x_avant_r, y_avant_r, l_roue, lg_pneu);
    
    //capot
    double lg_capot = 1;
    double l_capot = f.m_tracteur.m_empatement+0.3;
    drawRect3D(x_arriere, y_arriere, l_capot, lg_capot);
    
    //cabine
    double l_cabine = 1.6;
    double x_cabine = x_arriere - cosa*0.3;
    double y_cabine  = y_arriere - sina*0.3;
    drawRect3D(x_cabine, y_cabine, l_cabine, l_cabine);
    
    
}

void GpsWidget::drawTracteur(){
    if(m_vue_3D){
        drawTracteur3D();
        return;
    }
    
    int h = 6*m_zoom;
    int w = 6*m_zoom;
    //drawTracteur3D();
    //return;
    GpsFramework & f = GpsFramework::Instance();
    
    double x_tracteur = m_width/2, y_tracteur = m_height/2;
    if(f.m_tracteur.m_pt_antenne_corrige){
        my_projete2(f.m_tracteur.m_pt_antenne_corrige->m_x, f.m_tracteur.m_pt_antenne_corrige->m_y, x_tracteur, y_tracteur);
    }
    
    
    double l2 = f.m_tracteur.m_antenne_essieu_arriere*m_zoom;
    
    double y_arriere = y_tracteur+l2;
    
    
    //outil
    m_painter->setBrush(m_brushOutil);
    double l_outil = f.m_tracteur.m_outil_distance;
    double lg_outil = f.m_tracteur.m_outil_largeur;
    m_painter->drawRect(x_tracteur - lg_outil*0.5*m_zoom, y_arriere + (l_outil-0.5)*m_zoom, lg_outil*m_zoom, 0.5*m_zoom);
    m_painter->drawRect(x_tracteur - 0.1*m_zoom, y_arriere, 0.2*m_zoom, (l_outil)*m_zoom);
    
    if(!m_debug){
        m_painter->drawPixmap(x_tracteur-w/2, y_tracteur-h*0.55, w, h, *m_tracteur_2d);
        //m_painter->drawEllipse( x_tracteur-5, y_tracteur-5, 10, 10);
        return;
    }
    
    //tracteur
    m_painter->setBrush(m_brushTracteur);
    m_painter->setPen(m_penNo);
    
    //roue arriere
    double voie = 1.8*m_zoom;
    double l_roue = 1.7*m_zoom;
    double lg_pneu = 0.45*m_zoom;
    m_painter->drawRect(x_tracteur - voie/2, y_arriere-0.1*m_zoom, voie, 0.2*m_zoom);
    m_painter->drawRect(x_tracteur - voie/2 - lg_pneu/2, y_arriere - l_roue/2, lg_pneu, l_roue);
    m_painter->drawRect(x_tracteur + voie/2 - lg_pneu/2, y_arriere - l_roue/2, lg_pneu, l_roue);
    
    //cabine
    double l_cabine = 1.6*m_zoom;
    m_painter->drawRect(x_tracteur - l_cabine/2, y_arriere-1.2*m_zoom, l_cabine, l_cabine);
    
    //capot
    double lg_capot = 1*m_zoom;
    double l_capot = (f.m_tracteur.m_antenne_essieu_avant+0.2)*m_zoom;
    m_painter->drawRect(x_tracteur - lg_capot/2, y_tracteur-l_capot, lg_capot, f.m_tracteur.m_empatement*m_zoom);
    
    
    double y_direction = y_tracteur-f.m_tracteur.m_antenne_essieu_avant*m_zoom;
    m_painter->drawRect(x_tracteur - voie/2, y_direction-0.1*m_zoom, voie, 0.2*m_zoom);
    
    
    
    double l_roue2 = 1.1*m_zoom/2;
    double lg_pneu2 = 0.33*m_zoom/2;
    
    
    
    for(int i = -1; i<=1; i+=2){
        float x = x_tracteur+i*voie/2;
        
        float dx = -l_roue2*sin(f.m_angle_correction);
        float dy = l_roue2*cos(f.m_angle_correction);
        
        float droue_x = lg_pneu2;
        float droue_y = 0;
        if(abs(f.m_angle_correction) < 3.14){
            droue_x = lg_pneu2*cos(f.m_angle_correction);
            droue_y = lg_pneu2*sin(f.m_angle_correction);
        }
        
        m_painter->drawLine(x-dx, y_direction-dy, x+dx, y_direction+dy);
        
        QPointF points[4] = {
            QPointF(x-dx-droue_x, y_direction-dy-droue_y),
            QPointF(x-dx+droue_x, y_direction-dy+droue_y),
            QPointF(x+dx+droue_x, y_direction+dy+droue_y),
            QPointF(x+dx-droue_x, y_direction+dy-droue_y)
            
        };
        m_painter->drawPolygon(points, 4);
    }
    
    
    m_painter->setBrush(m_brushWhite);
    m_painter->setPen(m_penBlack);
    m_painter->drawEllipse(x_tracteur - 0.10*m_zoom, y_tracteur - 0.10*m_zoom, 0.20*m_zoom, 0.20*m_zoom);
    
    
    if(m_debug){
        //scene->addEllipse(w/2 - 0.05*m_zoom, h/2 - 0.05*m_zoom, 0.10*m_zoom, 0.10*m_zoom, m_penBlack, m_brushRed);
        /*my_projete2_pt(f.m_tracteur.m_pt_outil_arriere, x, y);
         scene->addEllipse(x-3, y-3, 6, 6, m_penRed, m_brushGreen);
         my_projete2_pt(f.m_tracteur.m_pt_outil_arriere_droite, x, y);
         scene->addEllipse(x-2, y-2, 4, 4, m_penRed, m_brushGreen);*/
    }
    /*
     
     double x;
     double y;
     
     double x2;
     double y2;
     
     //m_angle_correction
     
     my_projete(m_xref+dx, m_yref+dy, x, y);
     my_projete(m_xref+dx/4, m_yref+dy/4, x2, y2);
     
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
     QPolygon polygon;
     polygon << QPoint(w/2 + xA, h/2 - yA) << QPoint(w/2 + x2, h/2 - y2) << QPoint(w/2 - xA, h/2 + yA)<< QPoint(w/2 + x, h/2 - y);
     scene->addPolygon(polygon, m_penNo, m_brushTractor);
     
     scene->addLine(w/2+xA, h/2-yA, w/2 - xA, h/2 + yA, m_penBlue);
     
     if(f.m_config.m_debug){
     my_projete(m_xref, m_yref, x, y);
     double a = -f.m_angle_correction;
     my_projete(m_xref+dx*cos(a)-dy*sin(a), m_yref+dx*sin(a)+dy*cos(a), x2, y2);
     scene->addLine(w/2+x, h/2-y, w/2 + x2, h/2 - y2, m_penBlue);
     }*/// TODO
    
}


void GpsWidget::drawDebugEkf(){
    GpsFramework & f = GpsFramework::Instance();
    if(f.m_config.m_debug){
        m_painter->setPen( QPen(Qt::black, 2) ); // personnaliser
        
        for(auto p: f.m_ekf_module.m_list){
            double x1, y1;
            
            my_projete2(p->m_x, p->m_y, x1, y1);
            m_painter->drawPoint(x1, y1);
        }
        
        m_painter->setPen(m_penRed); // personnaliser
        double x_last = 0, y_last = 0;
        for(auto p: f.m_ekf_module.m_list_tracteur){
            double x1, y1;
            my_projete2(p->m_x, p->m_y, x1, y1);
            if(x_last != 0){
                m_painter->drawLine(x1, y1, x_last, y_last);
            }
            x_last = x1;
            y_last = y1;
            
        }
        x_last = 0;
        m_painter->setPen(m_penGreen); // personnaliser
        for(auto p: f.m_ekf_module.m_list_ekf){
            double x1, y1;
            my_projete2(p->m_x, p->m_y, x1, y1);
            if(x_last != 0){
                m_painter->drawLine(x1, y1, x_last, y_last);
            }
            x_last = x1;
            y_last = y1;
            
        }
    }
}

const int l1 = 15;
void GpsWidget::drawTop(){
    GpsFramework & f = GpsFramework::Instance();
    
    m_painter->setPen(m_penBlack);
    m_painter->setBrush(m_brushDarkGray);
    
    m_painter->drawRect(0, 0, m_width, 40);
    
    m_painter->setBrush(m_brushGray);
    if(f.m_ledAB == 888 && abs(f.m_distanceAB) < 0.025){
        m_painter->setBrush(m_brushGreen);
    }
    m_painter->drawRect(m_width/2-50, 5, 100, 30);
    QString s = QString::number(f.m_distanceAB, 'f', 2) + " m";
    
    drawQText(s, m_width/2, 23,sizeText_big, true);
    
    /*scene->addRect(m_width/2-60, 40, 120, 40, m_penBlack, m_brushDarkGray);
     scene->addRect(m_width/2-50, 45, 100, 30, m_penBlack, m_grayBrush);
     {
     QString s = QString::number(f.m_angle_correction/3.14*180, 'f', 2) + " °";
     drawQText(s, m_width/2, 63,sizeText_big, true);
     }*/
    
    if(f.m_ledAB != 888){
        m_painter->setBrush(m_brushGray);
        for(int i = 0; i < 8; ++i){
            m_painter->drawRect(m_width/2 - 60 - 10 - l1*i, 10, 10, 20);
            m_painter->drawRect(m_width/2 + 60 + l1*i, 10, 10, 20);
        }
        m_painter->setBrush(m_brushGreen);
        if(f.m_ledAB > 0){
            for(int i = 0; i < std::min(8, f.m_ledAB); ++i){
                m_painter->drawRect(m_width/2 - 60 - 10 - l1*i, 10, 10, 20);
            }
        } else {
            for(int i = 0; i < std::min(8, -f.m_ledAB); ++i){
                m_painter->drawRect(m_width/2 + 60 + l1*i, 10, 10, 20);
            }
        }
    } else {
        if(f.m_distanceAB < 0){
            if(f.m_distanceAB > -0.025){
                m_painter->setBrush(m_brushNo);
            } else if(f.m_distanceAB > -0.05){
                m_painter->setBrush(m_brushGreen);
            } else if(f.m_distanceAB > -0.15){
                m_painter->setBrush(m_brushOrange);
            } else {
                m_painter->setBrush(m_brushRed);
            }
            QPointF points[4] = {
                QPointF(m_width/2 - 80, 10),
                QPointF(m_width/2 - 100, 20),
                QPointF(m_width/2 - 80, 30),};
            m_painter->drawPolygon(points, 3);
            
            
        } else {
            if(f.m_distanceAB < 0.025){
                m_painter->setBrush(m_brushNo);
            } else if(f.m_distanceAB < 0.05){
                m_painter->setBrush(m_brushGreen);
            } else if(f.m_distanceAB < 0.15){
                m_painter->setBrush(m_brushOrange);
            } else {
                m_painter->setBrush(m_brushRed);
            }
            QPointF points2[4] = {
                QPointF(m_width/2 + 80, 10),
                QPointF(m_width/2 + 100, 20),
                QPointF(m_width/2 + 80, 30),};
            m_painter->drawPolygon(points2, 3);
        }
    }
    
    {
        QString s = QString::number(f.m_vitesse, 'f', 1) + " km/h";
        drawQText(s, 7*m_width/8-40, 20,sizeText_medium, true, true);
    }
    
    {
        int current_line = 0;
        if(f.m_line){
            current_line = f.m_lineAB.m_i_current;
        } else {
            current_line = f.m_curveAB.m_i_current;
        }
        QString s = QString::number(f.m_surface, 'f', 2) + " ha    ( "+QString::number(current_line)+" )";
        drawQText(s, 1*m_width/8, 20,sizeText_medium, true, true);
    }
    
    auto last_frame = f.m_lastGGAFrame;
    QPixmap * img = m_imgSatBlanc;
    if(f.isGpsConnected()){
        if(last_frame.m_fix == 4){
            f.m_rtk = true;
        }
        if(f.m_rtk){
            if(last_frame.m_fix == 1 || last_frame.m_fix == 2 || last_frame.m_fix == 5 || last_frame.m_fix == 9){
                img = m_imgSatOrange;
            } else if(last_frame.m_fix == 4){
                img = m_imgSatVert;
            } else {
                img = m_imgSatRouge;
            }
        } else {
            if(last_frame.m_fix == 1 || last_frame.m_fix == 5){
                img = m_imgSatOrange;
            } else if(last_frame.m_fix == 2 || last_frame.m_fix == 4 || last_frame.m_fix == 9){
                img = m_imgSatVert;
            } else {
                img = m_imgSatRouge;
            }
        }
    }
    
    drawButtonImage(m_buttonSat, img);
}


void GpsWidget::drawBottom(){
    //GpsFramework & f = GpsFramework::Instance();
    
    //auto last_frame = f.m_lastGGAFrame;
    m_painter->setBrush(m_brushDarkGray);
    m_painter->setPen(m_penBlack);
    m_painter->drawRect(0, m_height-40-l_bottom, m_width, 40+l_bottom);
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

/*void GpsWidget::drawVolant_(double y, double a, double r, double start_angle){
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
}*/

void GpsWidget::drawVolant(double y, double x){
    GpsFramework & f = GpsFramework::Instance();
    
    y = 3*m_height/4;
    x = 3*m_width/4;
    int r=40;
    
    double j = 1.2;
    m_painter->setBrush(m_brushWhite);
    m_painter->drawEllipse(x-r*j, y-r*j, 2*r*j, 2*r*j);
    
    
    
    /*QGraphicsEllipseItem* item = new QGraphicsEllipseItem(m_width/2-r, y-r, 2*r, 2*r);
     item->setBrush(m_grayBrush);
     item->setPen(m_penNo);
     item->setStartAngle(90*16-f.m_pilotModule.m_volant0*360*16);
     item->setSpanAngle(-angle*360*16);
     scene->addItem(item);*/
    
    j=0.8;
    m_painter->drawEllipse(x-r*j, y-r*j, 2*r*j, 2*r*j);
    {
        QString s = QString::number(f.m_pilotModule.m_volantTotal, 'f', 2);
        drawQText(s, x, y-r-10, sizeText_little, true, false);
    }
    {
        QString s = QString::number(f.m_pilotModule.m_volantMesured, 'f', 2);
        drawQText(s, x, y-r+20, sizeText_little, true, false);
    }
    
    {
        //drawVolant_(y, f.m_pilotModule.m_volantMesured, r-10, 0);
        
    }
    
    //double x1 = cos(i*2*3.14)*r;
    //double y1 = sin(i*2*3.14)*r;
    //scene->addRect(m_width/2+x1, y+y1, 2, 2);*/
    
}

void GpsWidget::drawDebug(){
    GpsFramework & f = GpsFramework::Instance();
    
    if(f.m_pilot_algo == FollowCarrot){
        if(f.m_line){
            m_painter->setPen(QColor(255,157,0));
            double fc_x;
            double fc_y;
            my_projete2(f.m_lineAB.m_fc_x, f.m_lineAB.m_fc_y, fc_x, fc_y);
            
            double fc_lh_x;
            double fc_lh_y;
            my_projete2(f.m_lineAB.m_fc_lh_x, f.m_lineAB.m_fc_lh_y, fc_lh_x, fc_lh_y);
            m_painter->drawLine(fc_x, fc_y, fc_lh_x, fc_lh_y);
            
            double fc_d_x;
            double fc_d_y;
            my_projete2(f.m_lineAB.m_fc_x + f.m_deplacementX, f.m_lineAB.m_fc_y + f.m_deplacementY, fc_d_x, fc_d_y);
            m_painter->drawLine(fc_x, fc_y, fc_d_x, fc_d_y);
            
            double fc_h_x;
            double fc_h_y;
            my_projete2(f.m_lineAB.m_fc_xh, f.m_lineAB.m_fc_yh, fc_h_x, fc_h_y);
            m_painter->drawEllipse(fc_h_x-1, fc_h_y-1, 2, 2);
            
            
            double a_x;
            double a_y;
            my_projete2(f.m_lineAB.m_pointA.m_x, f.m_lineAB.m_pointA.m_y, a_x, a_y);
            m_painter->drawEllipse(a_x-1, a_y-1, 2, 2);
            m_painter->drawText(a_x-1, a_y-1, "A");
            
            
            double b_x;
            double b_y;
            my_projete2(f.m_lineAB.m_pointB.m_x, f.m_lineAB.m_pointB.m_y, b_x, b_y);
            m_painter->drawEllipse(b_x-1, b_y-1, 2, 2);
            m_painter->drawText(b_x-1, b_y-1, "B");
            
            
        }
        
        if(!f.m_line){
            m_painter->setPen(QColor(255,157,0));
            double fc_x;
            double fc_y;
            my_projete2(f.m_curveAB.m_fc_x, f.m_curveAB.m_fc_y, fc_x, fc_y);
            
            double fc_lh_x;
            double fc_lh_y;
            my_projete2(f.m_curveAB.m_fc_lh_x, f.m_curveAB.m_fc_lh_y, fc_lh_x, fc_lh_y);
            m_painter->drawLine(fc_x, fc_y, fc_lh_x, fc_lh_y);
            
            double fc_d_x;
            double fc_d_y;
            my_projete2(f.m_curveAB.m_fc_x + f.m_deplacementX, f.m_curveAB.m_fc_y + f.m_deplacementY, fc_d_x, fc_d_y);
            m_painter->drawLine(fc_x, fc_y, fc_d_x, fc_d_y);
            
            double fc_h_x;
            double fc_h_y;
            my_projete2(f.m_curveAB.m_fc_xh, f.m_curveAB.m_fc_yh, fc_h_x, fc_h_y);
            m_painter->drawEllipse(fc_h_x-2, fc_h_y-2, 4, 4);
            
            
        }
    }
    
    if(f.m_pilot_algo == RearWheelPosition){
        if(!f.m_line){
            if(f.m_curveAB.m_curb_l != 0){
                m_painter->setPen(QColor(255,157,0));
                m_painter->setBrush(m_brushNo);
                double l = f.m_curveAB.m_curb_l*2*m_zoom;
                double curb_x;
                double curb_y;
                my_projete2(f.m_curveAB.m_curb_x, f.m_curveAB.m_curb_y, curb_x, curb_y);
                m_painter->drawEllipse(curb_x-l/2, curb_y-l/2, l, l);
                
                double curb_cx;
                double curb_cy;
                my_projete2(f.m_curveAB.m_curb_c_x, f.m_curveAB.m_curb_c_y, curb_cx, curb_cy);
                m_painter->drawEllipse(curb_cx-1, curb_cy-1, 2, 2);
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
        m_painter->setPen(m_penBlack);
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
            
            //scene->addRect(x, y, l2, m_height-100, m_penBlack, m_brushLightGrayDebug);
            
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
            m_painter->setPen(m_penBlack);
            m_painter->setBrush(m_brushLightGrayDebug);
            m_painter->drawRect(x, y, w, h);
            drawText("Erreurs", x+w/2, y+15, sizeText_big, true);
            drawText(f.m_messages_errors, x+20, y+40, sizeText_little, false);
            m_buttonErrorOk.m_x = (x+w/2);
            m_buttonErrorOk.m_y = (y+h-30);
            drawButtonImage(m_buttonErrorOk, m_imgOk);
            // m_pilot_serial_input
        }
    }
}

void GpsWidget::addButtons(){
    drawButtonImage(m_buttonPlus, m_imgPlus);
    drawButtonImage(m_buttonMinus, m_imgMinus);
    drawButtonImage(m_buttonGuidage, m_imgGuidage);
    drawButtonImage(m_buttonParcelle, m_imgParcelle);
    drawButtonImage(m_buttonOption, m_imgOption);
    
    GpsFramework & f = GpsFramework::Instance();
    if(f.m_pauseDraw){
        drawButtonImage(m_buttonChamp, m_imgChampBlanc);
    } else {
        drawButtonImage(m_buttonChamp, m_imgChampVert);
    }
    
    if(f.isPilotConnected()){
        if(f.getVolantEngaged()){
            drawButtonImage(m_buttonVolant, m_imgVolantVert);
        } else {
            drawButtonImage(m_buttonVolant, m_imgVolantBlanc);
        }
        
        
    } else {
        drawButtonImage(m_buttonVolant, m_imgVolantRouge);
    }
    if(f.m_config.m_pilot_auto_deactive > 0){
        if(f.m_pilot_auto){
            drawButtonImage(m_buttonVolantAuto, m_imgVolantAutoVert);
        } else {
            drawButtonImage(m_buttonVolantAuto, m_imgVolantAutoGris);
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
    if(!m_infosWidget.m_close){
        m_infosWidget.onMouse(x, y);
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
        m_infosWidget.m_close = false;
    } else if(m_buttonChamp.isActive(x2, y2)){
        GpsFramework::Instance().changeDraw();
    } else if(m_buttonVolant.isActive(x2, y2)){
        GpsFramework::Instance().setVolantEngaged(!GpsFramework::Instance().getVolantEngaged());
    } else if(f.m_config.m_pilot_auto_deactive > 0 && m_buttonVolantAuto.isActive(x2, y2)){
        GpsFramework::Instance().m_pilot_auto = !GpsFramework::Instance().m_pilot_auto;
    } else if(m_button3d.isActive(x2, y2)){
        GpsFramework::Instance().m_config.m_3d = !GpsFramework::Instance().m_config.m_3d;
    }
    //draw();
}

