#include "stat.hpp"
#include "../logging.hpp"

#include <math.h>


void ValueStat::addNewValue(double value){
    if(value > 10000){
        return;
    }
    m_values.push_front(value);
    while(m_values.size()>50){
        m_values.pop_back();
    }
    
    double sum = 0;
    double sum_carre = 0;
    for(auto c : m_values){
        sum += c;
        sum_carre += c*c;
    }
    m_moy = sum/((double)m_values.size());
    m_moy_carre = sqrt(sum_carre/((double)m_values.size()));
   
    double sum2 = 0;
    for(auto c : m_values){
        sum2 += (m_moy-c)*(m_moy-c);
    }
    m_et = sqrt(sum2/m_values.size());
}
