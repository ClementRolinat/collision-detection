#include <thread>
#include <chrono>
#include "ibex.h"
#include "vibes.h"
#include "tools.h"
#include <math.h>
#include <unistd.h>

using namespace std;
using namespace ibex;

int main(int argc, char** argv) {
    Interval T(0,30);

    double waypoints[2][2] = {{0,0},{40,50}};

    Interval boatSpeed;
    boatSpeed = (sqrt(pow(waypoints[1][0],2)+pow(waypoints[1][1],2))/T.ub())*Interval(0.9,1.1);

    Interval boatHead;

    boatHead = acos(waypoints[1][0]/(sqrt(pow(waypoints[1][0],2)+pow(waypoints[1][1],2))))*Interval(0.9, 1.1);

    double pos[2][2] ={{-1,1},{-1,1}};
    IntervalVector boatInitPos(2, pos);

    Interval obstacles[2][4] = {{Interval(2.3,2.4), Interval(30,31), Interval(-10,-9), Interval(2.2,2.3)},{Interval(0,0), Interval(50,52), Interval(20, 22), Interval(2,2.3)}}; // {speed, posInitx, posInity, heading}

    bool collisionRisk = 0;


    vibes::beginDrawing();
    vibes::newFigure("collision detection with moving obstacles");
    vibes::setFigureProperties(vibesParams("x", 100, "y", 100, "width", 800, "height", 800));

    for (int i = 0; i<2; i++){
        if (collisionRisk == 0){
            collisionRisk = collisionCondition(boatSpeed, boatInitPos[0], boatInitPos[1], boatHead, obstacles[i][0], obstacles[i][1], obstacles[i][2], obstacles[i][3], T);
        }
    }

    double t = T.lb();
    double dt = 0.5;
    Interval x,y;
    while ( t<=T.ub()){
        x = boatSpeed*cos(boatHead)*t + boatInitPos[0];
        y = boatSpeed*sin(boatHead)*t + boatInitPos[1];
        vibes::drawBox(x.lb(), x.ub(), y.lb(), y.ub(), "[blue]");
        for (int i = 0; i<2; i++){
            x = obstacles[i][0]*cos(obstacles[i][3])*t + obstacles[i][1];
            y = obstacles[i][0]*sin(obstacles[i][3])*t + obstacles[i][2];
            vibes::drawBox(x.lb(), x.ub(), y.lb(), y.ub(), "[red]");
        }

        this_thread::sleep_for(chrono::milliseconds(100));
        vibes::clearFigure("collision detection with moving obstacles");
        t+= dt;
    }

    cout << collisionRisk << endl;

    return 0;

}






