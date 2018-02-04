#include "base_widget.hpp"

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
