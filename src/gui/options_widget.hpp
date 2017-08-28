#ifndef OPTIONS_WIDGET_HPP
#define OPTIONS_WIDGET_HPP

#include "include_qt.hpp"

class OptionsWidget : public QDialog
{
    Q_OBJECT
    
    QSpinBox * m_boxLargeur;
    QComboBox * m_boxInput;
    QPushButton * m_closeButton;
public:
    OptionsWidget(QWidget *parent = 0);
    ~OptionsWidget();
    
    QPushButton * m_pullButton;
    void setValue();
public slots:
    void onValueChange();
};

#endif
