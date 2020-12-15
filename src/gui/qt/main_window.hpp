#include "include_qt.hpp"
#include "../gps_widget.hpp"

class MyWidget : public QWidget
{
  public:
    GpsWidget * m_gpsWidget = NULL;
    MyWidget( QWidget *parent = 0 ) : QWidget( parent ) {}

    void paintEvent(QPaintEvent* e)
    {
        QWidget::paintEvent(e); // effectue le comportement standard

        QPainter painter(this); // construire
        if(m_gpsWidget){
            m_gpsWidget->setPainter(&painter);
            m_gpsWidget->draw();
        }

    } // détruire
    
    void mouseReleaseEvent ( QMouseEvent * event );
};

class MainWindow : public QMainWindow, public IGpsObserver
{
    GpsWidget * m_gpsWidget;
    QTimer *m_timer;
    Q_OBJECT
    
    QWidget *centralWidget;
    explicit MainWindow(QWidget *parent = 0);
    
public:
    static MainWindow * Instance_ptr();
    MyWidget * m_my_widget;

    ~MainWindow();
    
    void test();
    void resizeEvent(QResizeEvent* event);

protected:
    void setupUi();
    void onNewPoint();
    void creerMenu();

signals:
    void onValueChangeSignal();

public slots:
    void onValueChangeSlot();
    void onTimerSlot();
    
    void openFile();

};
