#include "gui/qt/main_window.hpp"
#include <QApplication>
#include "gps_framework.hpp"


int main(int argc, char *argv[])
{
    GpsFramework & framework = GpsFramework::Instance();
    //framework.test();
    QApplication app(argc, argv);
    MainWindow * m = MainWindow::Instance_ptr();
    framework.addGpsObserver(m);
    m->show();
    INFO(QThread::currentThreadId());
    GpsThread * gpsThread = new GpsThread(framework);
    gpsThread->start();
    return app.exec();
}

/*#include <iostream>
#include "serial.hpp"

using namespace std;
using namespace boost;

int main(int argc, char* argv[])
{
    try {
        
        Serial serial("/dev/cu.wchusbserial1420",9600);
        
        //serial.writeString("Hello world\n");
        
        while(true){
            cout<<serial.readChar();
        }
        
    } catch(boost::system::system_error& e)
    {
        cout<<"Error: "<<e.what()<<endl;
        return 1;
    }
}
*/



