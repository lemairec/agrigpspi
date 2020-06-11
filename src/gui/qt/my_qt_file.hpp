#ifndef FILE_QT_H
#define FILE_QT_H

#include <QtSerialPort/QSerialPort>

#include "../../config/config.hpp"
#include <QTextStream>
#include <QObject>
#include <QTimer>

QT_USE_NAMESPACE

class MyQTFile : public QObject{
    Q_OBJECT
public:
    MyQTFile();
    
    void initOrLoad(Config & config);
    void open();
    
    void openFile(Config & config);

private:
    QTimer m_timer;
    QTextStream * m_text_stream;
private slots:
    void handleLineFile();
};


#endif
