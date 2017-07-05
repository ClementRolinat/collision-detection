#include "ibex.h"
#include "vibes.h"
#include "tools.h"
#include <unistd.h>

using namespace std;
using namespace ibex;


int main(int argc, char** argv) {
    

    double waypoints[2][2] = {{0,0},{40,50}};

    double Pos1[2][2] = {{50,55},{60,65}};
    IntervalVector obstacle1Pos(2, Pos1);

    double Pos2[2][2] = {{25,35},{50,55}};
    IntervalVector obstacle2Pos(2, Pos2);

    IntervalVector obstaclesPos[2] = {obstacle1Pos, obstacle2Pos};


    vibes::beginDrawing();
    vibes::newFigure("feasible path with fixed obstacles");
    vibes::setFigureProperties(vibesParams("x", 100, "y", 100, "width", 800, "height", 800));
    
    Contractor contractor;

    double _X[2][2] = {{0, 100},{0, 100}};
    IntervalVector X(2, _X);

    paving(X, contractor, obstaclesPos);
    


    vibes::endDrawing();

    return 0;
}