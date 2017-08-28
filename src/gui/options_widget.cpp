#include "options_widget.hpp"
#include <QLabel>
#include "../gps_framework.hpp"

OptionsWidget::OptionsWidget(QWidget *parent)
    :QDialog(parent)
{
    QVBoxLayout * layout = new QVBoxLayout();
    setLayout(layout);
    
    {
        QHBoxLayout * hlayout = new QHBoxLayout();
        QLabel * label = new QLabel("largeur");
        m_boxLargeur = new QSpinBox();
        m_boxLargeur->setRange(0,50);
        connect(m_boxLargeur, SIGNAL(valueChanged(int)), this, SLOT(onValueChange()));
        
        hlayout->addWidget(label);
        hlayout->addWidget(m_boxLargeur);
        layout->addLayout(hlayout);
    }
    
    {
        QHBoxLayout * hlayout = new QHBoxLayout();
        QLabel * label = new QLabel("input");
        m_boxInput = new QComboBox();
        m_boxInput->addItem("none");
        m_boxInput->addItem("file");
        m_boxInput->addItem("/dev/cu.usbmodem1411");
        connect(m_boxInput, SIGNAL(currentIndexChanged(int)), this, SLOT(onValueChange()));
        
        hlayout->addWidget(label);
        hlayout->addWidget(m_boxInput);
        layout->addLayout(hlayout);
    }
    
    
    
    
    setValue();
}

OptionsWidget::~OptionsWidget()
{
}

void OptionsWidget::setValue(){
    m_boxLargeur->setValue(GpsFramework::Instance().m_config.m_largeur);
}

void OptionsWidget::onValueChange(){
    GpsFramework::Instance().m_config.m_largeur = m_boxLargeur->value();
    GpsFramework::Instance().m_config.m_input = m_boxInput->currentText().toUtf8().toStdString();
    GpsFramework::Instance().initOrLoadConfig();
}

