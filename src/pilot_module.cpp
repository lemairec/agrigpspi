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
    m_inverse = config.m_pilot_inverse;
    if(config.m_inputPilot != "none"){
        try {
            m_serial = new Serial(config.m_inputPilot,config.m_baudratePilot);
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
    std::ostringstream out;
    int res = value*m_algo_k;
    if(m_inverse){
        res = -res;
    }
    if(res<0){
        out << "$G;-" << (-res) << "\n";
        
        
    } else {
        out << "$G; " << res << "\n";
    }
    
    INFO(out.str());
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
        m_serial->writeString("$G; 1000\n");
    } else {
        m_serial->writeString("$G;-1000\n");
    }
}
