#include "base_widget.hpp"
#include "environnement.hpp"
#include <iostream>
#include <sstream>


/*void BaseWidget::onMouse(int x, int y){
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
}

void BaseWidget::drawText(const std::string & text, int x, int y, int size, bool center){
    QString s = QString::fromStdString(text);
    auto textItem = scene->addText(QString(s));
    textItem->setFont(QFont("Latin", size, 1, false));
    if(center){
        auto mBounds = textItem->boundingRect();
        textItem->setPos(x-mBounds.width()/2, y);
    } else {
        textItem->setPos(x, y);
    }
    
}*/




/**
 
 
 NEW!
 
 
 
 */


ButtonGui::ButtonGui(double x, double y, double rayon, int type)
:m_x(x), m_y(y), m_rayon(rayon), m_type(type)
{
}

bool ButtonGui::isActive(double x, double y){
    
    if(x > this->m_x-this->m_rayon  && x < this->m_x+this->m_rayon && y > this->m_y-this->m_rayon  && y < this->m_y+this->m_rayon){
        return true;
    }
    return false;
}


ValueGui::ValueGui(double x, double y, double rayon, int type, std::string label)
:m_x(x), m_y(y), m_rayon(rayon), m_label(label), m_buttonAdd(x+1.5*rayon, y, rayon, 0), m_buttonMinus(x, y, rayon, 0)
{
    m_buttonAdd.m_label = "+";
    m_buttonMinus.m_label = "-";
}

int ValueGui::isActive(double x, double y){
    if(m_buttonAdd.isActive(x, y)){
        return 1;
    }
    if(m_buttonMinus.isActive(x, y)){
        return -1;
    }
    return 0;
}

double ValueGui::getMultValue(double x, double y){
    if(m_buttonAdd.isActive(x, y)){
        return 1.1;
    }
    if(m_buttonMinus.isActive(x, y)){
        return 1.0/1.1;
    }
    return 1.0;
}

double ValueGui::getIntValue(double x, double y){
    if(m_buttonAdd.isActive(x, y)){
        return 1;
    }
    if(m_buttonMinus.isActive(x, y)){
        return -1;
    }
    return 0;
}

void BaseWidget::setSize(int width, int height){
    m_width = width;
    m_height = height;
}

void BaseWidget::onMouseInt(double x, double y){
    this->onMouse(x/m_width, y/m_height);
}



void BaseWidget::drawText(const std::string & text, double x, double y){
    int size = 20;
    QString s = QString::fromStdString(text);
    auto textItem = scene->addText(QString(s));
    textItem->setFont(QFont("Latin", size, 1, false));
    //if(center){
      //  auto mBounds = textItem->boundingRect();
      //  textItem->setPos(x-mBounds.width()/2, y);
    //} else {
        textItem->setPos(x*m_width, y*m_height-18);
    //}
    
}

void BaseWidget::drawButtonImage(ButtonGui * button, QPixmap & pixmap){
    int x = m_width*button->m_x-m_height*button->m_rayon;
    int y = m_height*button->m_y-m_height*button->m_rayon;
    
    int d = m_height*button->m_rayon*2;
    
    auto item = new QGraphicsPixmapItem(pixmap);
    item->setScale(0.4);
    item->setPos(x, y);
    scene->addItem(item);
    
    //scene->addEllipse(x, y, d, d, QPen(QColor(0,0,0)), QBrush(QColor(255, 0, 0)));
}


void BaseWidget::drawButton(ButtonGui * button, int color){
    int x = m_width*button->m_x-m_height*button->m_rayon;
    int y = m_height*button->m_y-m_height*button->m_rayon;
    int d = m_height*button->m_rayon*2;
    
    if(color == COLOR_RED){
        scene->addEllipse(x, y, d, d, QPen(QColor(0,0,0)), QBrush(QColor(255, 0, 0)));
    } else if(color == COLOR_GREEN){
        scene->addEllipse(x, y, d, d, QPen(QColor(0,0,0)), QBrush(QColor(0, 255, 0)));
    } else if(color == COLOR_CHECK){
        scene->addEllipse(x, y, d, d, QPen(QColor(0,0,0)), QBrush(QColor(255,255,255)));
        scene->addEllipse(x, y, d*0.8, d*0.8, QPen(QColor(0,0,0)), QBrush(QColor(155,155,155)));
        
            
    } else {
        scene->addEllipse(x, y, d*0.8, d*0.8, QPen(QColor(0,0,0)), QBrush(QColor(255,255,255)));
    }
}


void BaseWidget::drawButtonLabel(ButtonGui * button, int color){
    double x = m_width*button->m_x-m_height*button->m_rayon;
    double y = m_height*button->m_y-m_height*button->m_rayon;
    double d = m_height*button->m_rayon*2;
    
    if(color == COLOR_RED){
        scene->addEllipse(x, y, d, d, QPen(QColor(0,0,0)), QBrush(QColor(255, 0, 0)));
    } else if(color == COLOR_GREEN){
        scene->addEllipse(x, y, d, d, QPen(QColor(0,0,0)), QBrush(QColor(0, 255, 0)));
    } else if(color == COLOR_CHECK){
        scene->addEllipse(x, y, d, d, QPen(QColor(0,0,0)), QBrush(QColor(255,255,255)));
        scene->addEllipse(x+d*0.1, y+d*0.1, d*0.8, d*0.8, QPen(QColor(0,0,0)), QBrush(QColor(155,155,155)));
        
            
    } else {
        scene->addEllipse(x, y, d, d, QPen(QColor(0,0,0)), QBrush(QColor(255,255,255)));
    }
    
    drawText(button->m_label, button->m_x-20, button->m_y);
}

void BaseWidget::drawText(const std::string & text, int x, int y, int size, bool center){
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

QPixmap * BaseWidget::loadImage(const std::string & s){
    std::string s2 = ProjectSourceDir+s;
    QImage image2(QString::fromStdString(s2));
    QPixmap * res  = new QPixmap(QPixmap::fromImage(image2));
    return res;
}

void BaseWidget::drawValueGui(ValueGui * valueGui, double value){
    drawButtonLabel(&(valueGui->m_buttonMinus), 0);
    drawButtonLabel(&(valueGui->m_buttonAdd), 0);
    
    std::ostringstream strs;
    strs << valueGui->m_label << value;
    drawText(strs.str(), valueGui->m_x + 3*valueGui->m_rayon, valueGui->m_y);
    
}

