#include "util.hpp"
#include "../logging.hpp"

#include <math.h>

double my_angle(double x1, double y1, double x2, double y2){
    if(x1 == 0 && y1 ==0){
        return 0;
    }
    double n1 = sqrt(x1*x1+y1*y1);
    double n2 = sqrt(x2*x2+y2*y2);
    double cosa = (x1*x2+y1*y2)/(n1*n2);
    double siga = (x1*y2-y1*x2);
    
    double angle = acos(cosa);
    if(siga < 0){
        angle = -angle;
    }
    
    
    return angle;
}

double angleBetweenPI2(double a){
    double angle = a;
    if(a>3.14/2){
        angle = angle-3.14;
    }
    if(angle>3.14/2){
        angle = angle-3.14;
    }
    if(angle < -3.14/2){
        angle = angle+3.14;
    }
    if(angle < -3.14/2){
        angle = angle+3.14;
    }
    return angle;
}

bool isNotEqualDoubles2 (double a, double b, double epsilon)
{
   double diff = a - b;
   INFO(diff << " " << epsilon);
   return (diff > epsilon) || (-diff < -epsilon);
}
