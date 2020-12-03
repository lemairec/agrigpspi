#ifndef PARCELLE_LOAD_WIDGET_H
#define PARCELLE_LOAD_WIDGET_H

#include "../base_widget.hpp"

class ParcelleLoadWidget : public BaseWidget {
    QPixmap * m_imgOk;
    QPixmap * m_imgCancel;
    QPixmap * m_imgLigneAB;
    QPixmap * m_imgCurveAB;
      
    ButtonGui m_buttonOk;
    ButtonGui m_buttonCancel;
    
    SelectButtonGui m_selectParcelles;
    SelectButtonGui m_selectLine;
    int m_parcelleSelected = 0;
    
    
    ButtonGui m_buttonLigneCurve;
    ButtonGui m_buttonDemiOutil;
    
    bool m_line = true;
    bool m_demi_outil = true;
    
    int m_x = 0;
    int m_lg = 20;
public:
    ParcelleLoadWidget();
    
    void setSize(int width, int height);
    
    void my_projete(double x, double y, double & x_res, double & y_res);
    void drawParcelle();
        
    virtual void draw();
    virtual void onMouse(int x, int y);
    
    void open();
    
};

#endif
