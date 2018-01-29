#include "options_widget.hpp"
#include <QLabel>
#include "../gps_framework.hpp"
#include "environnement.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/algorithm/string.hpp>

std::string execute(std::string cmd){
    std::string file = ProjectSourceBin + "/tmp_cmd";
    std::string cmd2 = cmd + " > " + file;
    system(cmd2.c_str());
    std::ifstream infile(file);
    std::stringstream strStream;
    strStream << infile.rdbuf();//read the file
    std::string res = strStream.str();
    return res;
}

OptionsWidget::OptionsWidget(QWidget *parent)
    :QDialog(parent)
{
    QVBoxLayout * layout = new QVBoxLayout();
    setLayout(layout);
    
    {
        QHBoxLayout * hlayout = new QHBoxLayout();
        QLabel * label = new QLabel("largeur");
        m_boxLargeur = new QDoubleSpinBox();
        m_boxLargeur->setSingleStep(0.2);
        m_boxLargeur->setRange(0,50);
        
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
        
        hlayout->addWidget(label);
        hlayout->addWidget(m_boxInput);
        layout->addLayout(hlayout);
    }
    m_okButton = new QPushButton("ok");
    layout->addWidget(m_okButton);
    m_pullButton = new QPushButton("pull");
    layout->addWidget(m_pullButton);
    
    
    setValue();
    addSerial();
    
    connect(m_boxLargeur, SIGNAL(valueChanged(double)), this, SLOT(onOk()));
    connect(m_boxInput, SIGNAL(currentIndexChanged(int)), this, SLOT(onOk()));
    
    connect(m_okButton, SIGNAL(clicked()), this, SLOT(onOk()));
    connect(m_pullButton, SIGNAL(clicked()), this, SLOT(openPull()));
    
}

OptionsWidget::~OptionsWidget()
{
}

void OptionsWidget::addSerial(){
    {
        std::string res = execute("ls /dev/cu.*");
        std::vector<std::string> strs;
        boost::split(strs, res, boost::is_any_of("\n"));
        for(auto s : strs){
            if(!s.empty()){
                INFO(s);
                QString s2 = QString::fromStdString(s);
                m_boxInput->addItem(s2);
            }
        }
    }
    {
        std::string res = execute("ls /dev/ttyACM*");
        std::vector<std::string> strs;
        boost::split(strs, res, boost::is_any_of("\n"));
        for(auto s : strs){
            if(!s.empty()){
                INFO(s);
                QString s2 = QString::fromStdString(s);
                m_boxInput->addItem(s2);
            }
        }
    }
}


void OptionsWidget::setValue(){
    m_boxLargeur->setValue(GpsFramework::Instance().m_config.m_largeur);
}

void OptionsWidget::onOk(){
    INFO("ok");
    GpsFramework::Instance().m_config.m_largeur = m_boxLargeur->value();
    GpsFramework::Instance().m_config.m_input = m_boxInput->currentText().toUtf8().constData();
    GpsFramework::Instance().m_config.save();
    GpsFramework::Instance().initOrLoadConfig();
    INFO("close");
    //close();
    INFO("ok done");
}

void OptionsWidget::openPull(){
    std::string cmd = "cd " + ProjectSourceDir + "; git pull";
    INFO(cmd);
    std::string res = execute(cmd);
    INFO(res);
    exit(0);
}
