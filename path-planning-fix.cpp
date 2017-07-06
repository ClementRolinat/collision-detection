#include "ibex.h"
#include "vibes.h"
#include "tools.h"
#include "node.h"

using namespace std;
using namespace ibex;

int main(int argc, char** argv) {
    

    double waypoints[2][2] = {{0,0},{40,50}};

    double Pos1[2][2] = {{50,55},{60,65}};
    IntervalVector obstacle1Pos(2, Pos1);

    double Pos2[2][2] = {{27,35},{47,52}};
    IntervalVector obstacle2Pos(2, Pos2);

    double Pos3[2][2] = {{13, 17},{54,58}};
    IntervalVector obstacle3Pos(2, Pos3);

    vector<IntervalVector> obstaclesPos;
    obstaclesPos.push_back(obstacle1Pos);
    obstaclesPos.push_back(obstacle2Pos);
    obstaclesPos.push_back(obstacle3Pos);


    vibes::beginDrawing();
    vibes::newFigure("feasible path with fixed obstacles");
    vibes::setFigureProperties(vibesParams("x", 100, "y", 100, "width", 800, "height", 800));
    
    Contractor contractor;

    double _X[2][2] = {{0, 100},{0, 100}};
    IntervalVector X(2, _X);

    vector<IntervalVector> listBoxes;
    paving(X, contractor, obstaclesPos, &listBoxes);


    for (int i = 0; i < listBoxes.size(); i++){
        cout << listBoxes[i] << endl;
    }
    
    


    vibes::endDrawing();

    return 0;
}