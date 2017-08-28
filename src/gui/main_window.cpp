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
    
    
    auto actionPull = new QAction(tr("&Pull"), this);
    connect(actionPull, SIGNAL(triggered()), this, SLOT(openPull()));
    
    auto actionQuitter = new QAction(tr("&Quitter"), this);
    actionQuitter->setShortcut(tr("Ctrl+Q"));
    connect(actionQuitter, SIGNAL(triggered()), qApp, SLOT(quit()));
    
    
    auto fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(actionQuitter);
    
    
   
    connect(m_gpsWidget, SIGNAL(setMessageStatusBar(QString &)), this, SLOT(setMessageStatusBar(QString &)));
    connect(m_gpsWidget->m_btnOptions, SIGNAL(clicked()), this, SLOT(openOptions()));
    
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
    
    
    
    QTabWidget * tab = new QTabWidget();
    m_gpsWidget = new GpsWidget();
    tab->addTab(m_gpsWidget, "gps");
    m_optionsWidget = new OptionsWidget();
    //tab->addTab(optionsWidget, "config");
    
    CloseButton = new QPushButton();
    OptionsButton->setGeometry(QRect(20, 20, 100, 50));
    GPSButton->setGeometry(QRect(20, 80, 100, 50));
    CloseButton->setGeometry(QRect(20, 140, 100, 50));
    
    
    this->setCentralWidget(tab);
    tab->setTabPosition(QTabWidget::South);
    
    
    m_statusBar = new QStatusBar;
    setStatusBar(m_statusBar);
    m_statusBar->showMessage("prêt");
    
    
    //OuvreOptions();
    
    //showFullScreen();
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

void MainWindow::openOptions(){
    m_optionsWidget->close();
    m_optionsWidget->show();
    m_optionsWidget->focusWidget();
    m_optionsWidget->setHidden(false);
}

void MainWindow::openPull(){
    QProcess process;
    process.start("cd ~/agrigpspi; git pull");
    exit(0);
}

