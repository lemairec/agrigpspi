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
    m_imgShutDown = loadImage("/images/close.png");
    
    //m_close=false;
    //m_page =5;
    //addSerials();
}

void OptionWidget::setSize(int width, int height){
    BaseWidget::setSize(width, height);
    m_button_close.setResize(0.75*m_width, 0.83*m_height, m_gros_button);
    
    m_button_p1.setResize(0.24*m_width, 0.20*m_height, m_gros_button);
    m_button_p2.setResize(0.24*m_width, 0.30*m_height, m_gros_button);
    m_button_p3.setResize(0.24*m_width, 0.40*m_height, m_gros_button);
    m_button_p4.setResize(0.24*m_width, 0.50*m_height, m_gros_button);
    m_button_p5.setResize(0.24*m_width, 0.60*m_height, m_gros_button);
    m_button_p6.setResize(0.24*m_width, 0.70*m_height, m_gros_button);
    
    resizePage1();
    resizePage2();
    resizePage3();
    resizePage4();
    resizePage5();
    resizePage6();
    
}

void OptionWidget::draw(){
    scene->addRect(m_width*0.2, m_height*0.1, m_width*0.6, m_height*0.8, m_penBlack, m_brushWhiteAlpha);
    scene->addRect(m_width*0.2, m_height*0.1, m_width*0.08, m_height*0.8, m_penBlack, m_brushDarkGray);
    
    drawButtonImage(&m_button_close, *m_imgClose);
    
    if(m_page == 1){
        drawButtonImage(&m_button_p1, *m_imgOutilBlanc);
        drawPage1();
    } else {
        drawButtonImage(&m_button_p1, *m_imgOutilGris);
    }
    
    if(m_page == 2){
        drawButtonImage(&m_button_p2, *m_imgSatBlanc);
        drawPage2();
    } else {
        drawButtonImage(&m_button_p2, *m_imgSatGris);
        
    }
    
    if(m_page == 3){
        drawButtonImage(&m_button_p3, *m_imgVolantBlanc);
        drawPage3();
    } else {
        drawButtonImage(&m_button_p3, *m_imgVolantGris);
        
    }
    
    if(m_page == 4){
        drawButtonImage(&m_button_p4, *m_imgOptionBlanc);
        drawPage4();
    } else {
        drawButtonImage(&m_button_p4, *m_imgOptionGris);
    }
    
    GpsFramework & f = GpsFramework::Instance();
    if(f.m_config.m_debug){
        if(m_page == 5){
            drawButtonImage(&m_button_p5, *m_imgOptionBlanc);
            drawPage5();
        } else {
            drawButtonImage(&m_button_p5, *m_imgOptionGris);
        }
        
        if(m_page == 6){
            drawButtonImage(&m_button_p6, *m_imgOptionBlanc);
            drawPage6();
        } else {
            drawButtonImage(&m_button_p6, *m_imgOptionGris);
        }
    }
}

void OptionWidget::onMouseInt(int x, int y){
    
    if(m_button_close.isActive(x,y)){
        m_close = true;
    } else if(m_button_p1.isActive(x,y)){
        m_page = 1;
    } else if(m_button_p2.isActive(x,y)){
        m_page = 2;
    } else if(m_button_p3.isActive(x,y)){
        m_page = 3;
    } else if(m_button_p4.isActive(x,y)){
        m_page = 4;
    } else if(m_button_p5.isActive(x,y)){
        m_page = 5;
    } else if(m_button_p6.isActive(x,y)){
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

/**
 PAGE 1
 */

void OptionWidget::resizePage1(){
    double dist = 0.03;
    m_button_p1add1.setResize((0.6-dist)*m_width, 0.3*m_height, m_petit_button);
    m_button_p1add2.setResize((0.6+dist)*m_width, 0.3*m_height, m_petit_button);
    
    m_button_p1minus1.setResize((0.4+dist)*m_width, 0.3*m_height, m_petit_button);
    m_button_p1minus2.setResize((0.4-dist)*m_width, 0.3*m_height, m_petit_button);
}

void OptionWidget::drawPage1(){
    GpsFramework & f = GpsFramework::Instance();
    QString s = "largueur de l'outil " + QString::number(f.m_config.m_outil_largeur, 'f', 2) + " m";
    drawText(s.toUtf8().constData(), 0.55*m_width, 0.15*m_height, sizeText_big, true);
    
    drawButtonImage(&m_button_p1add1, *m_imgPlus);
    drawButtonImage(&m_button_p1add2, *m_imgPlus);
    drawButtonImage(&m_button_p1minus1, *m_imgMinus);
    drawButtonImage(&m_button_p1minus2, *m_imgMinus);
}

void OptionWidget::onMousePage1(int x, int y){
    if(m_button_shudown.isActive(x, y)){
    } else if(m_button_p1add1.isActive(x,y)){
        GpsFramework & f = GpsFramework::Instance();
        f.m_config.m_outil_largeur += 0.1;
        f.initOrLoadConfig();
    } else if(m_button_p1minus1.isActive(x,y)){
        GpsFramework & f = GpsFramework::Instance();
        f.m_config.m_outil_largeur -= 0.1;
        f.initOrLoadConfig();
        
    } else if(m_button_p1add2.isActive(x,y)){
        GpsFramework & f = GpsFramework::Instance();
        f.m_config.m_outil_largeur += 1;
        f.initOrLoadConfig();
        
    } else if(m_button_p1minus2.isActive(x,y)){
        GpsFramework & f = GpsFramework::Instance();
        f.m_config.m_outil_largeur -= 1;
        f.initOrLoadConfig();
    }
}



/**
 PAGE 2
 */

void OptionWidget::resizePage2(){
    m_select_gps_serial.setResize(0.35*m_width,0.3*m_height, m_petit_button);
    
    m_select_gps_baudrates.clear();
    m_select_gps_baudrates.setResize(0.35*m_width,0.4*m_height, m_petit_button);
    m_select_gps_baudrates.addValueInt("9600", 9600);
    m_select_gps_baudrates.addValueInt("115200", 115200);
}

void OptionWidget::drawPage2(){
    drawText("Connection GPS", 0.55*m_width, 0.15*m_height, sizeText_big, true);
    
    GpsFramework & f = GpsFramework::Instance();
    
    m_select_gps_baudrates.setValueInt(f.m_config.m_baudrate_gps);
    m_select_gps_serial.setValueString(f.m_config.m_input_gps);
    
    drawSelectButtonGuiClose(&m_select_gps_serial);
    drawSelectButtonGuiClose(&m_select_gps_baudrates);
    
    drawSelectButtonGuiOpen(&m_select_gps_serial);
    drawSelectButtonGuiOpen(&m_select_gps_baudrates);
    
}

void OptionWidget::onMousePage2(int x, int y){
    GpsFramework & f = GpsFramework::Instance();
    
    if(onMouseSelectButton(&m_select_gps_serial, x, y)){
        f.m_config.m_input_gps = m_select_gps_serial.getValueString();
    };
    
    if(onMouseSelectButton(&m_select_gps_baudrates, x, y)){
        f.m_config.m_baudrate_gps = m_select_gps_baudrates.getValueInt();
    }
    f.initOrLoadConfig();
}


/**
 PAGE 3
 */

void OptionWidget::resizePage3(){
    m_select_pilot_serial.setResize(0.35*m_width,0.3*m_height, m_petit_button);
    
    m_select_pilot_baudrates.clear();
    m_select_pilot_baudrates.setResize(0.35*m_width,0.4*m_height, m_petit_button);
    m_select_pilot_baudrates.addValueInt("9600", 9600);
    m_select_pilot_baudrates.addValueInt("115200", 115200);
    
    
    m_select_pilot_langage.clear();
    m_select_pilot_langage.setResize(0.35*m_width,0.5*m_height, m_petit_button);
    m_select_pilot_langage.addValue("arduino");
    m_select_pilot_langage.addValue("hadrien");
    
    m_value_gui_lookahead_d.setResize(0.35*m_width, 0.7*m_height, m_petit_button, "lookahead ");
    
    
};

void OptionWidget::drawPage3(){
    GpsFramework & f = GpsFramework::Instance();
    drawText("Connection Pilot", 0.55*m_width, 0.15*m_height, sizeText_big, true);
    
    
    m_select_pilot_baudrates.setValueInt(f.m_config.m_baudratePilot);
    m_select_pilot_serial.setValueString(f.m_config.m_inputPilot);
    m_select_pilot_langage.m_selectedValue = f.m_config.m_pilot_langage;
    
    drawSelectButtonGuiClose(&m_select_pilot_langage);
    drawSelectButtonGuiClose(&m_select_pilot_baudrates);
    drawSelectButtonGuiClose(&m_select_pilot_serial);
    
    drawValueGui(&m_value_gui_lookahead_d, f.m_config.m_pilot_lookahead_d);
    
    drawSelectButtonGuiOpen(&m_select_pilot_serial);
    drawSelectButtonGuiOpen(&m_select_pilot_langage);
    drawSelectButtonGuiOpen(&m_select_pilot_baudrates);
    
}

void OptionWidget::onMousePage3(double x, double y){
    GpsFramework & f = GpsFramework::Instance();
    
    if(onMouseSelectButton(&m_select_pilot_serial, x, y)){
        f.m_config.m_inputPilot = m_select_pilot_serial.getValueString();
    };
    
    if(onMouseSelectButton(&m_select_pilot_baudrates, x, y)){
        f.m_config.m_baudratePilot = m_select_pilot_baudrates.getValueInt();
    }
    
    if(onMouseSelectButton(&m_select_pilot_langage, x, y)){
        f.m_config.m_pilot_langage = m_select_pilot_langage.m_selectedValue;
    }
    
    f.m_config.m_pilot_lookahead_d = f.m_config.m_pilot_lookahead_d * m_value_gui_lookahead_d.getMultValue(x,y);
    f.initOrLoadConfig();
    
}



/**
 PAGE 4
 */

void OptionWidget::resizePage4(){
    m_button_sens.setResize(0.35*m_width, 0.25*m_height, m_petit_button);
    m_button_debug.setResize(0.35*m_width, 0.35*m_height, m_petit_button);
};

void OptionWidget::drawPage4(){
    GpsFramework & f = GpsFramework::Instance();
    drawText("Avancé", 0.55*m_width, 0.15*m_height, sizeText_big, true);
    
    drawText("Sens de la ligne", 0.4*m_width, 0.25*m_height);
    drawText("Mode technicien", 0.4*m_width, 0.35*m_height);
    
    if(f.m_config.m_sensDraw){
        drawButtonLabel(&m_button_sens, COLOR_CHECK);
    } else {
        drawButtonLabel(&m_button_sens, COLOR_OTHER);
        
    }
    
    if(f.m_config.m_debug){
        drawButtonLabel(&m_button_debug, COLOR_CHECK);
    } else {
        drawButtonLabel(&m_button_debug, COLOR_OTHER);
        
    }
}

void OptionWidget::onMousePage4(double x, double y){
    GpsFramework & f = GpsFramework::Instance();
    
    if(m_button_sens.isActive(x,y)){
        GpsFramework & f = GpsFramework::Instance();
        f.m_config.m_sensDraw = !f.m_config.m_sensDraw;
    } else if(m_button_debug.isActive(x,y)){
        GpsFramework & f = GpsFramework::Instance();
        f.m_config.m_debug = !f.m_config.m_debug;
    }
    
    f.initOrLoadConfig();
    
}

/**
 PAGE 5
 */

void OptionWidget::resizePage5(){
    double dist2 = 0.08;
    
    m_button_motor_inverse.setResize(0.35*m_width, 0.20*m_height, m_petit_button);
    m_button_encoder_inverse.setResize(0.6*m_width, 0.20*m_height, m_petit_button);
    
    m_motor_vitesse_min.setResize(0.35*m_width, 0.3*m_height, m_petit_button, "vitesse moteur min ");
    m_motor_vitesse_max.setResize(0.35*m_width, 0.35*m_height, m_petit_button, "vitesse moteur max ");
    m_motor_vitesse_agressivite.setResize(0.35*m_width, 0.4*m_height, m_petit_button, "vitesse moteur agressivite ");
    
    m_button_algo2_goto_pas_by_tour.setResize(0.35*m_width, 0.5*m_height, m_petit_button, "goto pas by tour ");
    m_button_algo2_goto_angle_by_tour.setResize(0.35*m_width, 0.55*m_height, m_petit_button, "goto angle by tour ");
    m_button_algo2_goto_rel_s.setResize(0.35*m_width, 0.6*m_height, m_petit_button, "goto rel ");
};


void OptionWidget::drawPage5(){
    GpsFramework & f = GpsFramework::Instance();
    
    drawText("Autoguidage", 0.55*m_width, 0.15*m_height, sizeText_big, true);
    
    drawText(f.m_pilotModule.m_version_guidage, 0.55*m_width, 0.20*m_height, sizeText_medium, true);
    
    if(f.m_config.m_pilot_motor_inverse){
        drawButtonLabel(&m_button_motor_inverse, COLOR_CHECK);
    } else {
        drawButtonLabel(&m_button_motor_inverse, COLOR_OTHER);
    }
    drawText("Inv motor", 0.4*m_width, m_button_motor_inverse.m_y);
    if(f.m_config.m_pilot_encoder_inverse){
        drawButtonLabel(&m_button_encoder_inverse, COLOR_CHECK);
    } else {
        drawButtonLabel(&m_button_encoder_inverse, COLOR_OTHER);
    }
    drawText("Inv encoder", 0.65*m_width, m_button_motor_inverse.m_y);
    
    drawValueGui(&m_motor_vitesse_min, f.m_config.m_motor_vitesse_min);
    drawValueGui(&m_motor_vitesse_max, f.m_config.m_motor_vitesse_max);
    drawValueGui(&m_motor_vitesse_agressivite, f.m_config.m_motor_vitesse_agressivite);

    drawValueGui(&m_button_algo2_goto_pas_by_tour, f.m_config.m_volant_pas_by_tour);
    drawValueGui(&m_button_algo2_goto_angle_by_tour, f.m_config.m_volant_angle_by_tour);
    drawValueGui(&m_button_algo2_goto_rel_s, f.m_config.m_volant_derive);
}

void OptionWidget::onMousePage5(double x, double y){
    GpsFramework & f = GpsFramework::Instance();
    
    if(m_button_motor_inverse.isActive(x,y)){
        f.m_config.m_pilot_motor_inverse = !f.m_config.m_pilot_motor_inverse;
    }
    if(m_button_encoder_inverse.isActive(x,y)){
        f.m_config.m_pilot_encoder_inverse = !f.m_config.m_pilot_encoder_inverse;
    }
    
    f.m_config.m_motor_vitesse_min += 2*m_motor_vitesse_min.getIntValue(x,y);
    if(f.m_config.m_motor_vitesse_min > 70){
        f.m_config.m_motor_vitesse_min = 70;
    }
    if(f.m_config.m_motor_vitesse_min < 0){
        f.m_config.m_motor_vitesse_min = 0;
    }
    
    f.m_config.m_motor_vitesse_max += 2*m_motor_vitesse_max.getIntValue(x,y);
    if(f.m_config.m_motor_vitesse_max > 100){
        f.m_config.m_motor_vitesse_max = 100;
    }
    if(f.m_config.m_motor_vitesse_max < 0){
        f.m_config.m_motor_vitesse_max = 0;
    }
    f.m_config.m_motor_vitesse_agressivite = f.m_config.m_motor_vitesse_agressivite * m_motor_vitesse_agressivite.getMultValue(x,y);
    
    
    
    
    f.m_config.m_volant_pas_by_tour = f.m_config.m_volant_pas_by_tour * m_button_algo2_goto_pas_by_tour.getMultValue(x,y);
    f.m_config.m_volant_angle_by_tour = f.m_config.m_volant_angle_by_tour * m_button_algo2_goto_angle_by_tour.getMultValue(x,y);
    f.m_config.m_volant_derive = f.m_config.m_volant_derive * m_button_algo2_goto_rel_s.getMultValue(x,y);
    
    
    f.initOrLoadConfig();
    
}


/**
 PAGE 6
 */

void OptionWidget::resizePage6(){
    m_button_p6connect.setResize(0.40*m_width, 0.25*m_height, m_petit_button);
    m_button_p6disable.setResize(0.55*m_width, 0.25*m_height, m_petit_button);
    m_button_p6clearError.setResize(0.70*m_width, 0.25*m_height, m_petit_button);
    
    m_button_p6testGoToVLeft.setResize(0.40*m_width, 0.55*m_height, m_petit_button);
    m_button_p6testGoToV0.setResize(0.55*m_width, 0.55*m_height, m_petit_button);
    m_button_p6testGoToVRight.setResize(0.70*m_width, 0.55*m_height, m_petit_button);
};

void OptionWidget::drawPage6(){
    GpsFramework & f = GpsFramework::Instance();
    drawText("Test Volant", 0.55*m_width, 0.15*m_height, sizeText_big, true);
    
    drawButtonLabel(&m_button_p6connect, COLOR_OTHER);
    drawText("connect", m_button_p6connect.m_x, m_button_p6connect.m_y, sizeText_little, true);
    
    drawButtonLabel(&m_button_p6disable, COLOR_OTHER);
    drawText("disconnect", m_button_p6disable.m_x, m_button_p6disable.m_y, sizeText_little, true);
    
    drawText("Goto Volant", 0.55*m_width, 0.50*m_height, sizeText_medium, true);
    drawButtonLabel(&m_button_p6testGoToVLeft, COLOR_OTHER);
    drawText("-0.5 volant", m_button_p6testGoToVLeft.m_x, m_button_p6testGoToVLeft.m_y, sizeText_little, true);
    
    drawButtonLabel(&m_button_p6testGoToV0, COLOR_OTHER);
    drawText("0", m_button_p6testGoToV0.m_x, m_button_p6testGoToV0.m_y, sizeText_little, true);
    
    drawButtonLabel(&m_button_p6testGoToVRight, COLOR_OTHER);
    drawText("+0.5 volant", m_button_p6testGoToVRight.m_x, m_button_p6testGoToVRight.m_y, sizeText_little, true);
}

void OptionWidget::onMousePage6(double x, double y){
    GpsFramework & f = GpsFramework::Instance();
    
    if(m_button_p6connect.isActive(x,y)){
        f.m_pilotModule.engage();
    } else if(m_button_p6disable.isActive(x,y)){
        f.m_pilotModule.desengage();
    } else if(m_button_p6testGoToVLeft.isActive(x,y)){
        f.m_pilotModule.engage();
        f.m_pilotModule.myGotoVolant2(-0.5);
    } else if(m_button_p6testGoToV0.isActive(x,y)){
        f.m_pilotModule.engage();
        f.m_pilotModule.myGotoVolant2(0);
    } else if(m_button_p6testGoToVRight.isActive(x,y)){
        f.m_pilotModule.engage();
        f.m_pilotModule.myGotoVolant2(0.5);
    }
    
    f.initOrLoadConfig();
    
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
    
    
    
    
    m_select_gps_serial.clear();
    m_select_pilot_serial.clear();
    int i=2;
    y_port_title = 0.2;
    for(auto serial: serials){
        m_select_gps_serial.addValue(serial);
        
        if(serial != "file"){
            m_select_pilot_serial.addValue(serial);
        }
    }
}
