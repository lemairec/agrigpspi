#ifndef PARSER_H
#define PARSER_H
#include <string>
#include <memory>

class Parser {
public:
    Parser();
    void init();
    
    void printBuffer();
    
    void readFrame(const std::string & frame);
    void readChar(char c);

protected:
    size_t m_bufferIndLast = 0;
    char m_buffer[200];
    size_t m_tempInd = 0;
    
    void resetBuffer();
    void error();
    void debug();
    
    virtual void parseBuffer() = 0;
    
    void readUntilCommat();
    int getOneInt();
    int getIntWithChar(char c);
    int readNegInt();
    int readInt();
    double readDouble();
    double readDeg();
    
    double getTimeHour(double d);
};

#endif //GPS_H
