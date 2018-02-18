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
