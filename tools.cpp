#include "ibex.h"
#include <math.h>
#include "tools.h"
#include "vibes.h"

using namespace std;
using namespace ibex;

float width(const Interval& x){
    if (x.is_empty()){
        return 0;
    }
    return x.ub() - x.lb();
}

Interval left(const Interval& x){
    if (x.is_empty()){
        return Interval(Interval::EMPTY_SET);
    }
    return Interval(x.lb(), (x.lb() + x.ub())/2.);
}

Interval right(const Interval& x){
    if (x.is_empty()){
        return Interval(Interval::EMPTY_SET);
    }
    return Interval((x.lb() + x.ub())/2., x.ub());
}

float width(const IntervalVector& X){
    if ( X.is_empty()){
        return 0;
    }
    return max(width(X[0]), width(X[1]));
}

IntervalVector left(const IntervalVector& X){
    IntervalVector newX(2);
    if (X.is_empty()){
        return newX.empty(2);
    }
    if (width(X[0]) > width(X[1])){
        newX[0] = left(X[0]);
        newX[1] = X[1];
        return newX;
    }
    newX[0] = X[0];
    newX[1] = left(X[1]);
    return newX;
}

IntervalVector right(const IntervalVector& X){
    IntervalVector newX(2);
    if (X.is_empty()){
        return newX.empty(2);
    }
    if (width(X[0]) > width(X[1])){
        newX[0] = right(X[0]);
        newX[1] = X[1];
        return newX;
    }
    newX[0] = X[0];
    newX[1] = right(X[1]);       
    return newX;
}

void paving(IntervalVector X, Contractor& contractor, vector<IntervalVector> obstaclesPos, vector<IntervalVector>* listBoxes){
    
    IntervalVector X_past(X);
    contractor.contract(X, obstaclesPos);

    
    IntervalVector newBox(2);

    IntervalVector* ListComplementary;

    int size = X.complementary(ListComplementary);

    for ( int i = 0; i < size; i++){
        newBox = ListComplementary[i]&X_past;
        if (!newBox.is_flat()){
            (*listBoxes).push_back(newBox);
            vibes::drawBoxes({{newBox[0].lb(), newBox[0].ub(), newBox[1].lb(), newBox[1].ub()}}, "black[cyan]");
        }
    }
    

    for (int i = 0; i< obstaclesPos.size(); i++){
        if (X.is_subset(obstaclesPos[i])){
            vibes::drawBoxes({{obstaclesPos[i][0].lb(), obstaclesPos[i][0].ub(), obstaclesPos[i][1].lb(), obstaclesPos[i][1].ub()}}, "black[red]");
            return;
        }
    }


    paving(left(X), contractor, obstaclesPos, listBoxes);
    paving(right(X), contractor, obstaclesPos, listBoxes);
}

void release(Node* a, Node* b){
    if (b->getDist() > (a->getDist() + 1)){
        b->setDist(a->getDist()+1);
        b->setPred(a);
    }
}

Node* extractMin(vector<Node*> &Queue){
    Node* pNodeMinDist = Queue[0];
    double minDist = Queue[0]->getDist();
    int position = 0;
    for ( int i = 0; i < Queue.size(); i++){
        if (Queue[i]->getDist() < minDist){
            pNodeMinDist = Queue[i];
            minDist = Queue[i]->getDist();
            position = i;
        }
    }
    Queue.erase(Queue.begin() + position);
    return pNodeMinDist;
}


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