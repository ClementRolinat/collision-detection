#include "ibex.h"
#include <math.h>
#include "node.h"




class Contractor 
{
    public: void contract(ibex::IntervalVector& X, std::vector<ibex::IntervalVector> obstaclesPos){
        ibex::IntervalVector result(2, ibex::Interval::EMPTY_SET);
        for (int i = 0; i < obstaclesPos.size(); i++){
            result = result | (X & obstaclesPos[i]);
        }
        X = result;
    }
};

float width(const ibex::Interval& x);

ibex::Interval left(const ibex::Interval& x);

ibex::Interval right(const ibex::Interval& x);

float width(const ibex::IntervalVector& X);

ibex::IntervalVector left(const ibex::IntervalVector& X);

ibex::IntervalVector right(const ibex::IntervalVector& X);

void paving(ibex::IntervalVector X, Contractor& contractor, std::vector<ibex::IntervalVector> obstaclesPos, std::vector<ibex::IntervalVector>* listBoxes);

void release(Node* a, Node* b);

Node* extractMin(std::vector<Node*> &Queue);

bool collisionCondition(ibex::Interval v, ibex::Interval x0, ibex::Interval y0, ibex::Interval th, ibex::Interval vi, ibex::Interval xi, ibex::Interval yi, ibex::Interval thi, ibex::Interval t);
