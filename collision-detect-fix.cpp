#include "ibex.h"
#include "vibes.h"
#include "tools.h"
#include <math.h>



int main(int argc, char** argv) {

    Interval T(0,30);

    double waypoints[2][2] = {{0,0},{40,50}};

    double Pos1[2][2] = {{50,55},{60,65}};
    IntervalVector obstacle1Pos(2, Pos1);

    double Pos2[2][2] = {{25,35},{50,55}};
    IntervalVector obstacle2Pos(2, Pos2);

    IntervalVector obstaclesPos[2] = {obstacle1Pos, obstacle2Pos};

    Interval boatSpeed;
    boatSpeed = (sqrt(pow(waypoints[1][0],2)+pow(waypoints[1][1],2))/T.ub())*Interval(0.9,1.1);

    Interval boatHead;

    boatHead = acos(waypoints[1][0]/(sqrt(pow(waypoints[1][0],2)+pow(waypoints[1][1],2))))*Interval(0.9, 1.1);

    double pos[2][2] ={{-1,1},{-1,1}};
    IntervalVector boatInitPos(2, pos);

    bool collisionRisk = 0;


    vibes::beginDrawing();
    vibes::newFigure("collision detection with fixed obstacles");
    vibes::setFigureProperties(vibesParams("x", 100, "y", 100, "width", 800, "height", 800));

    double t = T.lb();
    double dt = 1;
    Interval x,y;
    while ( t<=T.ub()){
        x = boatSpeed*cos(boatHead)*t + boatInitPos[0];
        y = boatSpeed*sin(boatHead)*t + boatInitPos[1];
        vibes::drawBox(x.lb(), x.ub(), y.lb(), y.ub(), "[blue]");
        t+= dt;
    }

    for (int i = 0; i<2; i++){
        if (collisionRisk == 0){
            collisionRisk = collisionCondition(boatSpeed, boatInitPos[0], boatInitPos[1], boatHead, Interval(0,0), obstaclesPos[i][0], obstaclesPos[i][1], Interval(0,0), T);
        }
        vibes::drawBox(obstaclesPos[i][0].lb(), obstaclesPos[i][0].ub(), obstaclesPos[i][1].lb(), obstaclesPos[i][1].ub(), "[red]");

    }

    
    cout << collisionRisk << endl;
    

    vibes::endDrawing();

    return 0;
}