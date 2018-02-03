#include "include_qt.hpp"
#include "gps_widget.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
    QWidget *centralWidget;
    
    QStatusBar *m_statusBar;
public:
    GpsWidget * m_gpsWidget;

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
    void test();
    void resizeEvent(QResizeEvent* event){
        m_gpsWidget->resizeEvent(event);
    }
public slots:
    void setMessageStatusBar(QString & s);

private:
    void setupUi();
};
