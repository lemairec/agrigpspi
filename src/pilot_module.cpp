#include "pilot_module.hpp"
#include "serial.hpp"
#include <math.h>
#include "logging.hpp"

PilotModule::PilotModule(){
    //setRef(LAT_REF, LON_REF);
}

void PilotModule::initOrLoadConfig(Config & config){
    if(m_serial){
        delete(m_serial);
    }
    m_p = config.m_kp;
    m_i = config.m_ki;
    m_d = config.m_kd;
    m_serial = NULL;
    if(config.m_inputPilot != "none"){
        try {
            m_serial = new Serial(config.m_inputPilot,9600);
        } catch(boost::system::system_error& e)
        {
            WARN(config.m_input << " " << config.m_baudrate);
            m_serial = NULL;
        }
       
    }
    
}

void PilotModule::run(double value){
    double dv = value-m_last_value;
    int res = m_p*value+m_d*dv;
    
    INFO(m_p << "x" << value << "+" << m_d << "x" << dv << "  " << res);
    m_last_value=value;
    if(m_serial == NULL){
        return;
    }
    
    
    std::ostringstream out;
    if(res<0){
        out << "$R;" << (-res) << "\n";
        
        
    } else {
        out << "$L;" << res << "\n";
    }
    INFO(out.str()  );
    m_serial->writeString(out.str());
    
}

void PilotModule::test(int i){
    if(m_serial == NULL){
        return;
    }
    if(i<0){
        m_serial->writeString("$R;1000\n");
    } else {
        m_serial->writeString("$L;1000\n");
    }
}
