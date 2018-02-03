#include "include_qt.hpp"
#include "gps_widget.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
    QWidget *centralWidget;
public:
    GpsWidget * m_gpsWidget;

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
    void test();
    void resizeEvent(QResizeEvent* event){
        m_gpsWidget->resizeEvent(event);
    }
private:
    void setupUi();
};
