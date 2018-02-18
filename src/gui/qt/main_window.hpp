#include "include_qt.hpp"
#include "../gps_widget.hpp"


class View : public QGraphicsView {
public:
    GpsWidget * m_gpsWidget;
    QGraphicsScene * scene;

    void setupUi();
    void mouseReleaseEvent ( QMouseEvent * event );
};

class MainWindow : public QMainWindow, public IGpsObserver
{
    Q_OBJECT
    
    QWidget *centralWidget;
    explicit MainWindow(QWidget *parent = 0);
    
public:
    static MainWindow * Instance_ptr();
    View * m_view;

    ~MainWindow();
    
    void test();
    void resizeEvent(QResizeEvent* event);

protected:
    void setupUi();
    void onNewPoint();
    

signals:
    void onValueChangeSignal();

public slots:
    void onValueChangeSlot();
    
};
