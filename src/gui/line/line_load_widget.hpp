#ifndef LINE_LOAD_WIDGET_H
#define LINE_LOAD_WIDGET_H

#include "../base_widget.hpp"

class LineLoadWidget : public BaseWidget {
    QPixmap * m_imgOk;
    QPixmap * m_imgCancel;
    QPixmap * m_img_up;
    QPixmap * m_img_down;
    ButtonGui m_buttonCancel;
    ButtonGui m_buttonPageUp;
    ButtonGui m_buttonPageDown;
    
    std::vector<ButtonGui *> m_buttons;
    
    int m_x = 0;
    int m_lg = 20;
    int m_inter;
    int m_page = 0;
    int m_nbr_page = 0;
    
public:
    LineLoadWidget();
    
    void setSize(int width, int height);
    
    void my_projete(double x, double y, double & x_res, double & y_res);
    void drawParcelle();
    
    virtual void draw();
    virtual void onMouse(int x, int y);
    
    void open();
    bool m_delete = false;
};

#endif
