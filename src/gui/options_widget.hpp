#ifndef OPTIONS_BASE_RTK
#define OPTIONS_BASE_RTK

#include "include_qt.hpp"

class OptionsWidget : public QDialog
{
    Q_OBJECT
    
    QSpinBox * m_boxLargeur;

public:
    OptionsWidget(QWidget *parent = 0);
    ~OptionsWidget();
    
    void setValue();
public slots:
    void onValueChange();
};

#endif
