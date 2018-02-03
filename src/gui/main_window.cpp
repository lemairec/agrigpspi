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
    
    connect(m_gpsWidget, SIGNAL(setMessageStatusBar(QString &)), this, SLOT(setMessageStatusBar(QString &)));
}

MainWindow::~MainWindow()
{
    GpsFramework::Instance().removeObserver();
}

void MainWindow::setupUi(){
    if (this->objectName().isEmpty()){
        this->setObjectName(QString::fromUtf8("MainWindow"));
    }
    this->resize(800, 480);
    
    m_gpsWidget = new GpsWidget();
    this->setCentralWidget(m_gpsWidget);
    //showMaximized();
    showFullScreen();
    
    
    m_statusBar = new QStatusBar;
    setStatusBar(m_statusBar);
    m_statusBar->showMessage("prêt");
}

void MainWindow::setMessageStatusBar(QString & s){
    m_statusBar->showMessage(s);
}
