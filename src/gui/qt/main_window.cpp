#include "main_window.hpp"
#include "../option_widget.hpp"

#include <QGridLayout>
#include <QTabWidget>
#include <QStatusBar>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QResizeEvent>

#include <QTimer>
#include <QFileDialog>
#include "environnement.hpp"

void View::setupUi(){
    DEBUG("begin");
    scene = new QGraphicsScene(this);
    setScene(scene);
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    
    m_gpsWidget = GpsWidget::Instance();
    m_gpsWidget->setScene(scene);
    m_gpsWidget->m_satWidget.setScene(scene);
    m_gpsWidget->m_guidWidget.setScene(scene);
    m_gpsWidget->m_parcelleWidget.setScene(scene);
    m_gpsWidget->m_parcelleNewWidget.setScene(scene);
    m_gpsWidget->m_parcelleLoadWidget.setScene(scene);
    m_gpsWidget->m_key_pad_widget.setScene(scene);
    m_gpsWidget->m_key_board_widget.setScene(scene);
    
    
    
    DEBUG("end");
    
};

void View::mouseReleaseEvent ( QMouseEvent * event ){
    int x = event->x()-5;
    int y = event->y()-5;
    
    if(!m_gpsWidget->m_key_pad_widget.m_close){
        m_gpsWidget->m_key_pad_widget.onMouse(x, y);
    } else if(!m_gpsWidget->m_key_board_widget.m_close){
        m_gpsWidget->m_key_board_widget.onMouse(x, y);
    } else {
        
        if(!m_gpsWidget->m_satWidget.m_close){
            m_gpsWidget->m_satWidget.onMouse(x, y);
        } else if(!m_gpsWidget->m_guidWidget.m_close){
            m_gpsWidget->m_guidWidget.onMouse(x, y);
        } else if(!m_gpsWidget->m_parcelleNewWidget.m_close){
            m_gpsWidget->m_parcelleNewWidget.onMouse(x, y);
        } else if(!m_gpsWidget->m_parcelleLoadWidget.m_close){
            m_gpsWidget->m_parcelleLoadWidget.onMouse(x, y);
        } else if(!m_gpsWidget->m_parcelleWidget.m_close){
            m_gpsWidget->m_parcelleWidget.onMouse(x, y);
        } else {
            m_gpsWidget->onMouse(x, y);//TODO
        }
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
    DEBUG("begin");
    m_timer = new QTimer(this);
    m_timer->start(500);
    
    connect(m_timer, SIGNAL(timeout()), this, SLOT(onTimerSlot()));
    
    connect(this, SIGNAL(onValueChangeSignal()), this, SLOT(onValueChangeSlot()));
    
    setupUi();
    DEBUG("end");
    
}

MainWindow::~MainWindow()
{
//    GpsFramework::Instance().removeObserver();
}

void MainWindow::setupUi(){
    DEBUG("begin");
    if (this->objectName().isEmpty()){
        this->setObjectName(QString::fromUtf8("MainWindow"));
    }
    this->resize(800, 480);
    
    m_view = new View();
    m_view->setupUi();
    this->setCentralWidget(m_view);
    
#ifdef APPLE
    creerMenu();
    //showMaximized();
#else
    showFullScreen();
#endif
    
    //showMaximized();
    //showFullScreen();
    DEBUG("end");
}

//debug
//m_view->scene->setSceneRect(0, 0, width-10, height-10);
void MainWindow::resizeEvent(QResizeEvent *event){
    DEBUG("begin");
    int width = event->size().width();
    int height = event->size().height();
    m_view->scene->setSceneRect(0, 0, width-10, height-10);
    m_view->setBackgroundBrush(QBrush(QColor(183,166,138)));
    
    m_view->m_gpsWidget->setSize(width-10, height-10);
    //m_gpsWidget->resizeEvent(event);
    DEBUG("end");
}

void MainWindow::onNewPoint(){
    DEBUG("begin");
    onValueChangeSlot();
    DEBUG("end");
}

void MainWindow::onValueChangeSlot(){
    DEBUG("begin");
    m_view->m_gpsWidget->draw();
    DEBUG("end");
}

void MainWindow::onTimerSlot(){
    DEBUG("begin");
    m_view->m_gpsWidget->draw();
    DEBUG("end");
}

void MainWindow::creerMenu()
{
    DEBUG("begin");
    QMenu *menuFichier = menuBar()->addMenu(tr("&Fichier"));
    menuFichier->addAction("open File", this, SLOT(openFile()));
    DEBUG("end");
}

void MainWindow::openFile(){
    DEBUG("begin");
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Address Book"), QString::fromStdString(ProjectSourceDir)+QString("/gps_samples"),
                                                    tr("Gps files (*.ubx)"));
    
    GpsFramework & f = GpsFramework::Instance();
    f.m_config.m_file_gps = fileName.toUtf8().constData();
    f.m_config.m_input_gps = "file";
    f.initOrLoadConfig();
    DEBUG("end");
}

