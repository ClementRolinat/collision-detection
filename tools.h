#include "ibex.h"
#include <math.h>

using namespace std;
using namespace ibex;

class Contractor 
{
    public: void contract(IntervalVector& X, IntervalVector obstaclesPos[]){
        IntervalVector result(2, Interval::EMPTY_SET);
        for (int i = 0; i < 2; i++){
            result = result | (X & obstaclesPos[i]);
        }
        X = result;
    }
};

float width(const Interval& x);

Interval left(const Interval& x);

Interval right(const Interval& x);

float width(const IntervalVector& X);

IntervalVector left(const IntervalVector& X);

IntervalVector right(const IntervalVector& X);

void paving(IntervalVector X, Contractor& contractor, IntervalVector obstaclesPos[]);


bool collisionCondition(Interval v, Interval x0, Interval y0, Interval th, Interval vi, Interval xi, Interval yi, Interval thi, Interval t);
