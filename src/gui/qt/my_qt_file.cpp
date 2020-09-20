#include "my_qt_file.hpp"
#include "include_qt.hpp"
#include "../../gps_framework.hpp"
#include <sstream>
#include <QMetaEnum>
#include <QSerialPortInfo>

#include "environnement.hpp"

#define FILE_TIME 100

MyQTFile::MyQTFile(){
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(handleLineFile()));
}
void MyQTFile::initOrLoad(Config & config){
    DEBUG("begin");
    if(config.m_input_gps == "file"){
        INFO("file " << config.m_file_gps);
        openFile(config);
    }
    DEBUG("end");
};

void MyQTFile::handleLineFile(){
    DEBUG("begin");
    if(m_text_stream == NULL){
        WARN("null");
        return;
    }
    QString line = m_text_stream->readLine();
    if(!line.isNull()){
       std::string line_s = line.toUtf8().constData();
       for(size_t i = 0; i < line_s.size(); ++i){
           
           GpsFramework::Instance().m_gpsModule.readChar(line_s[i]);
       }
       GpsFramework::Instance().m_gpsModule.readChar('\n');
       //line = m_text_stream->readLine();
    }
    DEBUG("end");
}


void MyQTFile::openFile(Config & config){
    DEBUG("begin");
    if(m_text_stream == NULL){
        QFile * file = new QFile(QString::fromStdString(config.m_file_gps));
        if (!file->open(QIODevice::ReadOnly | QIODevice::Text)){
            WARN("can not open file");
            return;
        }

        m_text_stream = new QTextStream(file);
        m_timer.start(FILE_TIME);
    }
    DEBUG("end");
}
