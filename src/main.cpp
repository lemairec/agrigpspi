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

#include "boost/serial_port.hpp"
int main(int argc, char *argv[])
{
    /*SerialPort::print_devices();
    std::string name = SerialPort::get_port_name(0);

    SerialPort serial;
    bool rv = serial.start("/dev/tty.usbserial-141240", 115200);
    if (rv == false) {
        return -1;
    }

    // initialize
    serial.end_of_line_char(0x0d);
    //serial.write_some("BI010\r\n");
    //serial.write_some("PE011\r\n");
    //serial.write_some("RA01000\r\n");

    // wait
    sleep(10000 * 1000);

    serial.stop();

    return 0;*/
    QApplication app(argc, argv);
    GpsFramework & framework = GpsFramework::Instance();
    //framework.test();
    MainWindow * m = MainWindow::Instance_ptr();
    DEBUG("init");
    framework.initOrLoadConfig();
    DEBUG("fin init");
    m->show();
    DEBUG("exec");
    //INFO(QThread::currentThreadId());
    //GpsThread * gpsThread = new GpsThread(framework);
    //gpsThread->start();
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



