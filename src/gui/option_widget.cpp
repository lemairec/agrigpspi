#include "option_widget.hpp"

#include "../gps_framework.hpp"
#include "gps_widget.hpp"
#include "environnement.hpp"
#include "qt/main_window.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/algorithm/string.hpp>

#define PETIT_RAYON2 0.025

std::string execute2(std::string cmd){
    std::string file = ProjectSourceBin + "/tmp_cmd";
    std::string cmd2 = cmd + " > " + file;
    system(cmd2.c_str());
    std::ifstream infile(file);
    std::stringstream strStream;
    strStream << infile.rdbuf();//read the file
    std::string res = strStream.str();
    return res;
}

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
    
    //m_button_gps = new ButtonGui(0.25, 0.3, GROS_BUTTON, 0);
    //m_button_line = new ButtonGui(0.5, 0.3, GROS_BUTTON, 0);
    
    //m_close=false;
    //m_page =5;
    //addSerials();
}

void OptionWidget::draw(){
    
    
    
    scene->addRect(m_width*0.2, m_height*0.1, m_width*0.6, m_height*0.8, m_penBlack, m_brushWhite);
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

void OptionWidget::drawPage2(){
    drawText("Connection GPS", 0.55*m_width, 0.15*m_height, 20, true);
    
    GpsFramework & f = GpsFramework::Instance();
    drawText("Port :", 0.3*m_width, y_port_title*m_height, 20, false);
    for(auto button: m_buttonp2Serials){
        if(button->m_label == f.m_config.m_input){
            drawButton(button, COLOR_CHECK);
        } else {
            drawButton(button, COLOR_OTHER);
        }
        drawText(button->m_label, button->m_x+0.04, button->m_y);
    }
    
    for(auto button: m_buttonp2Baurates){
        if(button->m_labelInt == f.m_config.m_baudrate){
            drawButton(button, COLOR_CHECK);
        } else {
            drawButton(button, COLOR_OTHER);
        }
        drawText(button->m_label, button->m_x+0.04, button->m_y);
    }
    
}

/**
PAGE 3
*/

void OptionWidget::setPage3(){
};

void OptionWidget::drawPage3(){
    GpsFramework & f = GpsFramework::Instance();
    drawText("Connection Pilot", 0.55*m_width, 0.15*m_height, 20, true);
    
    drawText("Port :", 0.3*m_width, y_port_title*m_height, 20, false);
    for(auto button: m_buttonp3Serials){
        if(button->m_label == f.m_config.m_inputPilot){
            drawButton(button, COLOR_CHECK);
        } else {
            drawButton(button, COLOR_OTHER);
        }
        drawText(button->m_label, button->m_x+0.04, button->m_y);
        
    }
    
    drawText("Baudrate :", 0.3*m_width, y_baurates_title*m_height, 20, false);
    for(auto button: m_buttonp3Baurates){
        if(button->m_labelInt == f.m_config.m_baudratePilot){
            drawButton(button, COLOR_CHECK);
        } else {
            drawButton(button, COLOR_OTHER);
        }
        drawText(button->m_label, button->m_x+0.04, button->m_y);
    }
    }

void OptionWidget::onMousePage3(double x, double y){
    GpsFramework & f = GpsFramework::Instance();
    
    for(auto button: m_buttonp3Serials){
        if(button->isActive(x,y)){
            f.m_config.m_inputPilot = button->m_label;
            f.initOrLoadConfig();
        }
    }
    for(auto button: m_buttonp3Baurates){
        if(button->isActive(x,y)){
            GpsFramework & f = GpsFramework::Instance();
            f.m_config.m_baudratePilot = button->m_labelInt;
            f.initOrLoadConfig();
        }
    }

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
    m_button_select_algo = new SelectButtonGui(0.35, 0.30, PETIT_RAYON2);
    m_button_select_algo->addValue("algo_naif");
    m_button_select_algo->addValue("algo_follow_karott");

    m_button_algo_fk_lookahead_d = new ValueGui(0.4, 0.35, PETIT_RAYON2, 0, "fk lookahead ");
    m_button_algo_naif_k = new ValueGui(0.4, 0.35, PETIT_RAYON2, 0, "naif k");
    
    m_button_select_algo2 = new SelectButtonGui(0.35, 0.50, PETIT_RAYON2);
    m_button_select_algo2->addValue("algo_goto");
    m_button_select_algo2->addValue("algo_goto_rel");
    m_button_select_algo2->addValue("algo_pid");
    
    m_button_algo2_goto_k = new ValueGui(0.4, 0.55, PETIT_RAYON2, 0, "goto k ");
    m_button_algo2_goto_rel_s = new ValueGui(0.4, 0.60, PETIT_RAYON2, 0, "goto rel ");
    m_button_algo2_pid_kp = new ValueGui(0.4, 0.55, PETIT_RAYON2, 0, "pid p ");
    m_button_algo2_pid_kd = new ValueGui(0.4, 0.60, PETIT_RAYON2, 0, "pid d ");
    
    
    m_button_p5test1  = new ButtonGui(0.4, 0.8, PETIT_RAYON2, 0);
    m_button_p5test2  = new ButtonGui(0.5, 0.8, PETIT_RAYON2, 0);
    m_button_p5test3  = new ButtonGui(0.6, 0.8, PETIT_RAYON2, 0);
    
    m_button_p5connect  = new ButtonGui(0.4, 0.7, PETIT_RAYON2, 0);
    m_button_p5disable  = new ButtonGui(0.5, 0.7, PETIT_RAYON2, 0);
    m_button_p5clearError  = new ButtonGui(0.6, 0.7, PETIT_RAYON2, 0);
};


void OptionWidget::drawPage5(){
    GpsFramework & f = GpsFramework::Instance();
    
    drawText("Autoguidage", 0.55*m_width, 0.1*m_height, 20, true);
    
    m_button_select_algo->selectedValue = f.m_config.m_algo;
    m_button_select_algo2->selectedValue = f.m_config.m_algo2;
    
    drawText(f.m_pilotModule.m_version_guidage, 0.55*m_width, 0.20*m_height, 11, true);
    
    if(f.m_config.m_pilot_inverse){
        drawButtonLabel(m_button_inverse, COLOR_CHECK);
    } else {
        drawButtonLabel(m_button_inverse, COLOR_OTHER);
    }
    drawText("Inverse", 0.4, m_button_inverse->m_y);
    
    
    drawSelectButtonGuiClose(m_button_select_algo);
    if(f.m_config.m_algo == ALGO_FOLLOW_CARROT){
        drawValueGui(m_button_algo_fk_lookahead_d, f.m_config.m_algofk_lookahead_d);
    } else {
        drawValueGui(m_button_algo_naif_k, f.m_config.m_algo_naif_k);
    }
    
    if(f.m_config.m_algo2 == ALGO2_GOTO){
        drawValueGui(m_button_algo2_goto_k, f.m_config.m_algo2_goto_k);
    } else if(f.m_config.m_algo2 == ALGO2_GOTO_REL){
        drawValueGui(m_button_algo2_goto_k, f.m_config.m_algo2_goto_k);
        drawValueGui(m_button_algo2_goto_rel_s, f.m_config.m_algo2_goto_rel_s);
    } else {
        drawValueGui(m_button_algo2_pid_kp, f.m_config.m_algo2_pid_p);
        drawValueGui(m_button_algo2_pid_kd, f.m_config.m_algo2_pid_d);
    }
    
    drawSelectButtonGuiClose(m_button_select_algo2);
    
      
    
    drawButtonLabel(m_button_p5connect, COLOR_OTHER);
    drawButtonLabel(m_button_p5clearError, COLOR_OTHER);
    drawButtonLabel(m_button_p5disable, COLOR_OTHER);
    
    drawButtonLabel(m_button_p5test1, COLOR_GREEN);
    drawButtonLabel(m_button_p5test2, COLOR_OTHER);
    drawButtonLabel(m_button_p5test3, COLOR_GREEN);
    
    drawSelectButtonGuiOpen(m_button_select_algo);
    drawSelectButtonGuiOpen(m_button_select_algo2);
}

void OptionWidget::onMousePage5(double x, double y){
    GpsFramework & f = GpsFramework::Instance();
    
    if(m_button_select_algo->m_open){
        for(size_t i = 0; i < m_button_select_algo->m_buttons.size(); ++i){
            if(m_button_select_algo->m_buttons[i]->isActive(x, y)){
                m_button_select_algo->selectedValue = i;
                f.m_config.m_algo = i;
                f.initOrLoadConfig();
            };
        }
        m_button_select_algo->m_open = false;
        return;
        
    }
    if( m_button_select_algo->m_buttonOpen.isActive(x, y)){
        m_button_select_algo->m_open = !m_button_select_algo->m_open;
    }
    
    if(m_button_select_algo2->m_open){
        for(size_t i = 0; i < m_button_select_algo2->m_buttons.size(); ++i){
            if(m_button_select_algo2->m_buttons[i]->isActive(x, y)){
                m_button_select_algo2->selectedValue = i;
                f.m_config.m_algo2 = i;
                f.initOrLoadConfig();
            };
        }
        m_button_select_algo2->m_open = false;
        return;
        
    }
    if( m_button_select_algo2->m_buttonOpen.isActive(x, y)){
        m_button_select_algo2->m_open = !m_button_select_algo2->m_open;
    }
    
    
    
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
        f.m_pilotModule.run(0);
        return;
    } else if(m_button_p5disable->isActive(x,y)){
        f.m_pilotModule.run(1);
        return;
    } else if(m_button_p5clearError->isActive(x,y)){
        f.m_pilotModule.test(2);
        return;
    }
    
    if(m_button_inverse->isActive(x,y)){
        f.m_config.m_pilot_inverse = !f.m_config.m_pilot_inverse;
    }
    
    if(f.m_config.m_algo == ALGO_FOLLOW_CARROT){
        f.m_config.m_algofk_lookahead_d = f.m_config.m_algofk_lookahead_d * m_button_algo_fk_lookahead_d->getMultValue(x,y);
    } else {
        f.m_config.m_algo_naif_k = f.m_config.m_algo_naif_k * m_button_algo_naif_k->getMultValue(x,y);
    }
    
    if(f.m_config.m_algo2 == ALGO2_GOTO){
        f.m_config.m_algo2_goto_k = f.m_config.m_algo2_goto_k * m_button_algo2_goto_k->getMultValue(x,y);
    } else if(f.m_config.m_algo2 == ALGO2_GOTO_REL){
        f.m_config.m_algo2_goto_k = f.m_config.m_algo2_goto_k * m_button_algo2_goto_k->getMultValue(x,y);
        f.m_config.m_algo2_goto_rel_s = f.m_config.m_algo2_goto_rel_s + 10*m_button_algo2_goto_rel_s->isActive(x,y);
    } else {
        f.m_config.m_algo2_pid_p = f.m_config.m_algo2_pid_p * m_button_algo2_pid_kp->getMultValue(x,y);
        f.m_config.m_algo2_pid_d = f.m_config.m_algo2_pid_d * m_button_algo2_pid_kd->getMultValue(x,y);
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
    } else {
        if(m_page == 1){
            onMousePage1(x, y);
        } else if(m_page == 2){
            onMousePage2(x, y);
        } else if(m_page == 3){
            onMousePage3(x, y);
        } else if(m_page == 4){
            onMousePage4(x, y);
        } else {
           onMousePage5(x, y);
       }
    }
    
   
    /*if(id == 100){
        MainWindow::Instance_ptr()->showMinimized();
        std::string cmd = "cd " + ProjectSourceDir + "; git pull";
        run(cmd);
    }
    
    if(id == 101){
        MainWindow::Instance_ptr()->showMinimized();
        std::string cmd = "rm -rf " + ProjectSourceBin + ";";
        run(cmd);
    }*/
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

void OptionWidget::onMousePage2(double x, double y){

    for(auto button: m_buttonp2Serials){
        if(button->isActive(x,y)){
            GpsFramework & f = GpsFramework::Instance();
            f.m_config.m_input = button->m_label;
            f.initOrLoadConfig();
        }
    }
    
    for(auto button: m_buttonp2Baurates){
        if(button->isActive(x,y)){
            GpsFramework & f = GpsFramework::Instance();
            f.m_config.m_baudrate = button->m_labelInt;
            f.initOrLoadConfig();
        }
    }
}



void OptionWidget::open(){
    addSerials();
}

void OptionWidget::addSerials(){
    std::list<std::string> m_serials;
    m_serials.clear();
    m_serials.push_back("file");
    m_serials.push_back("none");
    
    {
        std::string res = execute2("ls /dev/cu.*");
        std::vector<std::string> strs;
        boost::split(strs, res, boost::is_any_of("\n"));
        for(auto s : strs){
            if(!s.empty()){
                m_serials.push_back(s);
            }
        }
    }
    {
        std::string res = execute2("ls /dev/ttyACM*");
        std::vector<std::string> strs;
        boost::split(strs, res, boost::is_any_of("\n"));
        for(auto s : strs){
            if(!s.empty()){
                INFO(s);
                m_serials.push_back(s);
            }
        }
    }
    {
        std::string res = execute2("ls /dev/ttyUSB*");
        std::vector<std::string> strs;
        boost::split(strs, res, boost::is_any_of("\n"));
        for(auto s : strs){
            if(!s.empty()){
                INFO(s);
                m_serials.push_back(s);
            }
        }
    }
    
    m_buttonp2Serials.clear();
    m_buttonp3Serials.clear();
    int i=2;
    y_port_title = 0.2;
    for(auto serial: m_serials){
        auto button = new ButtonGui(0.3, y_port_title+i*0.05, 0.02, 0);
        button->m_label = serial;
        m_buttonp2Serials.push_back(button);
        
        if(serial != "file"){
            auto button2 = new ButtonGui(0.3, y_port_title+i*0.05, 0.02, 0);
            button2->m_label = serial;
            m_buttonp3Serials.push_back(button2);
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
        m_buttonp3Baurates.push_back(button);
        m_buttonp2Baurates.push_back(button);
        ++i;
    }
}


