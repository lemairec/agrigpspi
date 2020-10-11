#include "option_widget.hpp"

#include "../gps_framework.hpp"
#include "gps_widget.hpp"
#include "environnement.hpp"
#include "qt/main_window.hpp"

#define PETIT_RAYON2 0.025

OptionWidget::OptionWidget(){
    
    m_imgClose = loadImage("/images/ok.png");
    m_imgPlus = loadImage("/images/plus.png");
    m_imgMinus = loadImage("/images/minus.png");
    m_imgSatBlanc = loadImage("/images/sat_blanc.png");
    m_imgSatGris = loadImage("/images/sat_gris.png");
    m_imgVolantBlanc = loadImage("/images/volant_blanc.png");
    m_imgVolantGris = loadImage("/images/volant_gris.png");
    m_imgOutilBlanc = loadImage("/images/outil_blanc.png");
    m_imgOutilGris = loadImage("/images/outil_gris.png");
    m_imgOptionBlanc = loadImage("/images/option_blanc.png");
    m_imgOptionGris = loadImage("/images/option_gris.png");
    
    m_button_close = new ButtonGui(0.75, 0.83, GROS_BUTTON, 0);
    
    m_button_p1  = new ButtonGui(0.24, 0.20, GROS_BUTTON, 0);
    m_button_p2  = new ButtonGui(0.24, 0.30, GROS_BUTTON, 0);
    m_button_p3  = new ButtonGui(0.24, 0.40, GROS_BUTTON, 0);
    m_button_p4  = new ButtonGui(0.24, 0.50, GROS_BUTTON, 0);
    m_button_p5  = new ButtonGui(0.24, 0.60, GROS_BUTTON, 0);
    m_button_p6  = new ButtonGui(0.24, 0.70, GROS_BUTTON, 0);
    
    
    //page 1
    double dist = 0.03;
    double rayon = PETIT_RAYON;
    m_button_p1add1 = new ButtonGui(0.6-dist, 0.3, rayon, 0);
    m_button_p1add2 = new ButtonGui(0.6+dist, 0.3, rayon, 0);
    
    m_button_p1minus1 = new ButtonGui(0.4+dist, 0.3, rayon, 0);
    m_button_p1minus2 = new ButtonGui(0.4-dist, 0.3, rayon, 0);
    
    
    
    
    
    
    
    setPage3();
    setPage4();
    setPage5();
    setPage6();
    
    //m_button_gps = new ButtonGui(0.25, 0.3, GROS_BUTTON, 0);
    //m_button_line = new ButtonGui(0.5, 0.3, GROS_BUTTON, 0);
    
    //m_close=false;
    //m_page =5;
    //addSerials();
}

void OptionWidget::draw(){
    
    
    
    scene->addRect(m_width*0.2, m_height*0.1, m_width*0.6, m_height*0.8, m_penBlack, m_brushWhiteAlpha);
    scene->addRect(m_width*0.2, m_height*0.1, m_width*0.08, m_height*0.8, m_penBlack, m_brushDarkGray);
    
    drawButtonImage(m_button_close, *m_imgClose);
    
    if(m_page == 1){
        drawButtonImage(m_button_p1, *m_imgOutilBlanc);
        drawPage1();
    } else {
        drawButtonImage(m_button_p1, *m_imgOutilGris);
    }
    
    if(m_page == 2){
        drawButtonImage(m_button_p2, *m_imgSatBlanc);
        drawPage2();
    } else {
        drawButtonImage(m_button_p2, *m_imgSatGris);
        
    }
    
    if(m_page == 3){
        drawButtonImage(m_button_p3, *m_imgVolantBlanc);
        drawPage3();
    } else {
        drawButtonImage(m_button_p3, *m_imgVolantGris);
        
    }
    
    if(m_page == 4){
        drawButtonImage(m_button_p4, *m_imgOptionBlanc);
        drawPage4();
    } else {
        drawButtonImage(m_button_p4, *m_imgOptionGris);
    }
    
    GpsFramework & f = GpsFramework::Instance();
    if(f.m_config.m_debug){
        if(m_page == 5){
            drawButtonImage(m_button_p5, *m_imgOptionBlanc);
            drawPage5();
        } else {
            drawButtonImage(m_button_p5, *m_imgOptionGris);
        }
        
        if(m_page == 6){
            drawButtonImage(m_button_p6, *m_imgOptionBlanc);
            drawPage6();
        } else {
            drawButtonImage(m_button_p6, *m_imgOptionGris);
        }
    }
}

void OptionWidget::drawPage1(){
    GpsFramework & f = GpsFramework::Instance();
    QString s = "largueur de l'outil " + QString::number(f.m_config.m_largeur, 'f', 2) + " m";
    drawText(s.toUtf8().constData(), 0.55*m_width, 0.15*m_height, 20, true);
    
    drawButtonImage(m_button_p1add1, *m_imgPlus);
    drawButtonImage(m_button_p1add2, *m_imgPlus);
    drawButtonImage(m_button_p1minus1, *m_imgMinus);
    drawButtonImage(m_button_p1minus2, *m_imgMinus);
}

/**
 PAGE 2
 */

void OptionWidget::drawPage2(){
    drawText("Connection GPS", 0.55*m_width, 0.15*m_height, 20, true);
    
    GpsFramework & f = GpsFramework::Instance();
    drawText("Port :", 0.3*m_width, y_port_title*m_height, 20, false);
    for(auto button: m_buttonp2Serials){
        if(button->m_label == f.m_config.m_input_gps){
            drawButton(button, COLOR_CHECK);
        } else {
            drawButton(button, COLOR_OTHER);
        }
        drawText(button->m_label, button->m_x+0.04, button->m_y);
    }
    
    for(auto button: m_buttonp2Baurates){
        if(button->m_labelInt == f.m_config.m_baudrate_gps){
            drawButton(button, COLOR_CHECK);
        } else {
            drawButton(button, COLOR_OTHER);
        }
        drawText(button->m_label, button->m_x+0.04, button->m_y);
    }
    
}

void OptionWidget::onMousePage2(double x, double y){
    
    for(auto button: m_buttonp2Serials){
        if(button->isActive(x,y)){
            GpsFramework & f = GpsFramework::Instance();
            f.m_config.m_input_gps = button->m_label;
            f.initOrLoadConfig();
        }
    }
    
    for(auto button: m_buttonp2Baurates){
        if(button->isActive(x,y)){
            GpsFramework & f = GpsFramework::Instance();
            f.m_config.m_baudrate_gps = button->m_labelInt;
            f.initOrLoadConfig();
        }
    }
}


/**
 PAGE 3
 */

void OptionWidget::setPage3(){
    m_select_pilot_serial = new SelectButtonGui(0.35,0.3, PETIT_RAYON2);
    
    m_select_pilot_baudrates = new SelectButtonGui(0.35,0.4, PETIT_RAYON2);
    m_select_pilot_baudrates->addValueInt("9600", 9600);
    m_select_pilot_baudrates->addValueInt("115200", 115200);
    
    m_select_pilot_langage = new SelectButtonGui(0.35,0.5, PETIT_RAYON2);
    m_select_pilot_langage->addValue("arduino");
    m_select_pilot_langage->addValue("hadrien");
    
    m_value_gui_lookahead_d = new ValueGui(0.35, 0.7, PETIT_RAYON2, 0, "lookahead ");
    
    
};

void OptionWidget::drawPage3(){
    GpsFramework & f = GpsFramework::Instance();
    drawText("Connection Pilot", 0.55*m_width, 0.15*m_height, 20, true);
    
    
    m_select_pilot_baudrates->setValueInt(f.m_config.m_baudratePilot);
    m_select_pilot_serial->setValueString(f.m_config.m_inputPilot);
    m_select_pilot_langage->m_selectedValue = f.m_config.m_pilot_langage;
    
    drawSelectButtonGuiClose(m_select_pilot_langage);
    drawSelectButtonGuiClose(m_select_pilot_baudrates);
    drawSelectButtonGuiClose(m_select_pilot_serial);
    
    drawValueGui(m_value_gui_lookahead_d, f.m_config.m_pilot_lookahead_d);
    
    drawSelectButtonGuiOpen(m_select_pilot_serial);
    drawSelectButtonGuiOpen(m_select_pilot_langage);
    drawSelectButtonGuiOpen(m_select_pilot_baudrates);
    
}

void OptionWidget::onMousePage3(double x, double y){
    GpsFramework & f = GpsFramework::Instance();
    
    if(onMouseSelectButton(m_select_pilot_serial, x, y)){
        f.m_config.m_inputPilot = m_select_pilot_serial->getValueString();
    };
    
    if(onMouseSelectButton(m_select_pilot_baudrates, x, y)){
        f.m_config.m_baudratePilot = m_select_pilot_baudrates->getValueInt();
    }
    
    if(onMouseSelectButton(m_select_pilot_langage, x, y)){
        f.m_config.m_pilot_langage = m_select_pilot_langage->m_selectedValue;
    }
    
    f.m_config.m_pilot_lookahead_d = f.m_config.m_pilot_lookahead_d * m_value_gui_lookahead_d->getMultValue(x,y);
    f.initOrLoadConfig();
    
}



/**
 PAGE 4
 */

void OptionWidget::setPage4(){
    m_button_sens = new ButtonGui(0.35, 0.25, PETIT_RAYON2, 0);
    m_button_debug = new ButtonGui(0.35, 0.35, PETIT_RAYON2, 0);
};

void OptionWidget::drawPage4(){
    GpsFramework & f = GpsFramework::Instance();
    drawText("Avancé", 0.55*m_width, 0.1*m_height, 20, true);
    
    drawText("Sens de la ligne", 0.4, 0.25);
    drawText("Mode technicien", 0.4, 0.35);
    
    if(f.m_config.m_sensDraw){
        drawButtonLabel(m_button_sens, COLOR_CHECK);
    } else {
        drawButtonLabel(m_button_sens, COLOR_OTHER);
        
    }
    
    if(f.m_config.m_debug){
        drawButtonLabel(m_button_debug, COLOR_CHECK);
    } else {
        drawButtonLabel(m_button_debug, COLOR_OTHER);
        
    }
}

void OptionWidget::onMousePage4(double x, double y){
    GpsFramework & f = GpsFramework::Instance();
    
    if(m_button_sens->isActive(x,y)){
        GpsFramework & f = GpsFramework::Instance();
        f.m_config.m_sensDraw = !f.m_config.m_sensDraw;
    } else if(m_button_debug->isActive(x,y)){
        GpsFramework & f = GpsFramework::Instance();
        f.m_config.m_debug = !f.m_config.m_debug;
    }
    
    f.initOrLoadConfig();
    
}

/**
 PAGE 5
 */

void OptionWidget::setPage5(){
    double dist2 = 0.08;
    
    m_button_inverse = new ButtonGui(0.35, 0.20, PETIT_RAYON2);
    
    m_motor_vitesse_min = new ValueGui(0.35, 0.3, PETIT_RAYON2, 0, "vitesse moteur min ");
    m_motor_vitesse_max = new ValueGui(0.35, 0.35, PETIT_RAYON2, 0, "vitesse moteur max ");
    m_motor_vitesse_agressivite = new ValueGui(0.35, 0.4, PETIT_RAYON2, 0, "vitesse moteur agressivite ");
    
    m_button_algo2_goto_pas_by_tour = new ValueGui(0.35, 0.5, PETIT_RAYON2, 0, "goto pas by tour ");
    m_button_algo2_goto_angle_by_tour = new ValueGui(0.35, 0.55, PETIT_RAYON2, 0, "goto angle by tour ");
    m_button_algo2_goto_rel_s = new ValueGui(0.35, 0.6, PETIT_RAYON2, 0, "goto rel ");
};


void OptionWidget::drawPage5(){
    GpsFramework & f = GpsFramework::Instance();
    
    drawText("Autoguidage", 0.55*m_width, 0.1*m_height, 20, true);
    
    drawText(f.m_pilotModule.m_version_guidage, 0.55*m_width, 0.20*m_height, 11, true);
    
    if(f.m_config.m_pilot_inverse){
        drawButtonLabel(m_button_inverse, COLOR_CHECK);
    } else {
        drawButtonLabel(m_button_inverse, COLOR_OTHER);
    }
    drawText("Inverse", 0.4, m_button_inverse->m_y);
    
    drawValueGui(m_motor_vitesse_min, f.m_config.m_motor_vitesse_min);
    drawValueGui(m_motor_vitesse_max, f.m_config.m_motor_vitesse_max);
    drawValueGui(m_motor_vitesse_agressivite, f.m_config.m_motor_vitesse_agressivite);

    drawValueGui(m_button_algo2_goto_pas_by_tour, f.m_config.m_volant_pas_by_tour);
    drawValueGui(m_button_algo2_goto_angle_by_tour, f.m_config.m_volant_angle_by_tour);
    drawValueGui(m_button_algo2_goto_rel_s, f.m_config.m_volant_derive);
}

void OptionWidget::onMousePage5(double x, double y){
    GpsFramework & f = GpsFramework::Instance();
    
    if(m_button_inverse->isActive(x,y)){
        f.m_config.m_pilot_inverse = !f.m_config.m_pilot_inverse;
    }
    
    f.m_config.m_motor_vitesse_min += 2*m_motor_vitesse_min->getIntValue(x,y);
    if(f.m_config.m_motor_vitesse_min > 70){
        f.m_config.m_motor_vitesse_min = 70;
    }
    if(f.m_config.m_motor_vitesse_min < 0){
        f.m_config.m_motor_vitesse_min = 0;
    }
    
    f.m_config.m_motor_vitesse_max += 2*m_motor_vitesse_max->getIntValue(x,y);
    if(f.m_config.m_motor_vitesse_max > 100){
        f.m_config.m_motor_vitesse_max = 100;
    }
    if(f.m_config.m_motor_vitesse_max < 0){
        f.m_config.m_motor_vitesse_max = 0;
    }
    f.m_config.m_motor_vitesse_agressivite = f.m_config.m_motor_vitesse_agressivite * m_motor_vitesse_agressivite->getMultValue(x,y);
    
    
    
    
    f.m_config.m_volant_pas_by_tour = f.m_config.m_volant_pas_by_tour * m_button_algo2_goto_pas_by_tour->getMultValue(x,y);
    f.m_config.m_volant_angle_by_tour = f.m_config.m_volant_angle_by_tour * m_button_algo2_goto_angle_by_tour->getMultValue(x,y);
    f.m_config.m_volant_derive = f.m_config.m_volant_derive * m_button_algo2_goto_rel_s->getMultValue(x,y);
    
    
    f.initOrLoadConfig();
    
}


/**
 PAGE 6
 */

void OptionWidget::setPage6(){
    m_button_p6connect = new ButtonGui(0.40, 0.25, PETIT_RAYON2, 0);
    m_button_p6disable = new ButtonGui(0.55, 0.25, PETIT_RAYON2, 0);
    m_button_p6clearError = new ButtonGui(0.70, 0.25, PETIT_RAYON2, 0);
    
    m_button_p6testGoToVLeft = new ButtonGui(0.40, 0.55, PETIT_RAYON2, 0);
    m_button_p6testGoToV0 = new ButtonGui(0.55, 0.55, PETIT_RAYON2, 0);
    m_button_p6testGoToVRight = new ButtonGui(0.70, 0.55, PETIT_RAYON2, 0);
    /*    ;
     m_button_p6testGoToV0;
     m_button_p6testGoToVRight;*/
};

void OptionWidget::drawPage6(){
    GpsFramework & f = GpsFramework::Instance();
    drawText("Test Volant", 0.55*m_width, 0.1*m_height, 20, true);
    
    drawButtonLabel(m_button_p6connect, COLOR_OTHER);
    drawText("connect", m_button_p6connect->m_x*m_width, m_button_p6connect->m_y*m_height, 12, true);
    
    drawButtonLabel(m_button_p6disable, COLOR_OTHER);
    drawText("disconnect", m_button_p6disable->m_x*m_width, m_button_p6disable->m_y*m_height, 12, true);
    
    drawText("Goto Volant", 0.55*m_width, 0.50*m_height, 12, true);
    drawButtonLabel(m_button_p6testGoToVLeft, COLOR_OTHER);
    drawText("-0.5 volant", m_button_p6testGoToVLeft->m_x*m_width, m_button_p6testGoToVLeft->m_y*m_height, 12, true);
    
    drawButtonLabel(m_button_p6testGoToV0, COLOR_OTHER);
    drawText("0", m_button_p6testGoToV0->m_x*m_width, m_button_p6testGoToV0->m_y*m_height, 12, true);
    
    drawButtonLabel(m_button_p6testGoToVRight, COLOR_OTHER);
    drawText("+0.5 volant", m_button_p6testGoToVRight->m_x*m_width, m_button_p6testGoToVRight->m_y*m_height, 12, true);
}

void OptionWidget::onMousePage6(double x, double y){
    GpsFramework & f = GpsFramework::Instance();
    
    
    m_button_p6testGoToVLeft = new ButtonGui(0.40, 0.55, PETIT_RAYON2, 0);
    m_button_p6testGoToV0 = new ButtonGui(0.55, 0.55, PETIT_RAYON2, 0);
    m_button_p6testGoToVRight = new ButtonGui(0.70, 0.55, PETIT_RAYON2, 0);
    
    if(m_button_p6connect->isActive(x,y)){
        f.m_pilotModule.engage();
    } else if(m_button_p6disable->isActive(x,y)){
        f.m_pilotModule.desengage();
    } else if(m_button_p6testGoToVLeft->isActive(x,y)){
        f.m_pilotModule.engage();
        f.m_pilotModule.myGotoVolant2(-0.5);
    } else if(m_button_p6testGoToV0->isActive(x,y)){
        f.m_pilotModule.engage();
        f.m_pilotModule.myGotoVolant2(0);
    } else if(m_button_p6testGoToVRight->isActive(x,y)){
        f.m_pilotModule.engage();
        f.m_pilotModule.myGotoVolant2(0.5);
    }
    
    f.initOrLoadConfig();
    
}


void OptionWidget::onMouse(double x, double y){
    
    if(m_button_close->isActive(x,y)){
        m_close = true;
    } else if(m_button_p1->isActive(x,y)){
        m_page = 1;
    } else if(m_button_p2->isActive(x,y)){
        m_page = 2;
    } else if(m_button_p3->isActive(x,y)){
        m_page = 3;
    } else if(m_button_p4->isActive(x,y)){
        m_page = 4;
    } else if(m_button_p5->isActive(x,y)){
        m_page = 5;
    } else if(m_button_p6->isActive(x,y)){
        m_page = 6;
    } else {
        if(m_page == 1){
            onMousePage1(x, y);
        } else if(m_page == 2){
            onMousePage2(x, y);
        } else if(m_page == 3){
            onMousePage3(x, y);
        } else if(m_page == 4){
            onMousePage4(x, y);
        } else if(m_page == 5){
            onMousePage5(x, y);
        } else if(m_page == 6){
            onMousePage6(x, y);
        }
    }
}

void OptionWidget::onMousePage1(double x, double y){
    if(m_button_p1add1->isActive(x,y)){
        GpsFramework & f = GpsFramework::Instance();
        f.m_config.m_largeur += 0.1;
        f.initOrLoadConfig();
    } else if(m_button_p1minus1->isActive(x,y)){
        GpsFramework & f = GpsFramework::Instance();
        f.m_config.m_largeur -= 0.1;
        f.initOrLoadConfig();
        
    } else if(m_button_p1add2->isActive(x,y)){
        GpsFramework & f = GpsFramework::Instance();
        f.m_config.m_largeur += 1;
        f.initOrLoadConfig();
        
    } else if(m_button_p1minus2->isActive(x,y)){
        GpsFramework & f = GpsFramework::Instance();
        f.m_config.m_largeur -= 1;
        f.initOrLoadConfig();
    }
}


void OptionWidget::open(){
    addSerials();
}

void OptionWidget::addSerials(){
    GpsFramework & f = GpsFramework::Instance();
    
    std::vector<std::string> serials;
    serials.push_back("none");
    serials.push_back("file");
    std::vector<std::string> & s2 = f.m_serialModule.getAvailablePorts();
    for(auto s : s2){
        serials.push_back(s);
    }
    
    
    
    
    m_buttonp2Serials.clear();
    m_select_pilot_serial->clear();
    int i=2;
    y_port_title = 0.2;
    for(auto serial: serials){
        auto button = new ButtonGui(0.3, y_port_title+i*0.05, 0.02, 0);
        button->m_label = serial;
        m_buttonp2Serials.push_back(button);
        
        if(serial != "file"){
            m_select_pilot_serial->addValue(serial);
        }
        
        ++i;
    }
    
    y_baurates_title = y_port_title+i*0.05;
    ++i;
    ++i;
    std::list<int> baurates;
    
    baurates.push_back(9600);
    baurates.push_back(115200);
    for(auto baurate: baurates){
        auto button = new ButtonGui(0.3, y_port_title+i*0.05, 0.02, 0);
        button->m_label = std::to_string(baurate);
        button->m_labelInt = baurate;
        m_buttonp2Baurates.push_back(button);
        ++i;
    }
}


/*
 
 
 if(m_button_p5test1->isActive(x,y)){
 f.m_pilotModule.test(-1);
 return;
 } else if(m_button_p5test2->isActive(x,y)){
 f.m_pilotModule.test(0);
 return;
 } else if(m_button_p5test3->isActive(x,y)){
 f.m_pilotModule.test(1);
 return;
 } else if(m_button_p5connect->isActive(x,y)){
 f.m_pilotModule.run_test(0);
 return;
 } else if(m_button_p5disable->isActive(x,y)){
 f.m_pilotModule.run_test(1);
 return;
 } else if(m_button_p5clearError->isActive(x,y)){
 f.m_pilotModule.run_test(2);
 return;
 }
 */
