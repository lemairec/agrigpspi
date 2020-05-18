#ifndef SERIAL_HPP
#define SERIAL_HPP

#include <boost/asio.hpp>

//http://www.webalice.it/fede.tft/serial_port/serial_port.html

using namespace boost;

class Serial
{
public:
    /**
     * Constructor.
     * \param port device name, example "/dev/ttyUSB0" or "COM4"
     * \param baud_rate communication speed, example 9600 or 115200
     * \throws boost::system::system_error if cannot open the
     * serial device
     */
    Serial(std::string port, unsigned int baud_rate)
    : io(), serial(io,port)
    {
        serial.set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
    }
    
    /**
     * Write a string to the serial device.
     * \param s string to write
     * \throws boost::system::system_error on failure
     */
    void writeString(std::string s)
    {
        boost::asio::write(serial,boost::asio::buffer(s.c_str(),s.size()));
    }
    
    
    char readChar()
    {
        char c;
        asio::read(serial,asio::buffer(&c,1));
        return c;
    }
    
    std::string readString(int l)
    {
        char c[l];
        asio::read(serial,asio::buffer(&c,l+1));
        return std::string(c);
    }
    
    
private:
    boost::asio::io_service io;
    boost::asio::serial_port serial;
    //boost::asio::serial_port::
};

#endif
