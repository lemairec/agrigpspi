#ifndef LINE_AB_H
#define LINE_AB_H

class LineAB{
public:
    GpsPoint m_pointA;
    GpsPoint m_pointB;
    
    double m_a = 0;
    double m_b = 0;
    double m_c = 0;
    double m_sqrt_m_a_m_b;

    double m_ab_x = 0.0;
    double m_ab_y = 0.0;

    double m_angleAB = 0;
    
};


#endif // LINE_AB_H
