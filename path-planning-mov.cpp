#include "ibex.h"
#include "vibes.h"
#include "tools.h"
#include "sepProj.h"
#include <ctime>


using namespace std;
using namespace ibex;

double midPointDistance(IntervalVector X, IntervalVector Y){
    return sqrt(pow(X.mid()[0] - Y.mid()[0], 2) + pow(X.mid()[1] - Y.mid()[1], 2));
}

IntervalVector findClosest(vector<IntervalVector> listBoxes, IntervalVector boatSpeed){
    double dist = 1000000000;
    IntervalVector outputBox(2); 
    for ( int i = 0; i < listBoxes.size(); i++){
        if (midPointDistance(boatSpeed, listBoxes[i]) < dist){
            dist = midPointDistance(boatSpeed, listBoxes[i]);
            outputBox = listBoxes[i];
        }
    }
    return outputBox;
}

void createSepBorder(vector<vector<double>> border, vector <SepInter*> &listSep, IntervalVector boatInitPos, Interval T){
    Variable vx, vy;
    Function* pf1;
    Function* pf2;
    Function* pf3;
    Function* pf4;
    SepFwdBwd* pSep1;
    SepFwdBwd* pSep2;
    SepFwdBwd* pSep3;
    SepFwdBwd* pSep4;
    SepInter* pSep;    

    for (int i = 0; i < border.size(); i++){
        pf1 = new Function(vx, vy, ((border[i][0] - (vx*T.ub() + boatInitPos[0]))*(border[i][1] - boatInitPos[1]) - 
                                                    (border[i][1] - (vy*T.ub() + boatInitPos[1]))*(border[i][0] - boatInitPos[0]))*
                                                    ((border[(i+1)%border.size()][0] - (vx*T.ub() + boatInitPos[0]))*(border[(i+1)%border.size()][1] - boatInitPos[1]) - 
                                                    (border[(i+1)%border.size()][1] - (vy*T.ub() + boatInitPos[1]))*(border[(i+1)%border.size()][0] - boatInitPos[0]))
                                                    );
        
        pf2 = new Function(vx, vy, ((border[(i+1)%border.size()][0] - border[i][0])*(border[i][1] - boatInitPos[1]) - 
                                                    (border[(i+1)%border.size()][1] - border[i][1])*(border[i][0] - boatInitPos[0]))*
                                                    ((border[(i+1)%border.size()][0] - border[i][0])*(border[i][1] - (vy*T.ub() + boatInitPos[1])) - 
                                                    (border[(i+1)%border.size()][1] - border[i][1])*(border[i][0] - (vx*T.ub() + boatInitPos[0])))
                                                    );
        
        pf3 = new Function(vx, vy, max(abs(max(border[i][0], border[(i+1)%border.size()][0]) - max(boatInitPos[0].ub(), vx*T.ub() + boatInitPos[0].ub())), sqrt(sqr(min(boatInitPos[0].lb(), vx*T.ub() + boatInitPos[0].lb()) - min(border[i][0], border[(i+1)%border.size()][0])))) -
                                                    max(max(border[i][0], border[(i+1)%border.size()][0]) - min(border[i][0], border[(i+1)%border.size()][0]), max(boatInitPos[0].ub(), vx*T.ub() + boatInitPos[0].ub()) - min(boatInitPos[0].lb(), vx*T.ub() + boatInitPos[0].lb()))
                                                    );

        pf4 = new Function(vx, vy, max(abs(max(border[i][1], border[(i+1)%border.size()][1]) - max(boatInitPos[1].ub(), vy*T.ub() + boatInitPos[1].ub())), sqrt(sqr(min(boatInitPos[1].lb(), vy*T.ub() + boatInitPos[1].lb()) - min(border[i][1], border[(i+1)%border.size()][1])))) -
                                                    max(max(border[i][1], border[(i+1)%border.size()][1]) - min(border[i][1], border[(i+1)%border.size()][1]), max(boatInitPos[1].ub(), vy*T.ub() + boatInitPos[1].ub()) - min(boatInitPos[1].lb(), vy*T.ub() + boatInitPos[1].lb()))
                                                    );
        

        pSep1 = new SepFwdBwd(*pf1, LEQ);
        pSep2 = new SepFwdBwd(*pf2, LEQ);
        
        pSep3 = new SepFwdBwd(*pf3, LEQ);
        pSep4 = new SepFwdBwd(*pf4, LEQ);
        
        pSep = new SepInter(*pSep1, *pSep2, *pSep3, *pSep4);

        listSep.push_back(pSep);
    }
}


void pavingMov(IntervalVector speed, vector<SepInter*> listSep, vector<IntervalVector>& listBoxes){
    if (speed.is_empty()){
        return;
    }
    else if (speed.max_diam() < 0.1){
        return;
    }

    IntervalVector XinEnd(2);
    IntervalVector XoutEnd(2);
    IntervalVector Xin = speed;
    IntervalVector Xout = speed;
    IntervalVector maybeBox(2);
    vector<IntervalVector> listXout;
    vector<IntervalVector> listXin;

    //sleep(1);

    for (int i = 0; i < listSep.size(); i++){
        
        listSep[i]->SepInter::separate(Xin, Xout);
        listXout.push_back(Xout);
        listXin.push_back(Xin);
        Xin = speed;
        Xout = speed;
    }

    XoutEnd = listXout[0];
    XinEnd = listXin[0];
 
    for (int i = 1; i < listXout.size(); i++ ){
        XoutEnd = XoutEnd | listXout[i];
        XinEnd = XinEnd & listXin[i];
    }
 
    IntervalVector newBox(2);

    IntervalVector* ListComplementary;
    
    int size = XoutEnd.complementary(ListComplementary);
    
    for ( int i = 0; i < size; i++){
        newBox = ListComplementary[i]&speed;
        if (!newBox.is_flat()){
            listBoxes.push_back(newBox);
            vibes::drawBoxes({{newBox[0].lb(), newBox[0].ub(), newBox[1].lb(), newBox[1].ub()}}, "[cyan]");
        }
    }

    
    vibes::drawBoxes({{XoutEnd[0].lb(), XoutEnd[0].ub(), XoutEnd[1].lb(), XoutEnd[1].ub()}}, "[red]");

    //sleep(1);
      
    maybeBox = XinEnd & XoutEnd;

    vibes::drawBoxes({{maybeBox[0].lb(), maybeBox[0].ub(), maybeBox[1].lb(), maybeBox[1].ub()}}, "[yellow]");
    //sleep(1);
    pavingMov(left(maybeBox), listSep, listBoxes);
    
    pavingMov(right(maybeBox), listSep, listBoxes);
    
}


int main(int argc, char** argv) {
    clock_t start = clock();

    Interval T(0,30);

    vector<vector<vector<double>>> borderList = {{/*{-200, 0}, {-300, 0},{0, 200},{200, 0}, {300, 0}, {0, -200}}, {{-60, 0}, {-60, 50}, {-110, 50}, {-110, 0}*/}};

    double pos[2][2] ={{-1,1},{-1,1}};
    IntervalVector boatInitPos(2, pos);

    double _boatSpeed[2][2] = {{-1.4,-1.2}, {-0.5,-0.3}};
    IntervalVector boatSpeed(2,_boatSpeed);


    vector<IntervalVector> obstacles;
    double pos1[4][2] = {{2, 2.1}, {30, 31}, {-10, -9}, {2.9, 3.1}}; // {speed, posInitx, posInity, heading}
    double pos2[4][2] = {{3, 3.1}, {10, 11}, {0, 1}, {2.0, 2.2}};
    IntervalVector obs1(4, pos1);
    IntervalVector obs2(4, pos2);
    obstacles.push_back(obs1);
    obstacles.push_back(obs2);




    vibes::beginDrawing();
    vibes::newFigure("feasible speed");
    vibes::setFigureProperties(vibesParams("x", 100, "y", 100, "width", 800, "height", 800));


    
    Variable vx, vy, ai, bi, t;
    vector<SepInter*> listSep;
    IntervalVector proj(3);
    Function* pf1;
    Function* pf2;
    Function* pf3;
    SepFwdBwd* pSep1;
    SepFwdBwd* pSep2;
    SepFwdBwd* pSep3;
    SepProj* pSepProj1;
    SepProj* pSepProj2;
    SepProj* pSepProj3;
    SepInter* pSep;

    for (int i = 0; i<borderList.size(); i++){
        createSepBorder(borderList[i], listSep, boatInitPos, T);
    }
    

    for ( int i = 0; i < obstacles.size(); i++){
        pf1 = new Function(vx, vy, ai, bi, t, (vx - ai)*t);
        pf2 = new Function(vx, vy, ai, bi, t, (vy - bi)*t);
        pf3 = new Function(vx, vy, ai, bi, t, (vy - bi)/(vx - ai));

        proj[0] = obstacles[i][0]*cos(obstacles[i][3]);
        proj[1] = obstacles[i][0]*sin(obstacles[i][3]);
        proj[2] = T;

        pSep1 = new SepFwdBwd(*pf1, obstacles[i][1] - boatInitPos[0]);
        pSep2 = new SepFwdBwd(*pf2, obstacles[i][2] - boatInitPos[1]);
        pSep3 = new SepFwdBwd(*pf3, (obstacles[i][2] - boatInitPos[1])/(obstacles[i][1] - boatInitPos[0]));
        pSepProj1 = new SepProj(*pSep1, proj, 1000000);
        pSepProj2 = new SepProj(*pSep2, proj, 1000000);
        pSepProj3 = new SepProj(*pSep3, proj, 1000000);

        pSep = new SepInter(*pSepProj1, *pSepProj2, *pSepProj3);

        listSep.push_back(pSep);
    }


    double _speed[2][2] = {{-10, 10}, {-10, 10}};
    IntervalVector speed(2, _speed);
  
    vector<IntervalVector> listBoxes;

    pavingMov(speed, listSep, listBoxes);

    
    //vibes::drawBoxes({{boatSpeed[0].lb(), boatSpeed[0].ub(), boatSpeed[1].lb(), boatSpeed[1].ub()}}, "[blue]");

    IntervalVector newSpeed = findClosest(listBoxes, boatSpeed);

    //vibes::drawBoxes({{newSpeed[0].lb(), newSpeed[0].ub(), newSpeed[1].lb(), newSpeed[1].ub()}}, "[green]");

    clock_t end = clock();
    cout << "execution time " << (float(end - start))/CLOCKS_PER_SEC << endl;

    return 0;

}
