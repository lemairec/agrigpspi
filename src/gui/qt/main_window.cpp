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

void MyWidget::mouseReleaseEvent ( QMouseEvent * event ){
    int x = event->x()-5;
    int y = event->y()-5;
    
    m_gpsWidget->onMouse(x, y);
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
    m_timer->start(200);
    

    
    
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
    
    m_my_widget = new MyWidget();
    m_gpsWidget = GpsWidget::Instance();
    m_my_widget->m_gpsWidget = m_gpsWidget;
    m_my_widget->m_gpsWidget->setSize(800, 480);
    

    //m_my_widget->setupUi();
    this->setCentralWidget(m_my_widget);
    
    
    GpsFramework & f = GpsFramework::Instance();
    if(true){
        showFullScreen();
    } else {
        creerMenu();
    }
    
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
    m_gpsWidget->setSize(width, height);
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
    //m_view->m_gpsWidget->draw();
    DEBUG("end");
}

void MainWindow::onTimerSlot(){
    DEBUG("begin");
    m_my_widget->update();
    //m_view->m_gpsWidget->draw();
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

