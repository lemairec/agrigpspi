#ifndef LINE_LOAD_WIDGET_H
#define LINE_LOAD_WIDGET_H

#include "../base_widget.hpp"

class LineLoadWidget : public BaseWidget {
    QPixmap * m_imgOk;
    QPixmap * m_imgCancel;
    ButtonGui m_buttonOk;
    ButtonGui m_buttonCancel;
    
    SelectButtonGui m_selectLines;
    
    int m_x = 0;
    int m_lg = 20;
public:
    LineLoadWidget();
    
    void setSize(int width, int height);
    
    void my_projete(double x, double y, double & x_res, double & y_res);
    void drawParcelle();
    
    virtual void draw();
    virtual void onMouse(int x, int y);
    
    void open();
};

#endif
