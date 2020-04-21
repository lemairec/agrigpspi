#include "option_widget.hpp"

#include "../gps_framework.hpp"
#include "gps_widget.hpp"
#include "environnement.hpp"
#include "qt/main_window.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/algorithm/string.hpp>

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
    m_button_close = new ButtonGui(0.75, 0.15, GROS_BUTTON, 0);
    
    m_button_p1  = new ButtonGui(0.3, 0.15, GROS_BUTTON, 0);
    m_button_p2  = new ButtonGui(0.4, 0.15, GROS_BUTTON, 0);
    m_button_p3  = new ButtonGui(0.5, 0.15, GROS_BUTTON, 0);
    
    
    //page 1
    double dist = 0.02;
    double rayon = PETIT_RAYON;
    m_button_p1add1 = new ButtonGui(0.4+dist, 0.3, rayon, 0);
    m_button_p1add1->m_label="+";
    m_button_p1minus1 = new ButtonGui(0.4-dist, 0.3, rayon, 0);
    
    m_button_p1add2 = new ButtonGui(0.5+dist, 0.3, rayon, 0);
    m_button_p1add2->m_label="+";
    m_button_p1minus2 = new ButtonGui(0.5-dist, 0.3, rayon, 0);
    
    m_button_p1add3 = new ButtonGui(0.6+dist, 0.3, rayon, 0);
    m_button_p1add3->m_label="+";
    m_button_p1minus3 = new ButtonGui(0.6-dist, 0.3, rayon, 0);
    
    m_button_sens = new ButtonGui(0.3, 0.5, rayon, 0);
    m_button_debug = new ButtonGui(0.4, 0.5, rayon, 0);
    
    
    //page 3
    m_kp = new ValueGui(0.3, 0.3, PETIT_RAYON*0.5, 0, "kp ");
    m_ki = new ValueGui(0.3, 0.35, PETIT_RAYON*0.5, 0, "ki ");
    m_kd = new ValueGui(0.3, 0.4, PETIT_RAYON*0.5, 0, "kd ");
    m_kvitesse = new ValueGui(0.3, 0.45, PETIT_RAYON*0.5, 0, "kvitesse ");
    
    m_button_p3test1  = new ButtonGui(0.4, 0.8, rayon, 0);
    m_button_p3test2  = new ButtonGui(0.5, 0.8, rayon, 0);
    m_button_p3test3  = new ButtonGui(0.6, 0.8, rayon, 0);
    
    //m_button_gps = new ButtonGui(0.25, 0.3, GROS_BUTTON, 0);
    //m_button_line = new ButtonGui(0.5, 0.3, GROS_BUTTON, 0);
    
    //m_close=false;
    //addSerials();
}

void OptionWidget::draw(){
    
    scene->addRect(m_width*0.2, m_height*0.1, m_width*0.6, m_height*0.8, QPen(QColor(0,0,0)), QBrush(QColor(200,200,200)));
    
    scene->addRect(m_width*0.25, m_height*0.2, m_width*0.5, m_height*0.65, QPen(QColor(0,0,0)), QBrush(QColor(200,200,200)));
    
    drawButtonLabel(m_button_close, COLOR_RED);
    
    if(m_page == 1){
        drawButtonLabel(m_button_p1, COLOR_CHECK);
        drawButtonLabel(m_button_p2, COLOR_OTHER);
        drawButtonLabel(m_button_p3, COLOR_OTHER);
        drawPage1();
    } else if(m_page == 2){
        drawButtonLabel(m_button_p1, COLOR_OTHER);
        drawButtonLabel(m_button_p2, COLOR_CHECK);
        drawButtonLabel(m_button_p3, COLOR_OTHER);
        drawPage2();
    } else if(m_page == 3){
        drawButtonLabel(m_button_p1, COLOR_OTHER);
        drawButtonLabel(m_button_p2, COLOR_OTHER);
        drawButtonLabel(m_button_p3, COLOR_CHECK);
        drawPage3();
    }
     
    
    
}

void OptionWidget::drawPage1(){
    GpsFramework & f = GpsFramework::Instance();
    QString s = "largueur " + QString::number(f.m_config.m_largeur, 'f', 2) + " m";
    drawText(s.toUtf8().constData(), 0.5*m_width, 0.2*m_height, 20, true);
    
    drawButtonLabel(m_button_p1add1, COLOR_GREEN);
    drawButtonLabel(m_button_p1minus1, COLOR_RED);
    drawButtonLabel(m_button_p1add2, COLOR_GREEN);
    drawButtonLabel(m_button_p1minus2, COLOR_RED);
    drawButtonLabel(m_button_p1add3, COLOR_GREEN);
    drawButtonLabel(m_button_p1minus3, COLOR_RED);
    
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

void OptionWidget::drawPage2(){
    drawText("GPS", 0.5*m_width, 0.2*m_height, 20, true);
    
    GpsFramework & f = GpsFramework::Instance();
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

void OptionWidget::drawPage3(){
    GpsFramework & f = GpsFramework::Instance();
    drawText("Pilot", 0.5*m_width, 0.2*m_height, 20, true);
    
    drawButtonLabel(m_button_p3test1, COLOR_GREEN);
    drawButtonLabel(m_button_p3test2, COLOR_RED);
    drawButtonLabel(m_button_p3test3, COLOR_GREEN);
    
    drawValueGui(m_kp, f.m_config.m_kp);
    drawValueGui(m_ki, f.m_config.m_ki);
    drawValueGui(m_kd, f.m_config.m_kd);
    drawValueGui(m_kvitesse, f.m_config.m_kvitesse);
    
    for(auto button: m_buttonp3Serials){
        if(button->m_label == f.m_config.m_inputPilot){
            drawButton(button, COLOR_CHECK);
        } else {
            drawButton(button, COLOR_OTHER);
        }
        drawText(button->m_label, button->m_x+0.04, button->m_y);
        
    }
    
    for(auto button: m_buttonp3Baurates){
        if(button->m_labelInt == f.m_config.m_baudratePilot){
            drawButton(button, COLOR_CHECK);
        } else {
            drawButton(button, COLOR_OTHER);
        }
        drawText(button->m_label, button->m_x+0.04, button->m_y);
    }
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
    } else {
        if(m_page == 1){
            onMousePage1(x, y);
        } else if(m_page == 2){
            onMousePage2(x, y);
        } else {
           onMousePage3(x, y);
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
    } else if(m_button_p1add3->isActive(x,y)){
        GpsFramework & f = GpsFramework::Instance();
        f.m_config.m_largeur += 5;
        f.initOrLoadConfig();
        
    } else if(m_button_p1minus3->isActive(x,y)){
        GpsFramework & f = GpsFramework::Instance();
        f.m_config.m_largeur -= 5;
        f.initOrLoadConfig();
    } else if(m_button_sens->isActive(x,y)){
        GpsFramework & f = GpsFramework::Instance();
        f.m_config.m_sensDraw = !f.m_config.m_sensDraw;
        f.initOrLoadConfig();
    } else if(m_button_debug->isActive(x,y)){
        GpsFramework & f = GpsFramework::Instance();
        f.m_config.m_debug = !f.m_config.m_debug;
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
    if(m_button_p3test1->isActive(x,y)){
        f.m_pilotModule.test(-10);
    } else if(m_button_p3test2->isActive(x,y)){
        f.m_pilotModule.test(0);
        
    } else if(m_button_p3test3->isActive(x,y)){
       f.m_pilotModule.test(10);
       
   }
    
    INFO(m_kp->getMultValue(x,y));
    f.m_config.m_kp = f.m_config.m_kp * m_kp->getMultValue(x,y);
    f.m_config.m_ki = f.m_config.m_ki *m_ki->getMultValue(x,y);
    f.m_config.m_kd *= m_kd->getMultValue(x,y);
    f.m_config.m_kvitesse *= m_kvitesse->getMultValue(x,y);
    f.initOrLoadConfig();

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
    
    m_buttonp2Serials.clear();
    m_buttonp3Serials.clear();
    int i=0;
    for(auto serial: m_serials){
        auto button = new ButtonGui(0.3, 0.45+i*0.05, 0.02, 0);
        button->m_label = serial;
        m_buttonp2Serials.push_back(button);
        
        if(serial != "file"){
            auto button2 = new ButtonGui(0.3, 0.45+i*0.05, 0.02, 0);
            button2->m_label = serial;
            m_buttonp3Serials.push_back(button2);
        }
        
        ++i;
    }
    
    std::list<int> baurates;
    
    baurates.push_back(9600);
    baurates.push_back(115200);
    for(auto baurate: baurates){
        auto button = new ButtonGui(0.3, 0.45+i*0.05, 0.02, 0);
        button->m_label = std::to_string(baurate);
        button->m_labelInt = baurate;
        m_buttonp3Baurates.push_back(button);
        m_buttonp2Baurates.push_back(button);
        ++i;
    }
}


