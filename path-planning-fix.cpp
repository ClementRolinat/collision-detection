#include "ibex.h"
#include "vibes.h"
#include "tools.h"
#include <ctime>

using namespace std;
using namespace ibex;

int main(int argc, char** argv) {
    
    clock_t start = clock();

    double waypoints[2][2] = {{0,0},{40,50}};

    double pos[2][2] ={{-1,1},{-1,1}};
    IntervalVector boatInitPos(2, pos);

    double Pos1[2][2] = {{35,39},{20,24}};
    IntervalVector obstacle1Pos(2, Pos1);

    double Pos2[2][2] = {{27,32},{47,51}};
    IntervalVector obstacle2Pos(2, Pos2);

    double Pos3[2][2] = {{13, 17},{54,58}};
    IntervalVector obstacle3Pos(2, Pos3);

    double Pos4[2][2] = {{60,63},{48,51}};
    IntervalVector obstacle4Pos(2,Pos4);

    vector<IntervalVector> obstaclesPos;
    obstaclesPos.push_back(obstacle1Pos);
    obstaclesPos.push_back(obstacle2Pos);
    obstaclesPos.push_back(obstacle3Pos);
    obstaclesPos.push_back(obstacle4Pos);


    vibes::beginDrawing();
    vibes::newFigure("feasible path with fixed obstacles");
    vibes::setFigureProperties(vibesParams("x", 100, "y", 100, "width", 800, "height", 800));
    
    Contractor contractor;

    double _X[2][2] = {{-10, 80},{-10, 80}};
    IntervalVector X(2, _X);

    vector<IntervalVector> listBoxes;
    paving(X, contractor, obstaclesPos, &listBoxes);


    
    vector<Node> listNodes;
    listNodes.reserve(100);

    for ( int i = 0; i < listBoxes.size(); i++){
        Node newNode(listBoxes[i]);
        if (boatInitPos.is_subset(newNode.getValue())){
            newNode.setDist(0);
        }
        listNodes.push_back(newNode);
    }

    for (int i = 0; i < listNodes.size(); i++){
        for (int j = 0; j < listNodes.size(); j++){
            if (listNodes[i].getValue().intersects(listNodes[j].getValue()) and j != i){
                listNodes[i].addNeighbor(&listNodes[j]);
            }
        }
    }


    vector<Node*> priorityQueue;
    priorityQueue.reserve(100);

    for (int i = 0; i < listNodes.size(); i++){
        priorityQueue.push_back(&listNodes[i]);
    }

    Node* currentNode;
    while (priorityQueue.size() != 0){
        currentNode = extractMin(priorityQueue);
        for (int i = 0; i < currentNode->getNeighbor().size(); i++){
            release(currentNode, currentNode->getNeighbor()[i]);
        }
    }

    Node* origin;
    for (int i = 0; i < listNodes.size(); i++){
        if (listNodes[i].getValue()[0].contains(waypoints[1][0]) && listNodes[i].getValue()[1].contains(waypoints[1][1])){
            origin = &listNodes[i];
        }
    }

    currentNode = origin;
    bool endLoop = 0;
    vector<double> pathx;
    vector<double> pathy;
    pathx.push_back(waypoints[1][0]);
    pathy.push_back(waypoints[1][1]);
    //vector<IntervalVector> boxesPath;
    while (endLoop == 0){
        //boxesPath.push_back(currentNode->getValue());
        vibes::drawBoxes({{currentNode->getValue()[0].lb(), currentNode->getValue()[0].ub(), currentNode->getValue()[1].lb(), currentNode->getValue()[1].ub()}}, "black[yellow]");
        if (currentNode->getPred() == NULL){
            endLoop = 1;
        }
        else{
            if (currentNode->getValue()[0].overlaps(currentNode->getPred()->getValue()[0])){
                if (currentNode->getValue()[1].lb() == currentNode->getPred()->getValue()[1].ub()){
                    pathx.push_back((currentNode->getValue()[0] & currentNode->getPred()->getValue()[0]).mid());
                    pathy.push_back(currentNode->getValue()[1].lb());
                }
                else{
                    pathx.push_back((currentNode->getValue()[0] & currentNode->getPred()->getValue()[0]).mid());
                    pathy.push_back(currentNode->getValue()[1].ub());
                }
            }
            else{
                if (currentNode->getValue()[0].lb() == currentNode->getPred()->getValue()[0].ub()){
                    pathx.push_back(currentNode->getValue()[0].lb());
                    pathy.push_back((currentNode->getValue()[1] & currentNode->getPred()->getValue()[1]).mid());
                }
                else{
                    pathx.push_back(currentNode->getValue()[0].ub());
                    pathx.push_back((currentNode->getValue()[1] & currentNode->getPred()->getValue()[1]).mid());
                }
            }
        }
        currentNode = currentNode->getPred();
    }

    pathx.push_back(waypoints[0][0]);
    pathy.push_back(waypoints[0][1]);

    vibes::drawLine(pathx, pathy, "red");


    clock_t end = clock();
    cout << (float(end - start))/CLOCKS_PER_SEC << endl;

    vibes::endDrawing();

    return 0;
}