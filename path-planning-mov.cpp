#include "ibex.h"
#include "vibes.h"
#include "tools.h"

using namespace std;
using namespace ibex;



void pavingMov(IntervalVector speed, vector<SepInter*> listSep){
    if (width(speed) < 0.2){
        return;
    }
    IntervalVector XinEnd(2);
    IntervalVector XoutEnd(2);
    IntervalVector Xin = speed;
    IntervalVector Xout = speed;
    vector<IntervalVector> listXout;
    vector<IntervalVector> listXin;


    vibes::drawBoxes({{speed[0].lb(), speed[0].ub(), speed[1].lb(), speed[1].ub()}}, "[cyan]");

    for (int i = 0; i < listSep.size(); i++){
        listSep[i]->SepInter::separate(Xin, Xout);
        //cout << "test2" << endl;
        listXout.push_back(Xout);
        Xin = speed;
        Xout = speed;
    }

    XoutEnd = listXout[0];
    
    for (int i = 1; i < listXout.size(); i++ ){
        XoutEnd = XoutEnd | listXout[i];
    }
    

    vibes::drawBoxes({{XoutEnd[0].lb(), XoutEnd[0].ub(), XoutEnd[1].lb(), XoutEnd[1].ub()}}, "[red]");

    Xin = XoutEnd;
    Xout = XoutEnd;

    for (int i = 0; i < listSep.size(); i++){
        listSep[i]->SepInter::separate(Xin, Xout);
        listXin.push_back(Xin);
        Xin = XoutEnd;
        Xout = XoutEnd;
    }   

    XinEnd = listXin[0];
    
    for (int i = 0; i < listXin.size(); i++){
        XinEnd = XinEnd & listXin[i];
    }

    vibes::drawBoxes({{XinEnd[0].lb(), XinEnd[0].ub(), XinEnd[1].lb(), XinEnd[1].ub()}}, "[yellow]");

    pavingMov(left(XinEnd), listSep);
    pavingMov(right(XinEnd), listSep);
}



int main(int argc, char** argv) {
    Interval T(0,30);

    double pos[2][2] ={{-1,1},{-1,1}};
    IntervalVector boatInitPos(2, pos);


    vector<IntervalVector> obstacles;
    double pos1[4][2] = {{2, 2.4}, {30, 35}, {-10, -8}, {2.9, 3.5}}; // {speed, posInitx, posInity, heading}
    double pos2[4][2] = {{3, 3.4}, {10, 12}, {0, 3}, {0.5, 0.6}};
    IntervalVector obs1(4, pos1);
    IntervalVector obs2(4, pos2);
    obstacles.push_back(obs1);
    obstacles.push_back(obs2);




    vibes::beginDrawing();
    vibes::newFigure("feasible speed");
    vibes::setFigureProperties(vibesParams("x", 100, "y", 100, "width", 800, "height", 800));


    Variable vx, vy;
    vector<SepInter*> listSep;
    Function* pf1;
    Function* pf2;
    Function* pf3;
    SepFwdBwd* pSep1;
    SepFwdBwd* pSep2;
    SepFwdBwd* pSep3;
    SepInter* pSep;


    

    for ( int i = 0; i < obstacles.size(); i++){
        pf1 = new Function(vx, vy, (vx - obstacles[i][0]*cos(obstacles[i][3]))*T  +boatInitPos[0] - obstacles[i][1]);
        pf2 = new Function(vx, vy, (vy - obstacles[i][0]*sin(obstacles[i][3]))*T  + boatInitPos[1] - obstacles[i][2]);
        pf3 = new Function(vx, vy, (vy - obstacles[i][0]*sin(obstacles[i][3]))*(boatInitPos[0] - obstacles[i][1]) - (vx - obstacles[i][0]*cos(obstacles[i][3]))*(boatInitPos[1] - obstacles[i][2]));

        pSep1 = new SepFwdBwd(*pf1, Interval(0,0));
        pSep2 = new SepFwdBwd(*pf2, Interval(0,0));
        pSep3 = new SepFwdBwd(*pf3, Interval(0,0));
        pSep = new SepInter(*pSep1, *pSep2, *pSep3);

        listSep.push_back(pSep);
    }


    double _speed[2][2] = {{-10, 10}, {-10, 10}};
    IntervalVector speed(2, _speed);
  


    pavingMov(speed, listSep);


    return 0;

}
