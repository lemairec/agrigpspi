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
    m_imgImuBlanc = loadImage("/images/imu_blanc.png");
    m_imgImuGris = loadImage("/images/imu_gris.png");
    
    //m_close=false;
    //m_page =5;
    //addSerials();
}

void OptionWidget::setSize(int width, int height){
    BaseWidget::setSize(width, height);
    m_button_close.setResize(0.75*m_width, 0.83*m_height, m_gros_button);
    
    m_button_p1.setResize(0.1*m_width, 0.20*m_height, m_gros_button);
    m_button_p2.setResize(0.1*m_width, 0.30*m_height, m_gros_button);
    m_button_p3.setResize(0.1*m_width, 0.40*m_height, m_gros_button);
    m_button_p4.setResize(0.1*m_width, 0.50*m_height, m_gros_button);
    m_button_p5.setResize(0.1*m_width, 0.60*m_height, m_gros_button);
    m_button_p6.setResize(0.1*m_width, 0.70*m_height, m_gros_button);
    
    resizePage1();
    resizePage2();
    resizePage3();
    resizePage4();
    resizePage5();
    resizePage6();
    
}

void OptionWidget::draw(){
    m_painter->setPen(m_penBlack);
    m_painter->setBrush(m_brushWhiteAlpha);
    m_painter->drawRect(m_width*0.05, m_height*0.1, m_width*0.9, m_height*0.8);
    m_painter->setBrush(m_brushDarkGray);
    m_painter->drawRect(m_width*0.05, m_height*0.1, m_width*0.1, m_height*0.8);
    
    drawButtonImage(m_button_close, m_imgClose);
    
    if(m_page == 1){
        drawButtonImage(m_button_p1, m_imgOptionBlanc);
        drawPage1();
    } else {
        drawButtonImage(m_button_p1, m_imgOptionGris);
    }
    
    if(m_page == 2){
        drawButtonImage(m_button_p2, m_imgSatBlanc);
        drawPage2();
    } else {
        drawButtonImage(m_button_p2, m_imgSatGris);
        
    }
    
    if(m_page == 3){
        drawButtonImage(m_button_p3, m_imgImuBlanc, 0.5);
        drawPage3();
    } else {
        drawButtonImage(m_button_p3, m_imgImuGris, 0.5);
        
    }
    
    if(m_page == 4){
        drawButtonImage(m_button_p4, m_imgVolantBlanc);
        drawPage4();
    } else {
        drawButtonImage(m_button_p4, m_imgVolantGris);
    }
    
    //GpsFramework & f = GpsFramework::Instance();
    
    if(m_page == 5){
        drawButtonImage(m_button_p5, m_imgVolantBlanc);
        drawPage5();
    } else {
        drawButtonImage(m_button_p5, m_imgVolantGris);
    }
    
    if(m_page == 6){
        drawButtonImage(m_button_p6, m_imgVolantBlanc);
        drawPage6();
    } else {
        drawButtonImage(m_button_p6, m_imgVolantGris);
    }
}

void OptionWidget::onMouse(int x, int y){
    
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

double x1 = 0.22;
double x1bis = 0.27;


double x2 = 0.6;
double x2bis = 0.65;

void OptionWidget::drawPart1(double y, double h, std::string title){
    m_painter->setPen(m_penBlack);
    m_painter->setBrush(m_brushNo);
    m_painter->drawRect(m_width*0.17, y, m_width*0.37, h);
    drawText(title, m_width*(0.17+0.37/2), y-7, sizeText_medium, true);
}

void OptionWidget::drawPart2(double y, double h, std::string title){
    m_painter->setPen(m_penBlack);
    m_painter->setBrush(m_brushNo);
    m_painter->drawRect(m_width*0.55, y, m_width*0.37, h);
    drawText(title, m_width*(0.55+0.37/2), y-7, sizeText_medium, true);
}

void OptionWidget::resizePage1(){
    m_button_sens.setResize(x1*m_width, 0.3*m_height, m_petit_button);
    m_button_debug_file.setResize(x1*m_width, 0.4*m_height, m_petit_button);
    m_button_menu_vertical.setResize(x1*m_width, 0.5*m_height, m_petit_button);
    m_button_debug.setResize(x1*m_width, 0.6*m_height, m_petit_button);
    m_button_auto.setResize(x1*m_width, 0.7*m_height, m_petit_button);
};

void OptionWidget::drawPage1(){
    GpsFramework & f = GpsFramework::Instance();
    drawText("Option", 0.55*m_width, 0.15*m_height, sizeText_big, true);
    
    
    drawText("Sens de la ligne", x1bis*m_width, m_button_sens.m_y);
    if(f.m_config.m_sensDraw){
        drawButtonLabel(m_button_sens, COLOR_CHECK);
    } else {
        drawButtonLabel(m_button_sens, COLOR_OTHER);
        
    }
    
    drawText("Mode technicien", x1bis*m_width, m_button_debug.m_y);
    if(f.m_config.m_debug){
        drawButtonLabel(m_button_debug, COLOR_CHECK);
    } else {
        drawButtonLabel(m_button_debug, COLOR_OTHER);
        
    }
    
    drawText("Menu Vertical", x1bis*m_width, m_button_menu_vertical.m_y);
    if(f.m_config.m_menu_vertical){
        drawButtonLabel(m_button_menu_vertical, COLOR_CHECK);
    } else {
        drawButtonLabel(m_button_menu_vertical, COLOR_OTHER);
    }
    
    drawText("Debug Log File", x1bis*m_width, m_button_debug_file.m_y);
    if(f.m_config.m_debug_log){
        drawButtonLabel(m_button_debug_file, COLOR_CHECK);
    } else {
        drawButtonLabel(m_button_debug_file, COLOR_OTHER);
    }
    
    QString s;
    s = "Recherche auto des ports " + QString::number(f.m_serialModule.m_serial_searchs_i) + " / 1000";
    drawQText(s, x1bis*m_width, m_button_auto.m_y);
    drawButtonLabel(m_button_auto, COLOR_OTHER);

}

void OptionWidget::onMousePage1(int x, int y){
    GpsFramework & f = GpsFramework::Instance();
    
    if(m_button_sens.isActive(x,y)){
        f.m_config.m_sensDraw = !f.m_config.m_sensDraw;
    } else if(m_button_debug.isActive(x,y)){
        f.m_config.m_debug = !f.m_config.m_debug;
    } else if(m_button_debug_file.isActive(x,y)){
        f.m_config.m_debug_log = !f.m_config.m_debug_log;
    } else if(m_button_menu_vertical.isActive(x,y)){
        f.m_config.m_menu_vertical = !f.m_config.m_menu_vertical;
    } else if(m_button_auto.isActive(x,y)){
        f.m_serialModule.rechercheAuto();
    }

    f.initOrLoadConfig();
}


/**
 PAGE 2
 */

void OptionWidget::resizePage2(){
    m_select_gps_serial.setResize(x1*m_width,0.3*m_height, m_petit_button);
    
    m_select_gps_baudrates.clear();
    m_select_gps_baudrates.setResize(x1*m_width,0.4*m_height, m_petit_button);
    m_select_gps_baudrates.addValueInt("9600", 9600);
    m_select_gps_baudrates.addValueInt("115200", 115200);
    
    m_lissage_gps.setResize(x2*m_width, 0.3*m_height, m_petit_button);
    m_lissage_gps.clear();
    m_lissage_gps.addValue("none");
    m_lissage_gps.addValue("ekf");
    m_lissage_gps_ekf.setResize(x2*m_width, 0.4*m_height, m_petit_button, "ekf_lissage ");
    
    m_cap.setResize(x2*m_width, 0.6*m_height, m_petit_button);
    m_cap.clear();
    m_cap.addValue("custom");
    m_cap.addValue("ekf");
    m_cap.addValue("rmc");
    
    m_cap_d.setResize(x2*m_width, 0.7*m_height, m_petit_button, "cap d ");
    m_cap_ekf.setResize(x2*m_width, 0.7*m_height, m_petit_button, "ekf_cap ");
    
}



void OptionWidget::drawPage2(){
    drawText("GPS", 0.55*m_width, 0.15*m_height, sizeText_big, true);
    
    
    
    GpsFramework & f = GpsFramework::Instance();
    
    drawSelectButtonGuiClose(m_select_gps_serial);
    drawSelectButtonGuiClose(m_select_gps_baudrates);
    drawSelectButtonGuiClose(m_lissage_gps);
    drawSelectButtonGuiClose(m_cap);
    
    
    drawPart1(0.25*m_height, 0.25*m_height, "connection");
    m_select_gps_baudrates.setValueInt(f.m_config.m_baudrate_gps);
    m_select_gps_serial.setValueString(f.m_config.m_input_gps);
    
    drawPart2(0.25*m_height, 0.25*m_height, "lissage Gps");
    m_lissage_gps.m_selectedValue = f.m_config.m_lissage_gps_mode;
    if(f.m_config.m_lissage_gps_mode == (int)LissageGpsMode_Ekf){
        drawValueGui(m_lissage_gps_ekf, f.m_config.m_lissage_gps_ekf*100);
    }
    
    drawPart2(0.55*m_height, 0.25*m_height, "cap");
    m_cap.m_selectedValue = f.m_config.m_cap_mode;
    if(f.m_config.m_cap_mode == (int)CapMode_Ekf){
        drawValueGui(m_cap_ekf, f.m_config.m_cap_ekf*100);
    }
    if(f.m_config.m_cap_mode == (int)CapMode_Custom){
        drawValueGui(m_cap_d, f.m_config.m_cap_custom_d);
    }
    
    drawSelectButtonGuiOpen(m_select_gps_serial);
    drawSelectButtonGuiOpen(m_select_gps_baudrates);
    drawSelectButtonGuiOpen(m_lissage_gps);
    drawSelectButtonGuiOpen(m_cap);
    
    
    
    
}

void OptionWidget::onMousePage2(int x, int y){
    GpsFramework & f = GpsFramework::Instance();
    
    if(onMouseSelectButton(m_select_gps_serial, x, y)){
        f.m_config.m_input_gps = m_select_gps_serial.getValueString();
    } else if(onMouseSelectButton(m_select_gps_baudrates, x, y)){
        f.m_config.m_baudrate_gps = m_select_gps_baudrates.getValueInt();
    }
    
    if(onMouseSelectButton(m_lissage_gps, x, y)){
        f.m_config.m_lissage_gps_mode = m_lissage_gps.m_selectedValue;
    }
    if(f.m_config.m_lissage_gps_mode == (int)LissageGpsMode_Ekf){
        f.m_config.m_lissage_gps_ekf += 0.05*m_lissage_gps_ekf.getIntValue(x,y);
    }
    
    if(onMouseSelectButton(m_cap, x, y)){
        f.m_config.m_cap_mode = m_cap.m_selectedValue;
    }
    if(f.m_config.m_cap_mode == (int)CapMode_Ekf){
        f.m_config.m_cap_ekf += 0.05*m_cap_ekf.getIntValue(x,y);
    }
    if(f.m_config.m_cap_mode == (int)CapMode_Custom){
        f.m_config.m_cap_custom_d += 0.5*m_cap_d.getIntValue(x,y);
    }
    
    
    f.initOrLoadConfig();
}



/**
 PAGE 4
 */

void OptionWidget::resizePage4(){
    m_select_pilot_serial.setResize(x1*m_width,0.3*m_height, m_petit_button);
    
    m_select_pilot_baudrates.clear();
    m_select_pilot_baudrates.setResize(x1*m_width,0.4*m_height, m_petit_button);
    m_select_pilot_baudrates.addValueInt("9600", 9600);
    m_select_pilot_baudrates.addValueInt("115200", 115200);
    
    
    m_select_pilot_langage.clear();
    m_select_pilot_langage.setResize(x1*m_width,0.5*m_height, m_petit_button);
    m_select_pilot_langage.addValue("arduino");
    m_select_pilot_langage.addValue("hadrien");
    
    m_button_adaptive_vitesse.setResize(x2*m_width, 0.3*m_height, m_petit_button);
    
    m_select_algo.clear();
    m_select_algo.setResize(x2*m_width,0.4*m_height, m_petit_button);
    m_select_algo.addValue("follow_carrot_avant");
    m_select_algo.addValue("follow_carrot_arriere");
    m_select_algo.addValue("rear_wheel_position");
    m_select_algo.addValue("rwp_follow_carrot_avant");
    
    m_value_gui_lookahead_d.setResize(x2*m_width, 0.5*m_height, m_petit_button, "lookahead ");
    m_value_gui_rwp_kth.setResize(x2*m_width, 0.5*m_height, m_petit_button, "kth ");
    m_value_gui_rwp_kte.setResize(x2*m_width, 0.58*m_height, m_petit_button, "kte ");
    
    m_button_auto_deactive.setResize(x1*m_width, 0.7*m_height, m_petit_button, "auto deactive ");
    m_button_auto_active.setResize(x1*m_width, 0.78*m_height, m_petit_button, "auto active ");

};

void OptionWidget::drawPage4(){
    GpsFramework & f = GpsFramework::Instance();
    drawText("Pilotage", 0.55*m_width, 0.15*m_height, sizeText_big, true);
    
    
    drawPart1(0.25*m_height, 0.35*m_height, "connection");
    m_select_pilot_baudrates.setValueInt(f.m_config.m_baudratePilot);
    m_select_pilot_serial.setValueString(f.m_config.m_inputPilot);
    m_select_pilot_langage.m_selectedValue = f.m_config.m_pilot_langage;
    m_select_algo.m_selectedValue = f.m_config.m_pilot_algo;
    
    drawSelectButtonGuiClose(m_select_pilot_langage);
    drawSelectButtonGuiClose(m_select_pilot_baudrates);
    drawSelectButtonGuiClose(m_select_pilot_serial);
    drawSelectButtonGuiClose(m_select_algo);
    
    drawPart2(0.25*m_height, 0.35*m_height, "guidage");
    if(f.m_pilot_algo == AlgoPilot::FollowCarrotPontAvant){
        drawValueGui(m_value_gui_lookahead_d, f.m_config.m_pilot_lookahead_d);
    } else if(f.m_pilot_algo == AlgoPilot::FollowCarrotPontArriere){
        drawValueGui(m_value_gui_lookahead_d, f.m_config.m_pilot_lookahead_d);
    } else if(f.m_pilot_algo == AlgoPilot::RearWheelPosition){
        drawValueGui(m_value_gui_rwp_kth, f.m_config.m_pilot_rwp_kth);
        drawValueGui(m_value_gui_rwp_kte, f.m_config.m_pilot_rwp_kte);
    }
    
    drawText("adaptative vitesse", x2bis*m_width, m_button_adaptive_vitesse.m_y, sizeText_medium, false);
    if(f.m_config.m_pilot_adaptive_vitesse > 0.5){
        drawButton(m_button_adaptive_vitesse, COLOR_CHECK);
    } else {
        drawButton(m_button_adaptive_vitesse);
    }
    
    drawPart1(0.65*m_height, 0.20*m_height, "auto_parcelle");
    drawValueGui(m_button_auto_deactive, f.m_config.m_pilot_auto_deactive);
    drawValueGui(m_button_auto_active, f.m_config.m_pilot_auto_active);

    
    drawSelectButtonGuiOpen(m_select_pilot_serial);
    drawSelectButtonGuiOpen(m_select_pilot_langage);
    drawSelectButtonGuiOpen(m_select_pilot_baudrates);
    drawSelectButtonGuiOpen(m_select_algo);
    
}

void OptionWidget::onMousePage4(int x, int y){
    GpsFramework & f = GpsFramework::Instance();
    
    if(onMouseSelectButton(m_select_pilot_serial, x, y)){
        f.m_config.m_inputPilot = m_select_pilot_serial.getValueString();
    };
    
    if(onMouseSelectButton(m_select_pilot_baudrates, x, y)){
        f.m_config.m_baudratePilot = m_select_pilot_baudrates.getValueInt();
    }
    
    if(onMouseSelectButton(m_select_pilot_langage, x, y)){
        f.m_config.m_pilot_langage = m_select_pilot_langage.m_selectedValue;
    }
    
    if(onMouseSelectButton(m_select_algo, x, y)){
        f.m_config.m_pilot_algo = m_select_algo.m_selectedValue;
    }
    
    if(m_button_adaptive_vitesse.isActive(x, y)){
        f.m_config.m_pilot_adaptive_vitesse = !f.m_config.m_pilot_adaptive_vitesse;
    }
    
    f.m_config.m_pilot_auto_deactive += 5*m_button_auto_deactive.getIntValue(x,y);
    f.m_config.m_pilot_auto_active += 5*m_button_auto_active.getIntValue(x,y);
    
    
    if(f.m_pilot_algo == AlgoPilot::FollowCarrotPontAvant){
        f.m_config.m_pilot_lookahead_d = f.m_config.m_pilot_lookahead_d * m_value_gui_lookahead_d.getMultValue(x,y);
    } else if(f.m_pilot_algo == AlgoPilot::FollowCarrotPontArriere){
        f.m_config.m_pilot_lookahead_d = f.m_config.m_pilot_lookahead_d * m_value_gui_lookahead_d.getMultValue(x,y);
    } else if(f.m_pilot_algo == AlgoPilot::RearWheelPosition){
        f.m_config.m_pilot_rwp_kth += m_value_gui_rwp_kth.getIntValue(x,y)*0.1;
        f.m_config.m_pilot_rwp_kte += m_value_gui_rwp_kte.getIntValue(x,y)*0.1;
    }
    
    f.initOrLoadConfig();
    
}


/**
 PAGE 5
 */

void OptionWidget::resizePage5(){
    m_button_motor_inverse.setResize(0.35*m_width, 0.20*m_height, m_petit_button);
    m_button_encoder_inverse.setResize(0.6*m_width, 0.20*m_height, m_petit_button);
    
    m_motor_vitesse_agressivite.setResize(0.35*m_width, 0.3*m_height, m_petit_button, "moteur agressivite ");
    m_motor_vitesse_max.setResize(0.35*m_width, 0.35*m_height, m_petit_button, "moteur vitesse max ");
    m_volant_pas_by_tour.setResize(0.35*m_width, 0.45*m_height, m_petit_button, "encoder pas par tour ");
    m_volant_angle_by_tour.setResize(0.35*m_width, 0.5*m_height, m_petit_button, "roue angle par tour de volant ");
    m_volant_derive.setResize(0.35*m_width, 0.55*m_height, m_petit_button, "rattrapage de derive ");
    
    m_volant_frequence.setResize(0.35*m_width, 0.65*m_height, m_petit_button, "frequence ");
    
};

void OptionWidget::drawPage5(){
    GpsFramework & f = GpsFramework::Instance();
    
    drawText("Autoguidage", 0.55*m_width, 0.15*m_height, sizeText_big, true);
    
    drawText(f.m_pilotModule.m_version_guidage, 0.55*m_width, 0.20*m_height, sizeText_medium, true);
    
    if(f.m_config.m_pilot_motor_inverse){
        drawButtonLabel(m_button_motor_inverse, COLOR_CHECK);
    } else {
        drawButtonLabel(m_button_motor_inverse, COLOR_OTHER);
    }
    drawText("Inv motor", 0.4*m_width, m_button_motor_inverse.m_y);
    if(f.m_config.m_pilot_encoder_inverse){
        drawButtonLabel(m_button_encoder_inverse, COLOR_CHECK);
    } else {
        drawButtonLabel(m_button_encoder_inverse, COLOR_OTHER);
    }
    drawText("Inv encoder", 0.65*m_width, m_button_motor_inverse.m_y);
    
    drawValueGui(m_motor_vitesse_agressivite, f.m_config.m_motor_vitesse_agressivite);
    drawValueGui(m_motor_vitesse_max, f.m_config.m_motor_vitesse_max);

    drawValueGui(m_volant_pas_by_tour, f.m_config.m_volant_pas_by_tour);
    drawValueGui(m_volant_angle_by_tour, f.m_config.m_volant_angle_by_tour);
    drawValueGui(m_volant_derive, f.m_config.m_volant_derive);
    
    //drawValueGui(m_volant_frequence, f.m_config.m_pilot_frequence);
    
}

void OptionWidget::onMousePage5(int x, int y){
    GpsFramework & f = GpsFramework::Instance();
    
    if(m_button_motor_inverse.isActive(x,y)){
        f.m_config.m_pilot_motor_inverse = !f.m_config.m_pilot_motor_inverse;
    }
    if(m_button_encoder_inverse.isActive(x,y)){
        f.m_config.m_pilot_encoder_inverse = !f.m_config.m_pilot_encoder_inverse;
    }
    
    f.m_config.m_motor_vitesse_agressivite *= m_motor_vitesse_agressivite.getMultValue(x,y);
    f.m_config.m_motor_vitesse_max += 2*m_motor_vitesse_max.getIntValue(x,y);
    
    f.m_config.m_volant_pas_by_tour += 100*m_volant_pas_by_tour.getIntValue(x,y);
    f.m_config.m_volant_angle_by_tour += 1*m_volant_angle_by_tour.getIntValue(x,y);
    f.m_config.m_volant_derive += 0.001*m_volant_derive.getIntValue(x,y);
    INFO(f.m_config.m_volant_derive);
    
    //f.m_config.m_pilot_frequence += 10*m_volant_angle_by_tour.getIntValue(x,y);
    
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
    
    m_button_p6test1.setResize(0.50*m_width, 0.65*m_height, m_petit_button);
    
    m_button_yawl.setResize(0.30*m_width, 0.75*m_height, m_petit_button);
    
    m_button_yawl_value.setResize(0.30*m_width, 0.85*m_height, m_petit_button, "yawl agressivite");
};

void OptionWidget::drawPage6(){
    drawText("Test Volant", 0.55*m_width, 0.15*m_height, sizeText_big, true);
    
    drawButtonLabel(m_button_p6connect, COLOR_OTHER);
    drawText("connect", m_button_p6connect.m_x, m_button_p6connect.m_y, sizeText_little, true);
    
    drawButtonLabel(m_button_p6disable, COLOR_OTHER);
    drawText("disconnect", m_button_p6disable.m_x, m_button_p6disable.m_y, sizeText_little, true);
    
    drawText("Goto Volant", 0.55*m_width, 0.50*m_height, sizeText_medium, true);
    drawButtonLabel(m_button_p6testGoToVLeft, COLOR_OTHER);
    drawText("-0.5 volant", m_button_p6testGoToVLeft.m_x, m_button_p6testGoToVLeft.m_y, sizeText_little, true);
    
    drawButtonLabel(m_button_p6testGoToV0, COLOR_OTHER);
    drawText("0", m_button_p6testGoToV0.m_x, m_button_p6testGoToV0.m_y, sizeText_little, true);
    
    drawButtonLabel(m_button_p6testGoToVRight, COLOR_OTHER);
    drawText("+0.5 volant", m_button_p6testGoToVRight.m_x, m_button_p6testGoToVRight.m_y, sizeText_little, true);

    drawButtonLabel(m_button_p6test1, COLOR_OTHER);
    drawText("button", m_button_p6test1.m_x, m_button_p6test1.m_y, sizeText_little, true);
    
    GpsFramework & f = GpsFramework::Instance();
    if(f.m_config.m_algo_volant_mode == AlgoVolantMode_CapteurVitesse){
        drawButtonLabel(m_button_yawl, COLOR_OTHER);
    } else {
        drawButtonLabel(m_button_yawl, COLOR_CHECK);
    }
    
    drawValueGui(m_button_yawl_value, f.m_config.m_agressivite_yawl);
}

void OptionWidget::onMousePage6(int x, int y){
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
    } else if(m_button_p6test1.isActive(x,y)){
        f.m_pilotModule.test();
    }
    
    if(m_button_yawl.isActive(x, y)){
        if(f.m_config.m_algo_volant_mode == AlgoVolantMode_CapteurVitesse){
            f.m_config.m_algo_volant_mode = AlgoVolantMode_Encodeur;
        } else {
            f.m_config.m_algo_volant_mode = AlgoVolantMode_Encodeur;
        }
    }
    
    f.m_config.m_agressivite_yawl *= m_button_yawl_value.getMultValue(x,y);
    
    f.initOrLoadConfig();
}


/**
 PAGE 3
 */

void OptionWidget::resizePage3(){
    m_select_imu_serial.setResize(x1*m_width,0.3*m_height, m_petit_button);
    
    m_select_imu_baudrates.clear();
    m_select_imu_baudrates.setResize(x1*m_width,0.4*m_height, m_petit_button);
    m_select_imu_baudrates.addValueInt("9600", 9600);
    m_select_imu_baudrates.addValueInt("115200", 115200);
     
    m_lissage_imu.setResize(x2*m_width, 0.3*m_height, m_petit_button);
    m_lissage_imu.clear();
    m_lissage_imu.addValue("none");
    m_lissage_imu.addValue("ekf");
    m_lissage_imu_ekf.setResize(x2*m_width, 0.4*m_height, m_petit_button, "ekf_lissage ");
    
    m_correction_lateral_imu.setResize(x2*m_width, 0.6*m_height, m_petit_button);
}

void OptionWidget::drawPage3(){
    drawText("IMU", 0.55*m_width, 0.15*m_height, sizeText_big, true);
    
    GpsFramework & f = GpsFramework::Instance();
    
    drawPart1(0.25*m_height, 0.25*m_height, "connection");
    m_select_gps_baudrates.setValueInt(f.m_config.m_imu_baudrate);
    m_select_gps_serial.setValueString(f.m_config.m_imu_input);
    
    drawSelectButtonGuiClose(m_select_imu_serial);
    drawSelectButtonGuiClose(m_select_imu_baudrates);
    drawSelectButtonGuiClose(m_lissage_imu);
    
    drawPart2(0.25*m_height, 0.25*m_height, "lissage Imu");
    m_lissage_imu.m_selectedValue = f.m_config.m_lissage_imu_mode;
    if(f.m_config.m_lissage_imu_mode == (int)LissageImuMode_Ekf){
        drawValueGui(m_lissage_imu_ekf, f.m_config.m_lissage_imu_ekf*100);
    }
    
    drawText("correction de devers", x2bis*m_width, m_correction_lateral_imu.m_y, sizeText_medium, false);
    if(f.m_config.m_imu_correction_devers){
        drawButton(m_correction_lateral_imu, COLOR_CHECK);
    } else {
        drawButton(m_correction_lateral_imu);
    }
    
    drawSelectButtonGuiOpen(m_select_imu_serial);
    drawSelectButtonGuiOpen(m_select_imu_baudrates);
    drawSelectButtonGuiOpen(m_lissage_imu);
    
}

void OptionWidget::onMousePage3(int x, int y){
    GpsFramework & f = GpsFramework::Instance();
    
    if(onMouseSelectButton(m_select_imu_serial, x, y)){
        f.m_config.m_imu_input = m_select_imu_serial.getValueString();
    };
    
    if (m_correction_lateral_imu.isActive(x, y)){
       f.m_config.m_imu_correction_devers = !f.m_config.m_imu_correction_devers;
    }
    
    if(onMouseSelectButton(m_select_imu_baudrates, x, y)){
        f.m_config.m_imu_baudrate = m_select_imu_baudrates.getValueInt();
    }
    
    
    if(onMouseSelectButton(m_lissage_imu, x, y)){
        f.m_config.m_lissage_imu_mode = m_lissage_imu.m_selectedValue;
    }
    if(f.m_config.m_lissage_imu_mode == (int)LissageImuMode_Ekf){
        f.m_config.m_lissage_imu_ekf += 0.05*m_lissage_imu_ekf.getIntValue(x,y);
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
    m_select_imu_serial.clear();
    for(auto serial: serials){
        m_select_gps_serial.addValue(serial);
        
        if(serial != "file"){
            m_select_pilot_serial.addValue(serial);
            m_select_imu_serial.addValue(serial);
        }
    }
}
