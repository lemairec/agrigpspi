#include "gui/qt/main_window.hpp"
#include <QApplication>
#include "gps_framework.hpp"

#include <inttypes.h>

/* Function to calculate MODBUS CRC. */
/*

std::vector<char> l{0x01, 0x06, 0x00, 0x6A, 0x00, 0x64};
uint16_t res = calculcrc16Modbus(l);;
printf("%X\n", (int)res);

{
    std::vector<char> l{0x01, 0x10, 0x00, 0x33, 0x00, 0x01, 0x02, 0x00, 0x01};
    for(auto i : l){
        printf("%02" PRIx16 " ", i);
        
    }
    uint16_t res = calculcrc16Modbus(l);;
    printf("%X\n", (int)res);
}
{
    std::vector<char> l{0x01, 0x10, 0x00, 0x33, 0x00, 0x01, 0x02, 0x00, 0x01};
    for(auto i : l){
        printf("%02" PRIx16 " ", i);
        
    }
    uint16_t res = calculcrc16Modbus(l);;
    printf("%X\n", (int)res);
}*/

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    GpsFramework & framework = GpsFramework::Instance();
    //framework.test();
    MainWindow * m = MainWindow::Instance_ptr();
    framework.addGpsObserver(m);
    m->show();
    INFO(QThread::currentThreadId());
    /*GpsThread * gpsThread = new GpsThread(framework);
    gpsThread->start();*/
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



