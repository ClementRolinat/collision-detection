#include "ibex.h"
#include <math.h>
#include "tools.h"


using namespace std;
using namespace ibex;


bool collisionCondition(Interval v, Interval x0, Interval y0, Interval th, Interval vi, Interval xi, Interval yi, Interval thi, Interval t){
    Interval C1, C2, C3;
    C1 = (v*cos(th)-vi*cos(thi))*t+x0-xi;
    C2 = (v*sin(th)-vi*sin(thi))*t+y0-yi;
    C3 = (v*sin(th)-vi*sin(thi))*(x0-xi)-(v*cos(th)-vi*cos(thi))*(y0-yi);
    if (C1.contains(0) and C2.contains(0) and C3.contains(0)){
        return 1;
    }
    else{
        return 0;
    }
}