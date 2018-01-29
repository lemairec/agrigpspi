#include "main_window.hpp"
#include "options_widget.hpp"

#include <QGridLayout>
#include <QTabWidget>
#include <QStatusBar>
#include <QAction>
#include <QMenu>
#include <QMenuBar>


MainWindow::MainWindow(QWidget *parent)
    :QMainWindow(parent)
{
    
    setupUi();
    retranslateUi();
    
    connect(m_gpsWidget, SIGNAL(setMessageStatusBar(QString &)), this, SLOT(setMessageStatusBar(QString &)));
}

MainWindow::~MainWindow()
{
    GpsFramework::Instance().removeObserver();
}

void MainWindow::setupUi(){
    if (this->objectName().isEmpty())
        this->setObjectName(QString::fromUtf8("MainWindow"));
    this->resize(800, 480);
    
    OptionsButton = new QPushButton();
    
    GPSButton = new QPushButton();
    
    
    
    m_gpsWidget = new GpsWidget();
    //m_optionsWidget = new OptionsWidget();
    //tab->addTab(optionsWidget, "config");
    
    CloseButton = new QPushButton();
    OptionsButton->setGeometry(QRect(20, 20, 100, 50));
    GPSButton->setGeometry(QRect(20, 80, 100, 50));
    CloseButton->setGeometry(QRect(20, 140, 100, 50));
    
    
    this->setCentralWidget(m_gpsWidget);
    
    
    m_statusBar = new QStatusBar;
    setStatusBar(m_statusBar);
    m_statusBar->showMessage("prêt");
    
    
    //OuvreOptions();
    
    //showMaximized();
    showFullScreen();
}

void MainWindow::setMessageStatusBar(QString & s){
    m_statusBar->showMessage(s);
}

void MainWindow::retranslateUi()
{
    this->setWindowTitle(QApplication::translate("MainWindow", "AgriPi", 0));
    OptionsButton->setText(QApplication::translate("MainWindow", "Options"));
    GPSButton->setText(QApplication::translate("MainWindow", "GPS", 0));
    CloseButton->setText(QApplication::translate("MainWindow", "CLOSE GUI", 0));
}

