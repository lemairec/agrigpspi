#include "main_window.hpp"
#include "../option_widget.hpp"

#include <QGridLayout>
#include <QTabWidget>
#include <QStatusBar>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QResizeEvent>

void View::setupUi(){
    scene = new QGraphicsScene(this);
    setScene(scene);
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    
    m_gpsWidget = new GpsWidget();
    m_gpsWidget->setScene(scene);
    m_gpsWidget->m_optionsWidget.setScene(scene);
    
};

void View::mouseReleaseEvent ( QMouseEvent * event ){
    m_gpsWidget->onMouse(event->x(), event->y());
    if(!m_gpsWidget->m_optionsWidget.m_close){
        m_gpsWidget->m_optionsWidget.onMouseInt(event->x(), event->y());
    }
    m_gpsWidget->draw_force();
}

MainWindow * MainWindow::Instance_ptr(){
    static MainWindow gf;
    return &gf;
}

MainWindow::MainWindow(QWidget *parent)
    :QMainWindow(parent)
{
    connect(this, SIGNAL(onValueChangeSignal()), this, SLOT(onValueChangeSlot()));
    
    setupUi();
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
    
    m_view = new View();
    m_view->setupUi();
    this->setCentralWidget(m_view);
    
    //showMaximized();
    showFullScreen();
}

//debug
//m_view->scene->setSceneRect(0, 0, width-10, height-10);
void MainWindow::resizeEvent(QResizeEvent *event){
    int width = event->size().width();
    int height = event->size().height();
    m_view->scene->setSceneRect(0, 0, width, height);
    
    m_view->m_gpsWidget->setSize(width, height);
    //m_gpsWidget->resizeEvent(event);
}

void MainWindow::onNewPoint(){
    emit onValueChangeSignal();
}

void MainWindow::onValueChangeSlot(){
    m_view->m_gpsWidget->draw();
}


