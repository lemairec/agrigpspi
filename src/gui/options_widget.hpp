#ifndef OPTIONS_WIDGET_HPP
#define OPTIONS_WIDGET_HPP

#include "include_qt.hpp"

class OptionsWidget : public QDialog
{
    Q_OBJECT
    
    QDoubleSpinBox * m_boxLargeur;
    QComboBox * m_boxInput;
    QPushButton * m_okButton;
public:
    OptionsWidget(QWidget *parent = 0);
    ~OptionsWidget();
    
    QPushButton * m_pullButton;
    void setValue();
    void addSerial();
public slots:
    void onOk();
    void openPull();

};

#endif
