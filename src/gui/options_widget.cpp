#include "options_widget.hpp"
#include "../gps_framework.hpp"
#include "gps_widget.hpp"
#include "environnement.hpp"


#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/algorithm/string.hpp>

void BaseWidget::onMouse(int x, int y){
    for(auto button : m_buttons){
        if(x > button.m_x - 20  && x < button.m_x + 20 && y > button.m_y - 20  && y < button.m_y + 20){
            onButton(button);
        }
    }
}

void BaseWidget::onButton(const Button & button){
}


void BaseWidget::setSize(int width, int height){
    m_width = width;
    m_height = height;
    
    m_buttons.clear();
    
    m_buttons.push_front(Button(m_width*3/4-30,80,0));
    
    m_buttons.push_front(Button(m_width*3/8 + 22, 160,11));
    m_buttons.push_front(Button(m_width*3/8 - 22 , 160,12));
    m_buttons.push_front(Button(m_width/2 + 22, 160,21));
    m_buttons.push_front(Button(m_width/2 - 22, 160,22));
    m_buttons.push_front(Button(m_width*5/8 + 22, 160,31));
    m_buttons.push_front(Button(m_width*5/8 - 22 , 160,32));
    
    m_buttons.push_front(Button(m_width*3/8 - 30 , m_height-50,100));
}

void BaseWidget::drawText(QGraphicsScene * scene, const std::string & text, int x, int y, int size, bool center){
    QString s = QString::fromStdString(text);
    auto textItem = scene->addText(QString(s));
    textItem->setFont(QFont("Latin", size, 1, false));
    if(center){
        auto mBounds = textItem->boundingRect();
        textItem->setPos(x-mBounds.width()/2, y);
    } else {
         textItem->setPos(x, y);
    }
    
}

/**
 * OPTION WIDGET
 **/

std::string execute(std::string cmd){
    std::string file = ProjectSourceBin + "/tmp_cmd";
    std::string cmd2 = cmd + " > " + file;
    system(cmd2.c_str());
    std::ifstream infile(file);
    std::stringstream strStream;
    strStream << infile.rdbuf();//read the file
    std::string res = strStream.str();
    return res;
}


OptionsWidget::OptionsWidget(GpsWidget * gpsWidget){
    m_gpsWidget = gpsWidget;
    
}

void OptionsWidget::open(){
    m_close = false;
    addSerials();
    setSize(m_width, m_height);
    int i = 0;
    m_serials.push_front("file");
    m_serials.push_front("none");
    for(auto serial: m_serials){
        auto b = Button(m_width*3/8 + 30, 250 + i*40,1001+i);
        b.m_desc = serial;
        m_buttons.push_back(b);
        ++i;
    }
}

void OptionsWidget::draw(QGraphicsScene * scene){
    scene->addRect(m_width/4, 50, m_width/2, m_height-60, QPen(QColor(0,0,0)), QBrush(QColor(200,200,200)));
    GpsFramework & f = GpsFramework::Instance();
    QString s = "largueur " + QString::number(f.m_config.m_largeur, 'f', 2) + " m";
    drawText(scene, s.toUtf8().constData(), m_width/2, 100, 30, true);
    
    for(auto button : m_buttons){
        if(button.m_id == 11 || button.m_id == 21 || button.m_id == 31){
            scene->addEllipse(button.m_x-20, button.m_y-20, 40, 40, QPen(QColor(0,0,0)), QBrush(QColor(0, 200, 0)));
            std::string s;
            if(button.m_id == 11){
                s = "+ 0.1";
            } else if(button.m_id == 21){
                s = "+ 1";
            } else {
                s = "+ 5";;
            }
            drawText(scene, s, button.m_x, button.m_y-12, 15, true);
        }
        
        if(button.m_id == 12 || button.m_id == 22 || button.m_id == 32){
            scene->addEllipse(button.m_x-20, button.m_y-20, 40, 40, QPen(QColor(0,0,0)), QBrush(QColor(200, 0, 0)));
            std::string s;
            if(button.m_id == 12){
                s = "- 0.1";
            } else if(button.m_id == 22){
                s = "- 1";
            } else {
                s = "- 5";;
            }
            drawText(scene, s, button.m_x, button.m_y-12, 15, true);
        }
        
        if(button.m_id == 0){
            scene->addEllipse(button.m_x-20, button.m_y-20, 40, 40, QPen(QColor(0,0,0)), QBrush(QColor(200, 0, 0)));
            drawText(scene, "X", button.m_x, button.m_y-12, 15, true);
        }
        
        if(button.m_id == 100){
            scene->addEllipse(button.m_x-20, button.m_y-20, 40, 40, QPen(QColor(0,0,0)), QBrush(QColor(100, 100, 100)));
            drawText(scene, "mettre à jour", button.m_x+80, button.m_y-12, 15, true);
        }
        
        if(button.m_id > 1000){
            scene->addEllipse(button.m_x-15, button.m_y-15, 30, 30, QPen(QColor(0,0,0)), QBrush(QColor(150, 150, 150)));
            drawText(scene, button.m_desc, button.m_x+20, button.m_y-15, 20, false);
            if(button.m_desc == f.m_config.m_input){
                scene->addEllipse(button.m_x-10, button.m_y-10, 20, 20, QPen(QColor(0,0,0)), QBrush(QColor(0, 0, 0)));
            }
        }
    }
    
    
    drawText(scene, "entrée", m_width/2, 200, 30, true);
    
}

void OptionsWidget::onButton(const Button & button){
    int id = button.m_id;
    if(id == 0){
        m_close = true;
    } else if(id == 11){
        GpsFramework & f = GpsFramework::Instance();
        f.m_config.m_largeur += 0.1;
        f.initOrLoadConfig();
    } else if(id == 12){
        GpsFramework & f = GpsFramework::Instance();
        f.m_config.m_largeur -= 0.1;
        f.initOrLoadConfig();
        
    } else if(id == 21){
        GpsFramework & f = GpsFramework::Instance();
        f.m_config.m_largeur += 1;
        f.initOrLoadConfig();
        
    } else if(id == 22){
        GpsFramework & f = GpsFramework::Instance();
        f.m_config.m_largeur -= 1;
        f.initOrLoadConfig();
    } else if(id == 31){
        GpsFramework & f = GpsFramework::Instance();
        f.m_config.m_largeur += 5;
        f.initOrLoadConfig();
        
    } else if(id == 32){
        GpsFramework & f = GpsFramework::Instance();
        f.m_config.m_largeur -= 5;
        f.initOrLoadConfig();
    }
    
    if(id>1000){
        GpsFramework & f = GpsFramework::Instance();
        f.m_config.m_input = button.m_desc;
        f.initOrLoadConfig();
    }
    
    if(id == 100){
        std::string cmd = "cd " + ProjectSourceDir + "; git pull";
        INFO(cmd);
        //std::string res = execute(cmd);
        //INFO(res);
        exit(0);
    }
    m_gpsWidget->onValueChangeSlot(true);
};

void OptionsWidget::addSerials(){
    m_serials.clear();
    {
        std::string res = execute("ls /dev/cu.*");
        std::vector<std::string> strs;
        boost::split(strs, res, boost::is_any_of("\n"));
        for(auto s : strs){
            if(!s.empty()){
                m_serials.push_back(s);
            }
        }
    }
    {
        std::string res = execute("ls /dev/ttyACM*");
        std::vector<std::string> strs;
        boost::split(strs, res, boost::is_any_of("\n"));
        for(auto s : strs){
            if(!s.empty()){
                INFO(s);
                m_serials.push_back(s);
            }
        }
    }
}
