#include "main_window.hpp"
#include "options_widget.hpp"

#include <QGridLayout>
#include <QTabWidget>
#include <QStatusBar>

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
    
    CloseButton = new QPushButton();
    OptionsButton->setGeometry(QRect(20, 20, 100, 50));
    GPSButton->setGeometry(QRect(20, 80, 100, 50));
    CloseButton->setGeometry(QRect(20, 140, 100, 50));
    
    
    QTabWidget * tab = new QTabWidget();
    m_gpsWidget = new GpsWidget();
    tab->addTab(m_gpsWidget, "gps");
    OptionsWidget * optionsWidget = new OptionsWidget();
    tab->addTab(optionsWidget, "config");
    
    
    this->setCentralWidget(tab);
    tab->setTabPosition(QTabWidget::South);
    
    
    //OptionsBaseRtk * options = new OptionsBaseRtk(centralWidget);
    //xoptions->setGeometry(QRect(140, 20, 680, 440));
    m_statusBar = new QStatusBar;
    setStatusBar(m_statusBar);
    m_statusBar->showMessage("prêt");
    
    showFullScreen();
}

void MainWindow::setMessageStatusBar(QString & s){
    m_statusBar->showMessage(s);
}

void MainWindow::retranslateUi()
{
    this->setWindowTitle(QApplication::translate("MainWindow", "RTKLIB for Raspberry", 0));
    OptionsButton->setText(QApplication::translate("MainWindow", "Options"));
    GPSButton->setText(QApplication::translate("MainWindow", "GPS", 0));
    CloseButton->setText(QApplication::translate("MainWindow", "CLOSE GUI", 0));
}

void MainWindow::OuvreOptionsRover(){

}
void MainWindow::OuvreOptions(){
}
