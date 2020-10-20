#include "base_widget.hpp"
#include "environnement.hpp"
#include <iostream>
#include <sstream>

ButtonGui::ButtonGui(double x, double y, double rayon, int type)
:m_x(x), m_y(y), m_rayon(rayon), m_type(type)
{
}

void ButtonGui::setResize(int x, int y, int rayon){
    m_x = x;
    m_y = y;
    m_rayon = rayon;
}


bool ButtonGui::isActive(int x, int y){
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

void ValueGui::setResize(int x, int y, int rayon, std::string label){
    m_x = x;
    m_y = y;
    m_rayon = rayon;
    m_label = label;
    m_buttonAdd.setResize(x+1.5*rayon, y, rayon);
    m_buttonMinus.setResize(x-1.5*rayon, y, rayon);
    
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

double ValueGui::getMultValue2(double x, double y, double value){
    if(m_buttonAdd.isActive(x, y)){
        return value;
    }
    if(m_buttonMinus.isActive(x, y)){
        return 1.0/value;
    }
    return 1.0;
}

double ValueGui::getMultValue(double x, double y){
    return getMultValue2(x, y, 1.2);
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

SelectButtonGui::SelectButtonGui(double x, double y, double rayon)
    : m_x(x), m_y(y), m_rayon(rayon), m_buttonOpen(x, y, rayon, 0)
{
    
}

void SelectButtonGui::setResize(int x, int y, int rayon){
    m_x = x;
    m_y = y;
    m_rayon = rayon;
    m_buttonOpen.setResize(x, m_y, rayon);
    for(size_t i = 0; i < m_values.size(); ++i){
        m_buttons[i]->setResize(x, m_y+m_rayon*2*(i+1), rayon*0.8);
    }
    
}

void SelectButtonGui::addValue(std::string s){
    m_values.push_back(s);
    m_buttons.clear();
    for(size_t i = 0; i < m_values.size(); ++i){
        m_buttons.push_back(new ButtonGui(m_x+m_rayon, m_y+m_rayon*2*(i+1), m_rayon*0.8, 0));
    }
}

void SelectButtonGui::addValueInt(std::string s, int i){
    m_values.push_back(s);
    m_values_int.push_back(i);
    m_buttons.clear();
    for(size_t i = 0; i < m_values.size(); ++i){
        m_buttons.push_back(new ButtonGui(m_x+m_rayon, m_y+m_rayon*2*(i+1), m_rayon*0.8, 0));
    }
}

void SelectButtonGui::clear(){
    m_values.clear();
    m_buttons.clear();
    m_values_int.clear();
}

std::string SelectButtonGui::getValueString(){
     if(m_selectedValue >=0 && m_selectedValue < m_values.size()){
         return m_values[m_selectedValue];
     } else {
         return "error";
     }
 }
 
 int SelectButtonGui::getValueInt(){
    if(m_selectedValue >=0 && m_selectedValue < m_values_int.size()){
        return m_values_int[m_selectedValue];
    } else {
        return 0;
    }
}
 
void SelectButtonGui::setValueInt(int i){
    m_selectedValue = -1;
    for(int j = 0; j < m_values_int.size(); ++j){
        if(m_values_int[j] == i){
            m_selectedValue = j;
        }
    }
}
void SelectButtonGui::setValueString(std::string s){
    m_selectedValue = -1;
    for(int j = 0; j < m_values.size(); ++j){
        if(m_values[j] == s){
            m_selectedValue = j;
        }
    }
}




BaseWidget::BaseWidget(){
    m_penBlack = QPen(Qt::black);
    m_penRed = QPen(Qt::red);
    m_penBlue = QPen(Qt::blue);
    m_penBlue2 = QPen(Qt::blue, 5);
    
    m_grayBrush = QBrush (QColor(200,200,200));
    
    m_brushDarkGray = QBrush (QColor(60,60,60));
    
    QColor gray = QColor(120,120,120);
    gray.setAlphaF(0.95);
    m_brushLightGrayDebug = QBrush (gray);
    
    
    m_greenBrush = QBrush (Qt::green);
    
    m_brushWhite = QBrush(QColor(255,255,255));
    
    QColor white = QColor(255,255,255);
    white.setAlphaF(0.9);
    m_brushWhiteAlpha =QBrush(white);
    m_brushGreen = QBrush(QColor(0, 150, 0, 100));
    m_brushGreenTractor = QBrush(QColor(0, 150, 0));
    m_penNo.setColor(QColor(0, 250, 0, 0));
}


void BaseWidget::setSize(int width, int height){
    m_gros_button = 0.05*height;
    m_petit_button = 0.035*height;
    m_gros_gros_button = 0.07*height;
    m_width = width;
    m_height = height;
}

void BaseWidget::onMouseInt(double x, double y){
    this->onMouse(x/m_width, y/m_height);
}


void BaseWidget::drawButtonImage(ButtonGui * button, QPixmap & pixmap, double scale){
    double scale2 = 0.4*scale;
    int x = button->m_x-button->m_rayon;
    int y = button->m_y-button->m_rayon;
    int d = button->m_rayon*2;
    
    auto item = new QGraphicsPixmapItem(pixmap);
    item->setScale(scale2);
    auto size = item->boundingRect();
    item->setPos(button->m_x-size.width()*scale2/2, button->m_y-size.height()*scale2/2);
    scene->addItem(item);
    
    //scene->addRect(x, y, d, d, QPen(QColor(0,0,0)), QBrush(QColor(255, 0, 0)));
    //scene->addEllipse(m_width*button->m_x, m_height*button->m_y, 1, 1, QPen(QColor(0,0,0)), QBrush(QColor(0, 0, 0)));
}


void BaseWidget::drawButton(ButtonGui * button, int color){
    int x = button->m_x-button->m_rayon;
    int y = button->m_y-button->m_rayon;
    int d = button->m_rayon*2;
    
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
    int x = button->m_x-button->m_rayon;
    int y = button->m_y-button->m_rayon;
    int d = button->m_rayon*2;
    
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
    
    drawText(button->m_label, button->m_x-20, button->m_y, sizeText_medium);
}

void BaseWidget::drawSelectButtonGuiOpen(SelectButtonGui *select){
    if(select->m_open){
        scene->addRect(select->m_x, select->m_y, m_width*0.4, (select->m_buttons.size()+1)*select->m_rayon*2, m_penBlack, m_brushLightGrayDebug);
        for(size_t i = 0; i < select->m_buttons.size(); ++i){
            if(select->m_selectedValue == i){
                drawButtonLabel(select->m_buttons[i], COLOR_CHECK);
            } else {
                drawButtonLabel(select->m_buttons[i], COLOR_OTHER);
            }
            drawText(select->m_values[i], 0.4*m_width, select->m_buttons[i]->m_y, sizeText_medium);
        }
    }
}

void BaseWidget::drawSelectButtonGuiClose(SelectButtonGui *select){
    if(!select->m_open){
        drawButtonLabel(&(select->m_buttonOpen));
        drawText(select->getValueString(), select->m_buttonOpen.m_x+2*select->m_buttonOpen.m_rayon, select->m_buttonOpen.m_y, sizeText_medium);
    }
}

int BaseWidget::onMouseSelectButton(SelectButtonGui *select, double x, double y){
    int res = 0;
    if(select->m_open){
        for(size_t i = 0; i < select->m_buttons.size(); ++i){
            if(select->m_buttons[i]->isActive(x, y)){
                select->m_selectedValue = i;
                res = 1;
            };
        }
        select->m_open = false;
    }
    if( select->m_buttonOpen.isActive(x, y)){
        select->m_open = !select->m_open;
    }
    return res;
}

void BaseWidget::drawText(const std::string & text, int x, int y, SizeText size, bool center, bool white){
    QString s = QString::fromStdString(text);
    
    drawQText(s, x, y, size, center, white);
}

void BaseWidget::drawQText(const QString & s, int x, int y, SizeText size, bool center, bool white){
    auto textItem = scene->addText(s);
    
    int s2 = 10;
    switch (size) {
        case sizeText_big:
            s2 = 22;
            break;
        case sizeText_medium:
            s2 = 18;
            break;
        case sizeText_little:
            s2 = 14;
            break;
    }
    textItem->setFont(QFont("Latin", s2, 1, false));
    if(white){
        textItem->setDefaultTextColor(Qt::white);
    }
    
    if(center){
        auto mBounds = textItem->boundingRect();
        textItem->setPos(x-mBounds.width()/2, y-s2+2);
    } else {
        textItem->setPos(x, y-s2+2);
    }
}

void BaseWidget::drawValueGui2(ValueGui * valueGui, QPixmap * pixmap1, QPixmap * pixmap2, std::string s){
    
    drawButtonImage(&(valueGui->m_buttonAdd), *pixmap1);
    drawButtonImage(&(valueGui->m_buttonMinus), *pixmap2);
    drawText(s, valueGui->m_x+valueGui->m_rayon*3, valueGui->m_y, sizeText_medium);

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
    drawText(strs.str(), valueGui->m_x + 3*valueGui->m_rayon, valueGui->m_y, sizeText_medium);
    
}

