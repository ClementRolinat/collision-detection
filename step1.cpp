#include "ibex.h"
#include "vibes.h"
#include <math.h>

using namespace std;
using namespace ibex;

Interval collisionCondition(Interval v, Interval x0, Interval y0, Interval th, Interval vi, Interval xi, Interval yi, Interval thi, Interval t){
    C1 = (v*cos(th)-vi*cos(thi))*t+xi-x0;
    C2 = (v*sin(th)-vi*sin(thi))*t+yi-y0;
    C3 = (v*sin(th)-vi*sin(thi))*(xi-x0)-(v*cos(th)-vi*cos(thi))*(yi-y0);
    if (C1.contains(0) and C2.contains(0) and C3.contains(0)){
        return 1;
    else{
        return 0;
    }
    }
}

int main(int argc, char** argv) {

    Interval T(0,30);

    double waypoints[2][2] = {{0,0},{40,50}};

    double Pos1[2][2] = {{7,12},{4.5,5.5}};
    IntervalVector obstacle1Pos(2, Pos1);

    double Pos2[2][2] = {{50,55},{30,35}};
    IntervalVector obstacle2Pos(2, Pos2);

    IntervalVector obstaclesPos[2] = {obstacle1Pos, obstacle2Pos};

    Interval boatSpeed(9,11);

    Interval boatHead;

    boatHead = acos(waypoints[1][0]/(sqrt(pow(waypoints[1][0],2)+pow(waypoints[1][1],2))))*Interval(0.9, 1.1);

    double pos[2][2] ={{-1,1},{-1,1}};
    IntervalVector boatInitPos(2, pos);

    bool collisionRisk = 0;


    vibes::beginDrawing();
    vibes::newFigure("step 1");
    vibes::setFigureProperties(vibesParams("x", 200, "y", 100, "width", 800, "height", 800));

    double t = T.lb();
    dt = 0.5;
    Interval x,y;
    while ( t<=T.ub()){
        x = boatSpeed*cos(boatHead)*t + boatInitPos[0];
        y = boatSpeed*sin(boatHead)*t + boatInitPos[1];
        vibes::drawBox(x.lb(), x.ub(), y.lb(), y.ub(), "[blue]");
        t+= dt;
    }

    for (int i = 0; i<2; i++){
        if (collisionRIsk == 0){
            collisionRisk = collisionCondition(boatSpeed, boatInitPos[0], boatInitPos[1], boatHead, Interval(0,0), obstaclesPos[i][0], obstaclesPos[i][1], Interval(0,0), T);
        }
        vibes::drawBox(obstaclesPos[i][0].lb(), obstaclesPos[i][0].ub(), obstaclesPos[i][1].lb(), obstaclesPos[i][1].ub(), "[red]");

        }
    }

    
    cout << collisionRisk << endl;
    

    vibes::endDrawing();


    
    




}