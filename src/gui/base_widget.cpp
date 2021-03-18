#include "base_widget.hpp"
#include "environnement.hpp"
#include <iostream>
#include <sstream>
#include "../util/directory_manager.hpp"

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
     if(m_selectedValue >=0 && m_selectedValue < ((int)m_values.size())){
         return m_values[m_selectedValue];
     } else {
         return "error";
     }
 }
 
 int SelectButtonGui::getValueInt(){
    if(m_selectedValue >=0 && m_selectedValue < ((int)m_values.size())){
        return m_values_int[m_selectedValue];
    } else {
        return 0;
    }
}
 
void SelectButtonGui::setValueInt(int i){
    m_selectedValue = -1;
    for(size_t j = 0; j < m_values_int.size(); ++j){
        if(m_values_int[j] == i){
            m_selectedValue = j;
        }
    }
}
void SelectButtonGui::setValueString(std::string s){
    m_selectedValue = -1;
    for(size_t j = 0; j < m_values.size(); ++j){
        if(m_values[j] == s){
            m_selectedValue = j;
        }
    }
}



void ValueGuiKeyPad::setResize(int x, int y){
    m_x = x;
    m_y = y;
}

int ValueGuiKeyPad::isActive(double x, double y){
    return 0;
}

void ValueGuiKeyBoard::setResize(int x, int y){
    m_x = x;
    m_y = y;
}

int ValueGuiKeyBoard::isActive(double x, double y){
    return 0;
}




BaseWidget::BaseWidget(){
    m_penBlack = QPen(Qt::black);
    m_penGray = QPen(QColor(120,120,120));
    m_penRed = QPen(Qt::red);
    m_penGreen = QPen(Qt::green);
    m_penBlue = QPen(Qt::blue);
    m_penBlue2 = QPen(Qt::blue, 5);
    
    
    m_brushGray = QBrush (QColor(200,200,200));
    m_brushParcelle = QBrush(QColor(133,146,118, 100));

    m_brushBackGround1 = QBrush(QColor(183,166,138));
    m_brushBackGround2 = QBrush(QColor(183*1.2,166*1.2,138*1.2));
    
    m_brushDarkGray = QBrush (QColor(60,60,60));
    
    QColor gray = QColor(120,120,120);
    gray.setAlphaF(0.95);
    m_brushLightGrayDebug = QBrush (gray);
    
    
    m_brushGreen = QBrush (Qt::green);
    
    m_brushWhite = QBrush(QColor(255,255,255));
    
    QColor white = QColor(255,255,255);
    white.setAlphaF(0.9);
    m_brushWhiteAlpha =QBrush(white);
    m_brushGreenAlpha = QBrush(QColor(0, 150, 0, 100));
    m_penNo.setColor(QColor(0, 250, 0, 0));
    m_brushOrange = QBrush(QColor(255, 127, 0));
    m_brushRed = QBrush(Qt::red);
    
    int alpha = 255;
    //alpa = 100;
    QColor colorTracteur = QColor(0,0,200,alpha);
    m_brushTracteur = QBrush(colorTracteur);
    
    QColor colorOutil = QColor(60,60,200,alpha);
    m_brushOutil = QBrush(colorOutil);
    
    /*m_penOutil = QPen(colorOutil, 0.3*m_zoom);
    m_penTractorEssieu = QPen(color, 0.15*m_zoom);
    m_penTractorRoue = QPen(color, 0.3*m_zoom);*/
    
}


void BaseWidget::setSize(int width, int height){
    m_gros_button = 0.05*height;
    m_petit_button = 0.035*height;
    m_gros_gros_button = 0.1*height;
    m_width = width;
    m_height = height;
}

void BaseWidget::drawButtonImage(ButtonGui & button, QPixmap * pixmap, double scale){
    double scale2 = 0.4*scale;
    int w = pixmap->size().width()*scale2;
    int h = pixmap->size().height()*scale2;

    m_painter->drawPixmap(button.m_x-w/2, button.m_y-h/2, w, h, *pixmap);
}

void BaseWidget::drawMyImage(QPixmap & pixmap, int x, int y, double scale, bool center){
    double scale2 = 0.4*scale;
    
    int w = pixmap.size().width()*scale2;
    int h = pixmap.size().height()*scale2;

    m_painter->drawPixmap(x-w/2, y-h/2, w, h, pixmap);
}


void BaseWidget::drawButton(ButtonGui & button, int color){
    int x = button.m_x-button.m_rayon;
    int y = button.m_y-button.m_rayon;
    int d = button.m_rayon*2;
    
    m_painter->setPen(m_penBlack);
    if(color == COLOR_RED){
        m_painter->setBrush(QBrush(QColor(255, 0, 0)));
        m_painter->drawEllipse(x, y, d, d);
    } else if(color == COLOR_GREEN){
        m_painter->setBrush(QBrush(QColor(0, 255, 0)));
        m_painter->drawEllipse(x, y, d, d);
    } else if(color == COLOR_CHECK){
        int d2 = d*0.1;
        m_painter->setBrush(QBrush(QColor(255, 255, 255)));
        m_painter->drawEllipse(x, y, d, d);
        m_painter->setBrush(QBrush(QColor(155, 155, 155)));
        m_painter->drawEllipse(x+d2, y+d2, d-2*d2, d-2*d2);
    } else {
        m_painter->setBrush(QBrush(QColor(255, 255, 255)));
        m_painter->drawEllipse(x, y, d, d);
    }
}


void BaseWidget::drawButtonLabel(ButtonGui & button, int color){
    drawButton(button, color);
    
    drawText(button.m_label, button.m_x-20, button.m_y, sizeText_medium);
}

void BaseWidget::drawSelectButtonGuiOpen(SelectButtonGui & select){
    if(select.m_open){
        m_painter->setPen(m_penBlack);
        m_painter->setBrush(m_brushLightGrayDebug);
        m_painter->drawRect(select.m_x, select.m_y, m_width*0.4, (select.m_buttons.size()+1)*select.m_rayon*2);
        for(int i = 0; i < (int)select.m_buttons.size(); ++i){
            if(select.m_selectedValue == i){
                drawButtonLabel(*(select.m_buttons[i]), COLOR_CHECK);
            } else {
                drawButtonLabel(*(select.m_buttons[i]), COLOR_OTHER);
            }
            drawText(select.m_values[i], select.m_buttons[i]->m_x+2*select.m_buttonOpen.m_rayon, select.m_buttons[i]->m_y, sizeText_medium);
        }
    }
}

void BaseWidget::drawSelectButtonGuiClose(SelectButtonGui & select){
    if(!select.m_open){
        drawButtonLabel(select.m_buttonOpen);
        drawText(select.getValueString(), select.m_buttonOpen.m_x+2*select.m_buttonOpen.m_rayon, select.m_buttonOpen.m_y, sizeText_medium);
    }
}

int BaseWidget::onMouseSelectButton(SelectButtonGui & select, double x, double y){
    int res = 0;
    if(select.m_open){
        for(size_t i = 0; i < select.m_buttons.size(); ++i){
            if(select.m_buttons[i]->isActive(x, y)){
                select.m_selectedValue = i;
                res = 1;
            };
        }
        select.m_open = false;
    }
    if( select.m_buttonOpen.isActive(x, y)){
        select.m_open = !select.m_open;
    }
    return res;
}

void BaseWidget::drawText(const std::string & text, int x, int y, SizeText size, bool center, bool white){
    QString s = QString::fromStdString(text);
    
    drawQText(s, x, y, size, center, white);
}

void BaseWidget::drawQText(const QString & s, int x, int y, SizeText size, bool center, bool white){
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
    QFont font = QFont("Latin", s2, 1, false);
    font.setBold(true);
    //textItem->setFont(font);
    if(white){
        m_painter->setPen(Qt::white);
    }
    
    font.setPixelSize(s2);
    m_painter->setFont(font);
    
    if(center){
        //auto mBounds = textItem->boundingRect();
        auto fm = m_painter->fontMetrics();
        auto rect = fm.boundingRect(s);
        m_painter->drawText(x-rect.width()/2, y+5, s);
    } else {
        m_painter->drawText(x, y+2, s);
    }
   
}

void BaseWidget::drawQTexts(const QString & s, int x, int y, SizeText size, bool center, bool white){
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
    QFont font = QFont("Latin", s2, 1, false);
    font.setBold(true);
    //textItem->setFont(font);
    if(white){
        m_painter->setPen(Qt::white);
    }
    
    QStringList list1 = s.split(QLatin1Char('\n'));
    
    font.setPixelSize(s2);
    m_painter->setFont(font);
    
    int y2 = y+5;
    for(auto s3 : list1){
        if(center){
            //auto mBounds = textItem->boundingRect();
            auto fm = m_painter->fontMetrics();
            auto rect = fm.boundingRect(s3);
            m_painter->drawText(x-rect.width()/2, y2, s3);
        } else {
            m_painter->drawText(x, y2, s3);
        }
        y2 += s2*1.2;
    }
   
}

void BaseWidget::drawValueGui2(ValueGui & valueGui, QPixmap * pixmap1, QPixmap * pixmap2, std::string s){
    
    drawButtonImage((valueGui.m_buttonAdd), pixmap1);
    drawButtonImage((valueGui.m_buttonMinus), pixmap2);
    drawText(s, valueGui.m_x+valueGui.m_rayon*3, valueGui.m_y, sizeText_medium);

}
QPixmap * BaseWidget::loadImage(const std::string & s){
    std::string s2 = DirectoryManager::Instance().getSourceDirectory()+s;
    QImage image2(QString::fromStdString(s2));
    QPixmap * res  = new QPixmap(QPixmap::fromImage(image2));
    return res;
}

void BaseWidget::drawValueGui(ValueGui & valueGui, double value){
    drawButtonLabel((valueGui.m_buttonMinus), 0);
    drawButtonLabel((valueGui.m_buttonAdd), 0);
    
    std::ostringstream strs;
    strs << valueGui.m_label << value;
    drawText(strs.str(), valueGui.m_x + 3*valueGui.m_rayon, valueGui.m_y, sizeText_medium);
    
}


void BaseWidget::drawValueGuiKeyPad(ValueGuiKeyPad & value){
    QString s = QString::number(value.m_value);
    m_painter->setPen(m_penBlack);
    m_painter->setBrush(m_brushGreenAlpha);
    m_painter->drawRect(value.m_x-40, value.m_y-15, 80, 30);
    drawQText(s, value.m_x, value.m_y, sizeText_medium, true);
    if(value.m_label.size() > 0){
        drawText(value.m_label, value.m_x-60, value.m_y, sizeText_medium, true);
    }
    if(value.m_unity.size() > 0){
        drawText(value.m_unity, value.m_x+60, value.m_y, sizeText_medium, true);
    }
    
}

bool BaseWidget::isActiveValueGuiKeyPad(ValueGuiKeyPad & value, int x, int y){
    if(value.m_x-30 < x && x < value.m_x + 30 && value.m_y-15 < y && y < value.m_y+15){
        return true;
    }
    return false;
    
}


void BaseWidget::drawValueGuiKeyBoard(ValueGuiKeyBoard & value){
    m_painter->setPen(m_penBlack);
    m_painter->setBrush(m_brushGreenAlpha);
    m_painter->drawRect(value.m_x-80, value.m_y-15, 160, 30);
    drawText(value.m_text, value.m_x, value.m_y, sizeText_medium, true);
}
bool BaseWidget::isActiveValueGuiKeyBoard(ValueGuiKeyBoard & value, int x, int y){
    if(value.m_x-80 < x && x < value.m_x + 80 && value.m_y-15 < y && y < value.m_y+15){
       return true;
   }
   return false;
}
