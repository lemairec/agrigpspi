#include "pilot_module.hpp"
#include "serial.hpp"
#include <math.h>
#include "serial.hpp"

PilotModule::PilotModule(){
    //setRef(LAT_REF, LON_REF);
}

void PilotModule::initOrLoadConfig(Config & config){
    if(m_serial){
        delete(m_serial);
    }
    m_serial = NULL;
    if(config.m_inputPilot != "none"){
        m_serial = new Serial(config.m_inputPilot,9600);
    }
    
}

void PilotModule::test(int i){
    if(m_serial == NULL){
        return;
    }
    if(i<0){
        m_serial->writeString("$R;10000\n");
    } else {
        m_serial->writeString("$L;1000\n");
    }
}
