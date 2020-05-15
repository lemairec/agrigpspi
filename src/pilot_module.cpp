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
    m_algo_k = config.m_algo_k;
    
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
    clear();
    
}

void PilotModule::clear(){
    m_value_volant = 0;
}

void PilotModule::run(double value){

    
    double value2 = m_algo_k*value;
    int res = value2 - m_value_volant;
    m_value_volant += res;
    
    
    std::ostringstream out;
    if(res<0){
        out << "$R;" << (-res) << "\n";
        
        
    } else {
        out << "$L;" << res << "\n";
    }
    INFO(value << " " << res);
    if(m_serial){
        m_serial->writeString(out.str());
    } else {
        WARN("###################serial");
    }
}

void PilotModule::test(int i){
    if(m_serial == NULL){
        return;
    }
    INFO("$R;1000\n");
    if(i<0){
        m_serial->writeString("$R;1000\n");
    } else {
        m_serial->writeString("$L;1000\n");
    }
}
