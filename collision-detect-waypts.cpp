#include "ibex.h"
#include "vibes.h"
#include "tools.h"
#include <math.h>
#include <unistd.h>
#include <thread>
#include <chrono>

using namespace std;
using namespace ibex;

int main(int argc, char** argv) {

    //coordinate {x,y} of the waypoints
    vector<vector<double>> waypoints = {{0,0},{40,50},{-10,70}};
    
    //initial position of the boat,  it must be somewhere in the segment between the 2 firsts waypoints
    double pos[2][2] ={{-1,1},{-1,1}};
    IntervalVector boatInitPos(2, pos);

    //initial boat speed
    Interval boatSpeed(2,2.5);

    Interval boatHead;
    //boatHead = acos(waypoints[1][0]/(sqrt(pow(waypoints[1][0],2)+pow(waypoints[1][1],2))));


    Interval T(0,0);

    Interval obstacles[2][4] = {{Interval(1.5,1.7), Interval(30,35), Interval(-10,-8), Interval(1.8,1.9)},{Interval(3,3.5), Interval(10,12), Interval(0, 3), Interval(0.5,0.6)}}; // {speed, posInitx, posInity, heading}

    bool collisionRisk = 0;

    vibes::beginDrawing();
    vibes::newFigure("collision detection with boat following several waypoints");
    vibes::setFigureProperties(vibesParams("x", 100, "y", 100, "width", 800, "height", 800));

    for ( int i = 1; i< waypoints.size(); i++){
        
        if (i != 1){
            boatInitPos[0] = boatSpeed*cos(boatHead)*T.ub() + boatInitPos[0];
            boatInitPos[1] = boatSpeed*sin(boatHead)*T.ub() + boatInitPos[1];

            for (int j = 0; j<2; j++){
                obstacles[j][1] = obstacles[j][0]*cos(obstacles[j][3])*T.ub() + obstacles[j][1];
                obstacles[j][2] = obstacles[j][0]*sin(obstacles[j][3])*T.ub() + obstacles[j][2];
            }
        }

        T = Interval(0, (sqrt(pow(waypoints[i][0] - waypoints[i-1][0],2)+pow(waypoints[i][1] - waypoints[i-1][1],2)))/boatSpeed.mid());

        //update the boat heading
        //assume that the boat heading will be aligned with the 2 waypoints
        boatHead = atan2(waypoints[i][1] - waypoints[i-1][1], waypoints[i][0] - waypoints[i-1][0]);

        

        for (int i = 0; i<2; i++){

            if (collisionRisk == 0){
                collisionRisk = collisionCondition(boatSpeed, boatInitPos[0], boatInitPos[1], boatHead, obstacles[i][0], obstacles[i][1], obstacles[i][2], obstacles[i][3], T);
            }
        }
        cout << collisionRisk << endl;
        
        double t = T.lb();
        double dt = 1;
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
            vibes::clearFigure("collision detection with boat following several waypoints");
            t+= dt;
        }
    }    

    return 0;
}